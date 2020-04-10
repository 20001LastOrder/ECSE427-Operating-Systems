#include "memorymanager.h"

static int isEmpty(char c);
static int countPage(FILE* file);

// forward declaration for function in kernel.c
PCB* myinit(FILE* file);
void freePendingPCBs();

int launcher(FILE* p) {
	PCB* pcb = myinit(p);
	if(pcb == NULL){
		// create PCB failed
		return 0;
	}


	// create a file name same as the pid
	char buffer[999];
	sprintf(buffer, "%s/%d", BACK_STORE, pcb->pid);
	FILE* virtualMem = fopen(buffer, "wr");
	if (virtualMem == NULL) {
		// file open failed
		return 0;
	}

	//copy lines of original file to the new file
	int lineCount = 0;
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
		lineCount ++;
		// copy the content to the new file
		fprintf(virtualMem, "%s", pointer);
	}
	//close the original pointer
	fclose(p);
	p = NULL;

	//close and reopen the new file pointer in order to let system write to the disk
	fclose(virtualMem);
	sprintf(buffer, "%s/%d", BACK_STORE, pcb->pid);
	virtualMem = fopen(buffer, "rt");

	// initialize the pcb
	int pages = countPage(virtualMem);
	if(pages > FRAME_SIZE){
		printf("program exceed the maximum number of lines supported\n");
		freePendingPCBs();
		return 0;
	}

	pcb->maxPage = pages;
	pcb->maxLine = lineCount;
	pcb->pcPage = 0;
	pcb->pcOffset = 0;
	fclose(virtualMem);

	// load the first two pages to the ram
	int count = 0;
	int errorCode = 0;
	while(count < pages && count < DEFAULT_PAGES){
		errorCode += tryLoadPage(pcb, count);
		count += 1;
	}	

	if(errorCode < 0){
		return 0;
	}else{
		return 1;
	}
}

int loadPage(int pageNumber, FILE* file, int frameNumber){
	// put stream to the correct line position
	int lineNumber = 0;
	char buffer[999];
	while(lineNumber < pageNumber * LINES_PER_PAGE){
		fgets(buffer, 999, file);
		lineNumber++;
	}

	int size = 0;
	char contents[LINES_PER_PAGE][999];
	// get the next four lines of code
	while(fgets(buffer, 999, file) != NULL && size < 4){
		strcpy(contents[size], buffer);
		size ++;
	}

	if(size == 0){
		printf("Error: page number exceed maximum page size\n");
		return 1;
	}

	return loadFrame(frameNumber, contents, size);
}

int findFrame(){
	return nextAvailableFrame();
}

int findVictim(PCB *p){
	// first choose a random number
	// first generate a random number 
	int attemptFrame = (int)(((double) rand() / (double) RAND_MAX) * FRAME_SIZE);
	int count = 0;

	// if the attemptFrame belongs the current frame, try to find the next one until it cycles to the original position
	// or find a valid victim
	while(getPcbByFrameId(attemptFrame) == p && count < FRAME_SIZE){
		count ++;
		attemptFrame = (attemptFrame + 1) % FRAME_SIZE;
	}

	if(count == FRAME_SIZE){
		// cycles back to the original position, abort
		printf("ERROR: cannot find a valid victim\n");
		return -1;
	}

	// reset the page table
	PCB* victim = getPcbByFrameId(attemptFrame);
	for(int i = 0; i < PAGE_TABLE_SIZE; i++){
		if(victim->pageTable[i] == attemptFrame){
			victim->pageTable[i] = -1;
			break;
		}
	}
	return attemptFrame;
}

int updatePageTable(PCB* p, int pageNumber, int frameNumber, int victimFrame){
	int frameId = frameNumber != -1? frameNumber : victimFrame;
	// open the file in the backing store
	char buffer[100];
	sprintf(buffer, "%s/%d", BACK_STORE, p->pid);
	FILE* f = fopen(buffer, "rt");
	if(f == NULL){
		printf("ERROR: Cannot find the file in Back Store\n");
		return -1;
	}

	int errorCode = loadPage(pageNumber, f, frameId);
	if(errorCode < 0){
		// load page error
		return errorCode;
	}
	// load page success, update page table
	registerPCB(frameId, p);
	p->pageTable[pageNumber] = frameId;

	//close file pointer
	fclose(f);
	return 0;
}

int tryLoadPage(PCB* pcb, int pageNumber){
	if(pageNumber >= pcb->maxPage){
		printf("ERROR: Exceed the maximum page for the program\n");
		return -1;
	}

	int frameNumber = findFrame();
	int victimNumber = -1;
	if(frameNumber == -1){
		victimNumber = findVictim(pcb);
		if(victimNumber == -1){
			// Cannot find a page that is not belong to this process, abort
			return -1;
		}
	}

	return updatePageTable(pcb, pageNumber, frameNumber, victimNumber);
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