#ifndef PCB_DEF
#define PCB_DEF
#include <stdlib.h>
#define PAGE_TABLE_SIZE 10
// PCB struct
typedef struct PCB
{
	int pid;
    int PC;
    int start;
    int end;
    int pageTable[PAGE_TABLE_SIZE];
    int pcPage;
    int pcOffset;
    int maxPage;
    int maxLine;
} PCB;

// creat a PCB by specifying the start and end address of the program
PCB* makePCB();

// get next available pid
int getNextAvailablePid();
#endif