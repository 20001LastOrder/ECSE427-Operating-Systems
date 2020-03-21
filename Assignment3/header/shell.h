#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"

int shellUI();
int parse(char userInput[], char* words[], int wordsSize);
void handleErrorCode(int errorCode);