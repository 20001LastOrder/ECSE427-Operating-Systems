#ifndef RAM_DEF
#define RAM_DEF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// add a file to RAM, save the start and end index in the pointers
int addToRAM(FILE* file, int* start, int* end);

// free RAM space from start to end
void freeRAM(int startIndex, int endIndex);

// get string stored in ram
char* getRam(int addr);
#endif