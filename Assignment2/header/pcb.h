#ifndef PCB_DEF
#define PCB_DEF
#include <stdlib.h>

typedef struct PCB
{
    int PC;
    int start;
    int end;
} PCB;

typedef struct PCB PCB;
PCB* makePCB(int start, int end);

#endif