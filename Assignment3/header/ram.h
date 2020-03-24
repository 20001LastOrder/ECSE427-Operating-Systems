#ifndef RAM_DEF
#define RAM_DEF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FRAME_SIZE 10
#define LINES_PER_PAGE 4

// add a file to RAM, save the start and end index in the pointers
int addToRAM(FILE* file, int* start, int* end);

// free RAM space from start to end
void freeRAM(int startIndex, int endIndex);

// get string stored in ram
char* getRam(int addr);

// initialize ram with specific size
void initializeRAM();

//load a frame to the ram, the size must be smaller than or equalt to LINES_PER_PAGE 
int loadFrame(int frameId, char contents[LINES_PER_PAGE][999], int size);
#endif