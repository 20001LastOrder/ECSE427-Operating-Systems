#include "ram.h"

// forward declaration of methods inside ram.c
static int isSpace(char* buffer);

//simulated ram space (initiate whole space to null)
char** ram;
int ramSize = 0;

int addToRAM(FILE* file, int* start, int* end){
    // find an empty spot
    int startIndex = 0;
    for(int i = 0; i < ramSize; i++){
        if(ram[i] == NULL){
            startIndex = i;
            break;
        }
    }

    //printf("StartINDEX: %d\n", startIndex);
    // if there is no space, return error
    if(startIndex >= ramSize){
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
        if(spot >= ramSize || ram[spot] != NULL){
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
    if(addr < ramSize){
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

// initialize ram with specific size
void initializeRAM() {
    int size = FRAME_SIZE * LINES_PER_PAGE;
	ram = calloc(size, sizeof(char*));
	ramSize = size;
}

int loadFrame(int frameId, char contents[LINES_PER_PAGE][999], int size){
    int startPos = frameId * LINES_PER_PAGE;
    if(size > LINES_PER_PAGE){
        printf("ERROR: Exceed maximum frame size");
        return 1;
    }else if(ram[startPos] != NULL){
        printf("Error: Frame is in use");
        return 1;
    }

    //load the frame
    for(int i = 0; i < size; i++){
        ram[startPos + i] = strdup(contents[i]);
    }
    return 0;
}
