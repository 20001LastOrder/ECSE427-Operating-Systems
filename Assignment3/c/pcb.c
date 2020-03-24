#include "pcb.h"

int nextAvailablePid = 0;

PCB* makePCB(int start, int end){
    PCB* pcb = malloc(sizeof(PCB));

    if(pcb == NULL){
        return NULL;
    }

	pcb->pid = nextAvailablePid++;
    pcb->start = start;
    pcb->PC = start;
    pcb->end = end;

    return pcb;
}

int getNextAvailablePid() {
	return nextAvailablePid;
}