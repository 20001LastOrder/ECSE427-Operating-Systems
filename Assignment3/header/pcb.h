#ifndef PCB_DEF
#define PCB_DEF
#include <stdlib.h>

// PCB struct
typedef struct PCB
{
	int pid;
    int PC;
    int start;
    int end;
} PCB;

// creat a PCB by specifying the start and end address of the program
PCB* makePCB(int start, int end);

// get next available pid
int getNextAvailablePid();
#endif