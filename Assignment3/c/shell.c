#include "shell.h"


char prompt[100]= {'$', '\0'};
int parse(char userInput[], char** words, int wordsSize);
void handleErrorCode(int errorCode);

// forward declaration of function in kernel.c
void terminate();

int shellUI(){
    // print welcome message
    printf("Welcome to the Percy shell\n");
    printf("Version 2.0 Updated February 2020\n");
    printf("%s", prompt);
    char userInput[1000];

	// currently infinite loop for the looping
    while(1){
        if(fgets(userInput, 999, stdin) != NULL){
            int errorCode = interpret(userInput);
            //check break flag
            if(errorCode == -1) break;
            handleErrorCode(errorCode);
        
            // print promo only when the input is valid
            printf("%s", prompt);
        }else{
            // file input in finished, redirect back to normal stdin
            freopen("/dev/tty","r",stdin);
        }
    }

    terminate();
    return 0;
}

void handleErrorCode(int errorCode){
    switch(errorCode){
            case 1:
                printf("%s\n", "CMD DOES NOT EXIST: use 'help' for the list of commends");
                break;
            case 2:
                printf("%s: %s\n", "ARGUMENT ERROR", message);
                break;
            case 3:
                printf("%s: %s\n", "SHELL MEMORY ERROR", message);
                break;
            case 4:
                printf("%s: %s\n", "IO ERROR", message);
                break;
            case 5:
                printf("%s: %s\n", "KERNEL ERROR", message);
        }
}

static int isEndLine(char c) {
	return c == '\0' || c == '\n' || c == '\r';
}

static int isEndWord(char c) {
	return c == ' ' || c == '\t';
}

// parse the user input and store the results into words
int parse(char userInput[], char* words[], int wordsSize){
    //initialize the array of words to NULL
    // char* words[200] = {NULL};

    char* pointer = userInput;
    //skip white space
    while(isEndWord(*pointer)){
        pointer ++;
    }

    // loop through the input to break it down to tokens
    int wordsCount = 0;
	
	// used to support multiple words ("multiple words")
	// default we check for the end of word
	int checkEndWord = 1;

    while(wordsCount < wordsSize){
        char tmp[200];
        int i = 0;
        while((!checkEndWord || !isEndWord(*pointer)) && !isEndLine(*pointer)){
			//check for escape character
			if (*pointer == '\\' && *(pointer +1 ) == '"') {
				//skip escape character
				pointer++;
			} else if (*pointer == '"') {
				// " character along (not escaped) reset the check for end word
				checkEndWord = !checkEndWord;
				pointer++;
				continue;
			}
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

    return 1;
}