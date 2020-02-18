#include "pcb.h"

typedef struct PCB
{
    int PC;
    int start;
    int end;
} PCB;


PCB* makePCB(int start, int end){
    PCB* pcb = malloc(sizeof(PCB));
    pcb->start = start;
    pcb->PC = start;
    pcb->end = end;
    return pcb;
}