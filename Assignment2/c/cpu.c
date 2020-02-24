#include "cpu.h"


typedef struct CPU{
    int IP;
    char IR[1000];
    int quanta;
} CPU;

CPU cpu = {.quanta = 2};
int interpret(char* userInput);

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
    int q = quanta <= cpu.quanta? quanta : cpu.quanta;
    for(int i = 0; i < q; i++){
        strcpy(cpu.IR, getRam(cpu.IP));
        int errorCode = interpret(cpu.IR);
        cpu.IP++;
        if(errorCode != 0){
            return errorCode;
        }
    }
    return 0;
}