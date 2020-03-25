#include "cpu.h"
#define DEFAULT_QUNATA 2

// CPU Struct
typedef struct CPU{
    int IP;
    int offset;
    char IR[1000];
    int quanta;
} CPU;

// Create a cpu with the default quanta size to DEFAULT_QUANTA
CPU cpu = {.quanta = DEFAULT_QUNATA};
int cpuAvailable = 1;

// function deficnition in interpreter.c
extern int interpret(char* userInput);

// point the cpu to current address
void setIP(int start, int offset){
    cpu.IP = start;
    cpu.offset = offset;
}

int getFrame(){
    return cpu.IP;
}

int getOffset(){
    return cpu.offset;
}
// run a program for quanta
int run(int quanta){
    // find the smaller one of user provided and default quanta
    cpuAvailable = 0;
    int q = quanta <= cpu.quanta? quanta : cpu.quanta;
    for(int i = 0; i < q; i++){
        //if offset is 4, interrupt
        if(cpu.offset == 4){
            break;
        }

        strcpy(cpu.IR, getRam(cpu.IP + cpu.offset));
		int errorCode = interpret(cpu.IR);
        cpu.offset++;
        if(errorCode != 0){
            cpuAvailable = 1;
            return errorCode;
        }
    }

    cpuAvailable = 1;

    // singnal page fault
    if(cpu.offset == 4){
        return 100;
    }

    return 0;
}

int isCPUAvailabe(){
    return cpuAvailable;
}