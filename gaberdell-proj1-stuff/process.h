#pragma once

enum boundBy {
	CPU_BOUND, IO_BOUND
};

enum status {
    WAITING, QUEUED, RUNNING_CPU, RUNNING_IO, TERMINATED
};

typedef struct {
	char* processID;
	enum boundBy processBound;
	short burstNum;
	int interarivalTime;
	int* cpuIOTimes;
    enum status processStatus;
} simProcess;
