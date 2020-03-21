#include "shellmemory.h"

typedef struct MEM{
	char* var;
	char* value;
} MEM;

int memoryLength = 0;
int memorySize = 0;
MEM* memory;

int memExistingAdd(char* var, char* value){
	for(int i = 0; i < memorySize; i++){
		// find the variable with the same name
		if(strcmp(var, memory[i].var) == 0){
			//free old memory space
			free(memory[i].value);
			//assign new string to value, and return 1 to indicate that adding is successful
			memory[i].value = strdup(value);
			return 1;
		}
	}
	// the variable name does not exist in the mem
	return 0;
}

int memNewAdd(char* var, char* value){
	//check if memeorySize is going to exceed memoryLength
	if(memoryLength == 0){
		//reallocate memory for the array
		memory = malloc(2 * sizeof(MEM));
		memoryLength = 2;
	}else if(memorySize + 1 > memoryLength){
		//reallocate memory for the array
		MEM* newPointer = realloc(memory, 2 * memoryLength * sizeof(MEM));

		// realloc failed
		if(newPointer == NULL){
			return 0;
		}
		memory = newPointer;
		memoryLength *= 2;
	}

	//pointer to a empty spot of the shell memory
	MEM* spot = memory + memorySize;
	spot->var = strdup(var);
	spot->value = strdup(value);
	memorySize += 1;
	return 1;
}

int memAdd(char* var, char* value){
	if(memExistingAdd(var, value) || memNewAdd(var, value)){
		return 1;
	}

	//add failed
	return 0;
}

// if the return value is NULL indicates that finding is failed
char* memFind(char* var){
	char* value = NULL;
	for(int i = 0; i < memorySize; i++){
		// find the variable with the same name
		if(strcmp(var, memory[i].var) == 0){
			//duplicate the string
			value = strdup(memory[i].value);
			break;
		}
	}

	return value;
}
