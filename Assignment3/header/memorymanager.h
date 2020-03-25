#ifndef MEM_MANAGER_DEF
#define MEM_MANAGER_DEF
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"
#include "ram.h"
#define BACK_STORE "Backstore"
#define DEFAULT_PAGES 2

/*
 * file contains interface regarding memory management
**/

// create memory space for a program
int launcher(FILE* p);

//load a page from disk to ram
int loadPage(int pageNumber, FILE* file, int frameNumber);

//find the first available fram
int findFrame();

// ramdomly find a victim
int findVictim(PCB *p);

// update a page table
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);

// try load the next page for the program
int tryLoadPage(PCB* pcb, int pageNumber);

#endif // !MEM_MANAGER
