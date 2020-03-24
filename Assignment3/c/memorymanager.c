#include "memorymanager.h"

static int isEmpty(char c);
static int countPage(FILE* file);

int launcher(FILE* p) {
	if (p == NULL) {
		// file pointer is null
		return 0;
	}

	int pid = getNextAvailablePid();

	// create a file name same as the pid
	char buffer[999];
	sprintf(buffer, "Backstore/%d", pid);
	FILE* virtualMem = fopen(buffer, "wr");
	if (virtualMem == NULL) {
		// file open failed
		return 0;
	}

	//copy lines of original file to the new file
	while (fgets(buffer, 999, p) != NULL) {
		char* pointer = buffer;
		while (isEmpty(*pointer)) {
			pointer++;
		}

		if (*pointer == '\n' || *pointer == '\0') {
			// empty line, ignore
			continue;
		}
		// if there is no new line character at the end, add it to the end
		int length = strlen(pointer);
		if(*(pointer + length - 1) != '\n') strcat(pointer, "\n");
		// copy the content to the new file
		fprintf(virtualMem, pointer);
	}
	//close the original pointer
	fclose(p);

	//close and reopen the new file pointer
	fclose(virtualMem);
	sprintf(buffer, "Backstore/%d", pid);
	virtualMem = fopen(buffer, "rt");
	int pages = countPage(virtualMem);
	loadPage(0, virtualMem, 0);
	fclose(virtualMem);
	p = NULL;
	return 0;
}

int loadPage(int pageNumber, FILE* file, int frameNumber){
	// put stream to the correct position
	fseek(file, pageNumber * LINES_PER_PAGE, SEEK_SET);
	int size = 0;
	char buffer[999];
	char contents[LINES_PER_PAGE][999];
	// get the next four lines of code
	while(fgets(buffer, 999, file) != NULL && size <= 4){
		strcpy(contents[size], buffer);
		size ++;
	}

	if(size == 0){
		printf("Error: page number exceed maximum page size");
		return 1;
	}

	return loadFrame(frameNumber, contents, size);
}

int findFrame(){
	return nextAvailableFrame();
}

static int countPage(FILE* file){
	// reset to the initial position
	fseek(file, 0, SEEK_SET);
	char c;
	int lines;
	// the file in backstore will always end with a new line
	while((c = fgetc(file)) != EOF){
		if(c == '\n') lines++; 
	}

	int pages = lines / LINES_PER_PAGE;
	if(lines % LINES_PER_PAGE > 0){
		pages ++;
	}
	//reset file pointer back
	fseek(file, 0, SEEK_SET);

	return pages;
}

static int isEmpty(char c) {
	return c == ' ' || c == '\t';
}