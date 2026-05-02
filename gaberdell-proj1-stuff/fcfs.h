#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "process.h"

typedef struct node {
    simProcess *process;
    struct node *next;
} queueNode;

queueNode *start = NULL;
queueNode *end = NULL;

void printQueue() {
    printf("[Q:");
    if(start==NULL) { printf(" -]\n"); return; }
    queueNode *curr = start;
    while(curr != NULL) {
        printf(" %s", curr->process->processID);
        curr = curr->next;
        break;
    } printf("]\n");
}

void addProcess(simProcess *toAdd) {
    queueNode *newNode = malloc(sizeof(queueNode));
    newNode->process = toAdd;
    newNode->next = NULL;
    if(start == NULL) start = newNode;
    if(end != NULL) end->next = newNode;
    end = newNode;
}

void clearQueue() {
    while(start != NULL) {
        queueNode *tmp = start->next;
        free(start); start = tmp;
    } end = NULL;
}

simProcess *popProcess() {
    if(start == NULL) return NULL;
    simProcess *popped = start->process;
    queueNode *toFree = start;
    start = start->next;
    free(toFree); return popped;
}

void simulate(simProcess *processArray, int numProcesses, int switchTime) {
    int time = 0;
    simProcess *runningProcess = NULL;
    while(1) {
        simProcess *nextToArrive = NULL;
        for(int i = 0; i < numProcesses; i++) {
            if((processArray+i)->processStatus == WAITING && (nextToArrive==NULL || 
              nextToArrive->interarivalTime > (processArray+i)->interarivalTime)) {
                nextToArrive = (processArray+i);
            } 
        } if(nextToArrive==NULL) break;    
        addProcess(nextToArrive); time = nextToArrive->interarivalTime;
        nextToArrive->processStatus = QUEUED;
        printf("time %dms: Process %s arrived; added to ready queue ", time, nextToArrive->processID);
        printQueue();
        if(runningProcess==NULL) {
            nextToArrive->processStatus = RUNNING_CPU; 
            runningProcess = nextToArrive;
            popProcess(time);
            printf("time %dms: Process %s started using the CPU for %dms burst ", 
                time+(int)(switchTime/2), nextToArrive->processID, *(nextToArrive->cpuIOTimes));
            printQueue();
        }
    }
}
