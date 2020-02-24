#include "ram.h"
#define MAX_RAM 1000
// ram space
char* ram [1000] = {NULL};

int addToRAM(FILE* file, int* start, int* end){
    // find an empty spot
    int startIndex = 0;
    for(int i = 0; i < MAX_RAM; i++){
        if(ram[i] == NULL){
            startIndex = i;
            break;
        }
    }

    // if there is no space, return error
    if(startIndex >= MAX_RAM){
        return 0;
    }

    char buffer[1000];
    int i = 0;
    fgets(buffer, 999, file);
    while(!feof(file)){
        int spot = startIndex + i ;
        if(spot >= MAX_RAM || ram[spot] != NULL){
            return 0;
        }

        ram[spot] = strdup(buffer);
        i+=1;
        fgets(buffer, 999, file);
    }

    int endIndex = startIndex + i - 1;
    *start = startIndex;
    *end = endIndex;
    return 1;
}

void freeRAM(int startIndex, int endIndex){
    for(int i = startIndex; i <= endIndex; i++){
        free(ram[i]);
        ram[i] = NULL;
    }
}

char* getRam(int addr){
    if(addr < MAX_RAM){
        return ram[addr];
    }else{
        return NULL;
    }
}