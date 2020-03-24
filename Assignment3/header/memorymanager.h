#ifndef MEM_MANAGER_DEF
#define MEM_MANAGER_DEF
#include <string.h>
#include <stdio.h>
#include "pcb.h"
#include "ram.h"
/*
 * file contains interface regarding memory management
**/

// create memory space for a program
int launcher(FILE* p);

//load a page from disk to ram
int loadPage(int pageNumber, FILE* file, int frameNumber);


#endif // !MEM_MANAGER


