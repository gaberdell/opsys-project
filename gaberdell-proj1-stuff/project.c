#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int CPU_BOUND_FACTOR = 6;
int IO_BOUND_FACTOR = 8;

enum boundBy {
	CPU_BOUND, IO_BOUND
};

typedef struct {
	char* processID;
	enum boundBy processBound;
	short burstNum;
	int interarivalTime;

	int* cpuIOTimes;

} simProcess;


char** generateNames(int amount) {
	char** returnCharArray = calloc(amount, sizeof(char*));

	for (int amountToRun = 0; amountToRun < amount; amountToRun++) {
		char charFirstChar = 'A' + (amountToRun/10);
		char charSecondChar = '0' + (amountToRun%10);

		*(returnCharArray+amountToRun) = calloc(3, sizeof(char));

		*(*(returnCharArray+amountToRun)) = charFirstChar;
		*(*(returnCharArray+amountToRun)+1) = charSecondChar;
	}
	
	return returnCharArray;
}


double next_exp(double lamb, int upperLimit) {
	double finalVal = upperLimit+1;


	while (finalVal > upperLimit) {
		double r = drand48();
		finalVal = -log(r)/lamb;
	}

	return finalVal;
}

void initialSetUpForSimArray(simProcess* simArray, char** idList, int simSize, int cpuAmount) {
	for (int i = 0; i < simSize; i++) {
		(simArray+i)->processID = *(idList+i);

		//Might have to retool later because maybe if the number of CPU exceeds amount something else is supposed to happen idk tho..
		(simArray+i)->processBound = i < (simSize - cpuAmount) ? IO_BOUND : CPU_BOUND;
	}
}

int main (int argc, char ** argv) {

	int amountOfProcessToSim = atoi(*(argv+1));

	if (amountOfProcessToSim == 0) {
		fprintf(stderr, "You put no thing to process in the thing bruh moji virus 16\n");
		return EXIT_FAILURE;
	}

	int numOfCPUBoundProcess = atoi(*(argv+2));
	int randomSeed = atoi(*(argv+3));
	double lambdaValue = atof(*(argv+4));
	int ifTimeItTakesIsLongerThanThisSkip = atoi(*(argv+5));

	if (ifTimeItTakesIsLongerThanThisSkip < 0) {
		fprintf(stderr, "Your max value is negative?! What you trying to do start world war 3 or something!?\n");
		return EXIT_FAILURE;
	}

	printf("<<< -- process set (n=%d) with %d CPU-bound process%s\n", amountOfProcessToSim, numOfCPUBoundProcess, numOfCPUBoundProcess > 1 ? "es" : "");
	printf("<<< -- seed=%d; lambda=%.6f; upper bound=%d\n", randomSeed, lambdaValue, ifTimeItTakesIsLongerThanThisSkip);
	
	srand48(randomSeed);

	char** listOfNames = generateNames(amountOfProcessToSim);

	simProcess* processToSimArray = malloc(amountOfProcessToSim * sizeof(simProcess));

	initialSetUpForSimArray(processToSimArray, listOfNames, amountOfProcessToSim, numOfCPUBoundProcess);

	for (int i = 0; i < amountOfProcessToSim; i++) {

		printf("\n");

		int initialTime = floor(next_exp(lambdaValue, ifTimeItTakesIsLongerThanThisSkip));

		int burstAmount = ceil(drand48() * 16);

		(processToSimArray+i)->burstNum = burstAmount;

		(processToSimArray+i)->cpuIOTimes = malloc((burstAmount*2 - 1) * sizeof(int));

		char* boundString = "";

		switch ((processToSimArray+i)->processBound) {
			case CPU_BOUND:
				boundString = "CPU";
			break;

			case IO_BOUND:
				boundString = "I/O";
			break;

			default:

			break;
		}

		printf("%s-bound process %s: arrival time %dms; %d CPU burst%s:\n", boundString,  (processToSimArray+i)->processID, initialTime, burstAmount, burstAmount > 1 ? "s" : "");

		for (int j = 0; j < burstAmount-1; j++) {
			int cpuBurstTime = ceil(next_exp(lambdaValue, ifTimeItTakesIsLongerThanThisSkip));

			int ioBurstTime = ceil(next_exp(lambdaValue, ifTimeItTakesIsLongerThanThisSkip));

			if ((processToSimArray+i)->processBound == CPU_BOUND) {
				cpuBurstTime *= CPU_BOUND_FACTOR;
			}
			else if ((processToSimArray+i)->processBound == IO_BOUND) {
				ioBurstTime *= IO_BOUND_FACTOR;
			}

			*((processToSimArray+i)->cpuIOTimes+2*j) = cpuBurstTime;
			*((processToSimArray+i)->cpuIOTimes+2*j+1) = ioBurstTime;


			printf("==> CPU burst %dms ==> I/O burst %dms\n", cpuBurstTime, ioBurstTime);
		}

		int lastCPUBurst = ceil(next_exp(lambdaValue, ifTimeItTakesIsLongerThanThisSkip));
		if ((processToSimArray+i)->processBound == CPU_BOUND) {
				lastCPUBurst *= CPU_BOUND_FACTOR;
		}

		*((processToSimArray+i)->cpuIOTimes+2*(burstAmount-1)) = lastCPUBurst;
		

		printf("==> CPU burst %dms\n", lastCPUBurst);
	}
	

	for (int i = 0; i < amountOfProcessToSim; i++) {
		free(*(listOfNames+i));
		free((processToSimArray+i)->cpuIOTimes);
	}

	free(processToSimArray);
	free(listOfNames);

	return EXIT_SUCCESS;
}
