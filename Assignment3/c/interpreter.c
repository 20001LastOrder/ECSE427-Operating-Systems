#include "interpreter.h"

char message[200];

int isInExec = 0;

// forward declaration of parse function in shell
int parse(char userInput[], char** words, int wordsSize);

// commands definitions
char* commands[][2] = {
	{"help", "Display all the commands"},
	{"quit", "Exits / terminates the shell with “Bye!”"},
	{"set", "Assigns a value to shell memory, format: set VAR VALUE"},
	{"print", "Displays the STRING assigned to VAR, format: print VAR"},
	{"run", "Executes the file, format: run FILE.TXT"},
	{"exec", "Executes concurrent programs, format: exec p1.txt [p2.txt] [p3.txt]"}
};

// total number of commands
int commandsCount = 6;

// forward declaration of functions
void helpPrint(char* commandDesc[]);
int help(char* tokens[]);
int quit(char* tokens[]);
int set(char* tokens[]);
int print(char* tokens[]);
static int run(char* tokens[]);
int exec(char* tokens[]);
// declaration from kernel.c
int myinit(char* filename);
int isReadyQueueEmpty();
int scheduler();
void handleErrorCode(int errorCode);

// intepret the command and call the correspond methods
int interpret(char* userInput){
    //assume tokens[0] is the command
    //assume cmd args setting
	char* tokens[200] = {NULL};

	// parse the userInput (0 means there is not input rather than spaces)
	if(parse(userInput, tokens, 200) == 0){
		return 0;
	}

	int errorCode = 0;	
    //handle other cmd
    if(strcmp(tokens[0], commands[0][0]) == 0){
    	errorCode = help(tokens);
    }else if(strcmp(tokens[0], commands[1][0]) == 0){
    	errorCode = quit(tokens);
    }else if(strcmp(tokens[0], commands[2][0]) == 0){
    	errorCode = set(tokens);
    }else if(strcmp(tokens[0], commands[3][0]) == 0){
		errorCode = print(tokens);
	}else if(strcmp(tokens[0], commands[4][0]) == 0){
		errorCode = run(tokens);
	}else if(strcmp(tokens[0], commands[5][0]) == 0){
		errorCode = exec(tokens);
	}else{
    	errorCode = 1;
    }
    return errorCode;
}

// help: get all the available commands
int help(char* tokens[]){
	// if there are any arguments, output an error message
	if(tokens[1] != NULL){
		//free the original pointer
		strcpy(message, "help should not have any arguments");
		return 2;
	}

	char* header[] = {"COMMAND", "DESCRIPTION"};
	helpPrint(header);
	for(int i = 0; i < commandsCount; i++){
		helpPrint(commands[i]);
	}
	return 0;
}

// quit: quit the kernel
int quit(char* tokens[]){
	printf("Bye!\n");
	return -1;
}

// set: set a variable
int set(char* tokens[]){
	if(tokens[1] == NULL || tokens[2] == NULL){
		// too few args error
		strcpy(message, "too few args, set takes exactly 2 arguments: VAR VALUE");
		return 2;
	} else if(tokens[3] != NULL){
		// too many args error
		strcpy(message, "too many args, set takes exactly 2 arguments: VAR VALUE");
		return 2;
	}

	if(memAdd(tokens[1], tokens[2])){
		return 0;
	}else{
		strcpy(message, "variable adding failed");
		return 3;
	}
}

// print: print the value of a variable
int print(char* tokens[]){
	if(tokens[1] == NULL){
		// too few args error
		strcpy(message, "too few args, print takes exactly 1 arguments: VAR");
		return 2;
	}else if(tokens[2] != NULL){
		// too many args error
		strcpy(message, "too many args, print takes exactly 1 arguments: VAR");
		return 2;
	}

	char* result = memFind(tokens[1]);
	if(result == NULL){
		//30: variable does not exist in memory
		char buffer[100];
		sprintf(buffer, "%s variable does not exist in memory!", tokens[1]);
		strcpy(message, buffer);
		return 3;
	}else{
		printf("%s\n", result);
		return 0;
	}
}

