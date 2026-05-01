#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "process.h"

typedef struct Node {
    simProcess process;
    Node *next;
} queueNode
