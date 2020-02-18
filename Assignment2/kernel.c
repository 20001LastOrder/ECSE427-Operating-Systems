#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "pcb.h"

typedef struct Node Node;
typedef struct LinkedList LinkedList;

LinkedList readyQueue;

int main(int argc, char** argv){
    //print welcoming message
    printf("Kernel 1.0 loaded!\n");
    shellUI();
}

typedef struct LinkedList{
    Node* head;
    Node* tail;
} LinkedList;

typedef struct Node{
    PCB* pcb;
    Node* next;
} Node;

