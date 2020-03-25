#include "pcb.h"

int nextAvailablePid = 0;

PCB* makePCB(){
    PCB* pcb = malloc(sizeof(PCB));

    if(pcb == NULL){
        return NULL;
    }

	pcb->pid = nextAvailablePid++;

    // initially no pages is loaded, so initiate everything to -1
    for(int i = 0; i < PAGE_TABLE_SIZE; i++){
        pcb->pageTable[i] = -1;
    }

    return pcb;
}

int getNextAvailablePid() {
	return nextAvailablePid;
}