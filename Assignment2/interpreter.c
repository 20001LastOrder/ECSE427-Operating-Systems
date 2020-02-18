#include "interpreter.h"
#include "shellmemory.h"

char message[200];

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
int parse(char* userInput);
void helpPrint(char* commandDesc[]);
int help(char* tokens[]);
int quit(char* tokens[]);
int set(char* tokens[]);
int print(char* tokens[]);
int run(char* tokens[]);
int exec(char* tokens[]);

// intepret the command and call the correspond methods
int intepret(char* tokens[]){
    //assume tokens[0] is the command
    //assume cmd args setting
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
		strcpy(message, "variable does not exist!");
		return 3;
	}else{
		printf("%s\n", result);
		return 0;
	}
}

// run: load and run a given script
int run(char* tokens[]){
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
	fgets(line, 999, p);
	while(!feof(p)){
		errorCode = parse(line);

		// quit commend, leave the script (does not leave the shell)
		if(errorCode == -1){
			errorCode = 0;
			break;
		}else if(errorCode != 0){
			fclose(p);
			printf("Syntax or Internal Error on Line %d of file <%s> Script stoped: \n", lineNumber, tokens[1]);
			return errorCode;
		}
		lineNumber += 1;
		fgets(line, 999, p);
	}

	fclose(p);
	return errorCode;
}

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

	// check if there is any duplicate string
	char dupMessage[100];
	if(tokens[2] != NULL && strcmp(tokens[1], tokens[2]) == 0){
		sprintf(dupMessage, "Script %s already loaded", tokens[2]);
		strcpy(message, dupMessage);
		return 5;
	}else if(tokens[3] != NULL && strcmp(tokens[1], tokens[3]) == 0){
		sprintf(dupMessage, "Script %s already loaded", tokens[3]);
		strcpy(message, dupMessage);
		return 5;
	}else if(tokens[3] != NULL && strcmp(tokens[2], tokens[3]) == 0){
		sprintf(dupMessage, "Script %s already loaded", tokens[3]);
		strcpy(message, dupMessage);
		return 5;
	}
	return 0;
}

void helpPrint(char* commandDesc[]){
	printf("%-20.20s  %-20.80s \n", commandDesc[0], commandDesc[1]);
}