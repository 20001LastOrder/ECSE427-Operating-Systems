#include "pcb.h"

PCB* makePCB(int start, int end){
    PCB* pcb = malloc(sizeof(PCB));

    if(pcb == NULL){
        return NULL;
    }
    pcb->start = start;
    pcb->PC = start;
    pcb->end = end;
    return pcb;
}