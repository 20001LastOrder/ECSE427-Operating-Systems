#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "pcb.h"
#include "ram.h"
#include "cpu.h"

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
void freeAllPCBs();
void terminateHead();

int main(int argc, char** argv){
    //print welcoming message
    printf("Kernel 1.0 loaded!\n");
    shellUI();
}

// load a program to ram
// error codes: -1: program does not found
// error codes: -2: RAM space is full
// error codes: -3: PCB creation failed 
int myinit(char* filename){
    int start = 0;
    int end = 0;
    FILE* file = fopen(filename, "rt");

    //when any init error happens, terminate all the programs in RAM and return
    if(file == NULL){
        freeAllPCBs();
        return -1;
    }
    
    // try add the program to RAM
    if(!addToRAM(file, &start, &end)){
        freeAllPCBs();
		fclose(file);
        return -2;
    }

    // try make a PCB
    PCB* pcb = makePCB(start, end);
    if(pcb == NULL){
        freeAllPCBs();
		fclose(file);
        return -3;
    }

    addToReady(pcb);
	fclose(file);
    return 0;
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
    // TODO: not implemention needed yet, since no threading
    // get the head of CPU
    if(readyQueue.head == NULL){
        return 0;
    }

    ReadyQueueNode* head = readyQueue.head;
    setIR(head->pcb->PC);
    
    // run script based on the position of PC
    int errorCode = 0;
    if(isCPUAvailabe()){
        errorCode = run(head->pcb->end - head->pcb->PC + 1);
    }else{
        // if the CPU is not availale, return and let it try again.
        return errorCode;
    }

    int currentLine = getIR();
    // terminate program
    if(currentLine > head->pcb->end || errorCode < 0){
        terminateHead();
        return errorCode;
    }else{
        // put a program to the end of ready queue
        head->pcb->PC = currentLine;
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
void freeAllPCBs(){
    while(readyQueue.head != NULL){
        terminateHead();
    }
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
    freeRAM(head->pcb->start, head->pcb->end);
    free(head->pcb);
    free(head);
}
