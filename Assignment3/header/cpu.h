#ifndef CPU_DEF
#define CPU_DEF

#include <stdlib.h>
#include "ram.h"
#include "memorymanager.h"
#include <string.h>

// set the Instruction address of cpu to specific place
void setIP(int start, int offset);

// get the instruction register value
int getFrame();
int getOffset();

// run the current program for a maximum (quanta) lines of code
int run(int quanta);

// check if the CPU is currently available
int isCPUAvailabe();

#endif