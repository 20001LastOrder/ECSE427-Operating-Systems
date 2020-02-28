#include "cpu.h"
#define DEFAULT_QUNATA 2

// CPU Struct
typedef struct CPU{
    int IP;
    char IR[1000];
    int quanta;
} CPU;

// Create a cpu with the default quanta size to DEFAULT_QUANTA
CPU cpu = {.quanta = DEFAULT_QUNATA};
int cpuAvailable = 1;

// function deficnition in interpreter.c
extern int interpret(char* userInput);

// point the cpu to current address
void setIR(int address){
    cpu.IP = address;
}

int getIR(){
    return cpu.IP;
}

// run a program for quanta
int run(int quanta){
    // find the smaller one of user provided and default quanta
    cpuAvailable = 0;
    int q = quanta <= cpu.quanta? quanta : cpu.quanta;
    for(int i = 0; i < q; i++){
        strcpy(cpu.IR, getRam(cpu.IP));
		int errorCode = interpret(cpu.IR);
        cpu.IP++;
        if(errorCode != 0){
            cpuAvailable = 1;
            return errorCode;
        }
    }
    cpuAvailable = 1;
    return 0;
}

int isCPUAvailabe(){
    return cpuAvailable;
}