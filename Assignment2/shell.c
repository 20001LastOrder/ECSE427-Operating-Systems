#include "shell.h"


char prompt[100]= {'$', '\0'};
int parse(char* userInput);
void handleErrorCode(int errorCode);

int shellUI(){
    // print welcome message
    printf("Welcome to the Percy shell\n");
    printf("Version 2.0 Updated Febrary 2020\n");

    // currently infinite loop for the looping
    while(1){
        printf("%s", prompt);
        char userInput[1000];
        fgets(userInput, 999, stdin);
        int errorCode = parse(userInput);

        //check break flag
        if(errorCode == -1) break;
        handleErrorCode(errorCode);
    }
    return 0;
}

void handleErrorCode(int errorCode){
    switch(errorCode){
            case 1:
                printf("%s\n", "CMD DOES NOT EXIST: use 'help' for the list of commends");
                break;
            case 2:
                printf("%s: %s\n", "ARGUMENT ERROR: ", message);
                break;
            case 3:
                printf("%s: %s\n", "SHELL MEMORY ERROR: ", message);
                break;
            case 4:
                printf("%s: %s\n", "IO ERROR: ", message);
                break;
            case 5:
                printf("%s: %s\n", "KERNEL ERROR", message);
        }
}

int parse(char* userInput){
    //initialize the array of words to NULL
    char* words[200] = {NULL};

    char* pointer = userInput;
    //skip white space
    while(*pointer == ' ' || *pointer == '\t');


    // loop through the input to break it down to tokens
    int wordsCount = 0;
    while(1){
        char tmp[200];
        int i = 0;
        while(*pointer!=' ' && *pointer != '\t' && *pointer != '\0' && *pointer != '\n' && *pointer != '\r'){
             tmp[i] = *pointer;
             pointer += 1;
             i+= 1;
        }

        //add the end string flag and duplicate the word if there is any word
        if(i > 0){
            tmp[i] = '\0';
            words[wordsCount] = strdup(tmp);
            wordsCount += 1;
        }

        //break if reaches the end of the string
        if(*pointer == '\0'){
            break;
        }
        pointer += 1;
    }

    if(wordsCount == 0) return 0;

    return intepret(words);
}