
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
