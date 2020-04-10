#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "shell.h"
#include "pcb.h"
#include "ram.h"
#include "cpu.h"
#include "memorymanager.h"

// structs
typedef struct ReadyQueueNode{
    PCB* pcb;
    struct ReadyQueueNode* next;
} ReadyQueueNode;

typedef struct ReadyQueue{
    ReadyQueueNode* head;
    ReadyQueueNode* tail;
} ReadyQueue;

// create a ready queue with head and tail points to NULL
ReadyQueue readyQueue = {NULL, NULL};

// forward declaration of methods in kernel.c
void addToReady(PCB* pcb);
void freePendingPCBs();
void terminateHead();
void terminateTail();
void boot();
static int pageFault(PCB* pcb);
int pendingPCBs = 0;; // the number of pcbs not yet runs

int main(int argc, char** argv){
    //print welcoming message
	boot();
    printf("Kernel 1.0 loaded!\n");
    return shellUI();
}

//prepare the kernel
void boot() {
	// prepare the ram
	printf("Booting Kernel...\n");
	initializeRAM();

	//Check and delete Backstore
	char command[100];
	struct dirent* d;
	DIR* dir = opendir(BACK_STORE);
	if (dir != NULL) {
		//begin to count 
		int n = 0;
		while ((d = readdir(dir)) != NULL) {
			if (++n > 2) break;
		}

		if (n > 2) {
			// non-empty directory, print a warning message
			printf("WARNING: BackStore has files in it, deleting...\n");
		}
		
		// delete BackStore
		sprintf(command, "rm -r ./%s", BACK_STORE);
		system(command);
		printf("Back store deleted...\n");
	}
	else {
		printf("WARNING: BackStore Does not exist\n");
	}

	//create new BackStore
	sprintf(command, "mkdir ./%s", BACK_STORE);
	system(command);
	printf("new BackStore created...\n");
	printf("Kernel booted...\n");
}

// termination sequence
void terminate(){
    //remove all the files in the Backstore
    char command[100];
    sprintf(command, "rm ./%s/*", BACK_STORE);
    system(command);
    printf("Backstore files removed\n");
}

// load a program to ram and return the pcb
// error codes: -1: program does not found
// error codes: -2: RAM space is full
// error codes: -3: PCB creation failed 
PCB* myinit(FILE* file){
    //when any init error happens, terminate all the programs in RAM and return
    if(file == NULL){
        printf("ERROR: Program file not found\n");
		freePendingPCBs();
        return NULL;
    }
    
    // try make a PCB
    PCB* pcb = makePCB();
    if(pcb == NULL){
        printf("ERROR: PCB creation failed\n");
        freePendingPCBs();
		fclose(file);
        return NULL;
    }

    addToReady(pcb);
	pendingPCBs++;
    return pcb;
}

int isReadyQueueEmpty(){
    if(readyQueue.head == NULL){
        return 1;
    }else{
        return 0;
    }
}

int scheduler(){
    // check if the cpu is available
    // TODO: not implementation needed yet, since no threading
    // get the head of CPU
    if(readyQueue.head == NULL){
        return 0;
    }

	// starts running set pending pcbs to 0
	pendingPCBs = 0;

    ReadyQueueNode* head = readyQueue.head;
    PCB* pcb = head->pcb;
    int frameNumber = pcb->pageTable[pcb->pcPage];
    setIP(pcb->pageTable[pcb->pcPage] * LINES_PER_PAGE, pcb->pcOffset);
    
    // run script based on the position of PC
    int errorCode = 0;
    // only run the code if the page is in the ram
    if(isCPUAvailabe() && frameNumber != -1){
        errorCode = run(pcb->maxLine - pcb->pcPage * LINES_PER_PAGE + pcb->pcOffset);
    }else if(frameNumber == -1){
        // page fault before the cpu execution
        errorCode = pageFault(pcb);
        if(errorCode < 0){
            //page fault failed, abort
            return errorCode;
        }
    }else{
        // if the CPU is not available, return and let it try again.
        return errorCode;
    }

    // handle page switch
    if(errorCode == 100){  
        // load only if this is not the last page
        pcb->pcPage++;
        if(pcb->pcPage < pcb->maxPage && pcb->pageTable[pcb->pcPage] == -1){
            // page is not in the ram, load it from the disk
            errorCode = pageFault(pcb);
            if(errorCode < 0){
                //page fault failed, abort
                return errorCode;
            }
        }
        // reset offset
        pcb->pcOffset = 0;
    }else{
        // store offset
        pcb->pcOffset = getOffset();
    }

    // terminate program
    if(pcb->pcPage * LINES_PER_PAGE + pcb->pcOffset >= pcb->maxLine || errorCode < 0){
        terminateHead();
        return errorCode;
    }else{
        // put a program to the end of ready queue
        // set head to the next node
        if(head->next!=NULL){
            readyQueue.head = head->next;
        }

        if(readyQueue.tail != head){
            readyQueue.tail->next = head;
            readyQueue.tail = head;
        }
        head->next = NULL;
    }
    return errorCode;
}

static int pageFault(PCB* pcb){
    int errorCode = tryLoadPage(pcb, pcb->pcPage);
    return errorCode;
}

void addToReady(PCB* pcb){
    ReadyQueueNode* newNode = malloc(sizeof(ReadyQueueNode));
    newNode->pcb = pcb;
    newNode->next = NULL;

    // if it is the first every node
    if(readyQueue.head == NULL){
        readyQueue.head = newNode;
        readyQueue.tail = newNode;
    }else{
        ReadyQueueNode* previousTail = readyQueue.tail;
        previousTail->next = newNode;
        readyQueue.tail = newNode;
    }
}

// in the case of any load error, free all the PCBs created before 
// since no program will run
void freePendingPCBs(){
    for(int i = 0; i < pendingPCBs; i++){
		terminateTail();
	}
	pendingPCBs = 0;
}

void terminateHead(){
    ReadyQueueNode* head = readyQueue.head;
    if(head == NULL){
        return;
    }
    readyQueue.head = head->next;
    if(readyQueue.tail == head){
        readyQueue.tail = NULL;
    }

    head->next = NULL;
    // free ram space
    for(int i = 0; i < PAGE_TABLE_SIZE; i++){
        if(head->pcb->pageTable[i] != -1){
            freeRAM(head->pcb->pageTable[i]);
        }
    }
    free(head->pcb);
    free(head);
}

void terminateTail() {
	ReadyQueueNode* tail = readyQueue.tail;
	if (tail == NULL) {
		return;
	}
	ReadyQueueNode* pointer = readyQueue.head;
	if (pointer == NULL) {
		return;
	}
	if (pointer != tail) {
		// find the second last node
		while (pointer->next != tail) {
			pointer = pointer->next;
		}

		//set second last to tail
		readyQueue.tail = pointer;
		pointer->next = NULL;

	}else {
		// tail is header
		readyQueue.head = NULL;
		readyQueue.tail = NULL;
	}

	//free ram and tail
    for(int i = 0; i < PAGE_TABLE_SIZE; i++){
        if(tail->pcb->pageTable[i] != -1){
            freeRAM(tail->pcb->pageTable[i]);
        }
    }
	free(tail->pcb);
	free(tail);
}
