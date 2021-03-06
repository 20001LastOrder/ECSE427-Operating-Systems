#include "ram.h"
#define MAX_RAM 1000

//simulated ram space (initiate whole space to null)
char* ram [MAX_RAM] = {NULL};

// forward declaration of methods inside ram.c
static int isSpace(char* buffer);

int addToRAM(FILE* file, int* start, int* end){
    // find an empty spot
    int startIndex = 0;
    for(int i = 0; i < MAX_RAM; i++){
        if(ram[i] == NULL){
            startIndex = i;
            break;
        }
    }

    //printf("StartINDEX: %d\n", startIndex);
    // if there is no space, return error
    if(startIndex >= MAX_RAM){
        return 0;
    }
	
    char buffer[1000];
    int i = 0;
    while(!feof(file)){
        fgets(buffer, 999, file);
        if(isSpace(buffer) == 1){
            continue;
        }
        int spot = startIndex + i ;
        // file length exceeds RAM space, free previous space and return 0
        if(spot >= MAX_RAM || ram[spot] != NULL){
            //reset previously define space
            for(int  c = startIndex; c < spot; c++){
                free(ram[c]);
                ram[c] = NULL;
            }
            return 0;
        }

        ram[spot] = strdup(buffer);
        i+=1;
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

// check if a string only contains space characters
static int isSpace(char* buffer){
    char* pointer = buffer;
    while(*pointer != '\0'){
        char value = *pointer;
        if(value != '\n' && value != ' ' && value != '\r' && value != '\t'){
            return 0;
        }
        pointer++;
    }
    return 1;
}