#ifndef INTERPRETER_DEF
#define INTERPRETER_DEF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shellmemory.h"
#include "memorymanager.h"

extern char message[];
int interpret(char* userInput);

#endif