// run: load and run a given script
static int run(char* tokens[]){
	if(tokens[1] == NULL){
		// too few args error
		strcpy(message, "too few args, run takes exactly one argument: FILE.TXT");
		return 2;
	}else if(tokens[2] != NULL){
		// too many args error
		strcpy(message, "too many args, run takes exactly one argument: FILE.TXT");
		return 2;
	}


	int errorCode = 0;
	char line[1000];
	FILE* p = fopen(tokens[1], "r");

	//file does not exist
	if(p == NULL){
		strcpy(message, "Script not found");
		return 4;
	}

	int lineNumber = 1;
	while(!feof(p)){
		fgets(line, 999, p);
		errorCode = interpret(line);

		// quit commend, leave the script (does not leave the shell)
		if(errorCode == -1){
			errorCode = 0;
			break;
		}else if(errorCode < 0){
			fclose(p);
			printf("Syntax or Internal Error on Line %d of file <%s> Script stoped: \n", lineNumber, tokens[1]);
			return errorCode;
		}else if(errorCode != 0){
			handleErrorCode(errorCode);
		}

		lineNumber += 1;
	}

	fclose(p);
	return errorCode;
}

//deprecated: used to still run file when duplicates
/*static int hasDuplicate(char* programs[], int numPrograms, char* program) {
	for (int i = 0; i < numPrograms; i++) {
		if (strcmp(programs[i], program) == 0) {
			return 1;
		}
	}

	return 0;
}*/

// exec: exec 1 to 3 scripts concurrently
int exec(char* tokens[]){
	// check for the number of arguments
	if(tokens[1] == NULL){
		// too few args error
		strcpy(message, "too few args, exec takes 1 to 3 arguments p1 p2 p3");
		return 2;
	}else if(tokens[4]!=NULL){
		// too many args error
		strcpy(message, "too many args, exec takes 1 to 3 arguments p1 p2 p3");
		return 2;
	}

	// begin load fields
	int i = 1;
	//store program and if we are already in a exec call
	while(tokens[i]!=NULL){
		// deprecated: used to still run file when duplicates
/*		if (hasDuplicate(loadedPrograms, numLoadedPrograms, tokens[i])) {
			// already loaded program, ignore (not an error)
			printf("WARNING: program: %s is already loaded, skipping...\n", tokens[i]);
			i++;
			continue;
		}*/

		// check if there is any duplicate fiLename
		// char dupMessage[100];
		// if (tokens[2] != NULL && strcmp(tokens[1], tokens[2]) == 0) {
		// 	sprintf(dupMessage, "Script %s already loaded", tokens[2]);
		// 	strcpy(message, dupMessage);
		// 	return 5;
		// }
		// else if (tokens[3] != NULL && strcmp(tokens[1], tokens[3]) == 0) {
		// 	sprintf(dupMessage, "Script %s already loaded", tokens[3]);
		// 	strcpy(message, dupMessage);
		// 	return 5;
		// }
		// else if (tokens[3] != NULL && strcmp(tokens[2], tokens[3]) == 0) {
		// 	sprintf(dupMessage, "Script %s already loaded", tokens[3]);
		// 	strcpy(message, dupMessage);
		// 	return 5;
		// }

		//int errorCode = myinit(tokens[i]);
		FILE* file = fopen(tokens[i], "rt");
		int errorCode = launcher(file);
		if(errorCode < 0){
			char errorMessage[100];
			sprintf(errorMessage, "Program launch error: %s", tokens[i]);
			strcpy(message, errorMessage);
			return 5;
		}
		i+=1;
	}
	
	if (!isInExec) {
		isInExec = 1;
		while (isReadyQueueEmpty() != 1) {
			int errorCode = scheduler();
			if (errorCode != 0) {
				handleErrorCode(errorCode);
			}
		}

		isInExec = 0;
	}
	return 0;
}

void helpPrint(char* commandDesc[]){
	printf("%-20.20s  %-20.80s \n", commandDesc[0], commandDesc[1]);
}