#ifndef RAM_DEF
#define RAM_DEF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int addToRAM(FILE* file, int* start, int* end);
void freeRAM(int startIndex, int endIndex);
char* getRam(int addr);
#endif