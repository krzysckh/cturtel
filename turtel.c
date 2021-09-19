#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define turtel

#define VAR_MAX 100
#define LINE_LEN_MAX 1024

typedef int bool;
#define true 1
#define false 0

#define DEBUG true

#include "stdturtel.h"

typedef struct TurtelString {
	int len;
	char content[];
} TurtelString;

typedef struct TurtelNum {
	int content;
} TurtelNum;

typedef struct TurtelBool {
	bool content;
} TurtelBool;


int main (int argc, char *argv[]) {
	FILE *inpt = stdin, *outpt = stdout;

	TurtelString StringInfo[VAR_MAX];
	TurtelNum NumInfo[VAR_MAX];
	TurtelBool BoolInfo[VAR_MAX];

	/*
	int l = 5;
	StringInfo[0].len = l;
	char tempstr[l+1] = "bruh";
	int i = 0;
	for (i = 0; i < StringInfo[0].len; i++) {
		StringInfo[0].content[i] = tempstr[i];
	}
	printf("%s\n", StringInfo[0].content);
	*/
	
	char *full = malloc(sizeof(char)*10); /* initializing char pointer. full code will be stored here */

	if (DEBUG) printf("DEBUG: initialized *full with size of %ld\n", sizeof(full));

	char line[LINE_LEN_MAX]; /* initializing linebuffer. it's a temporary buffer for lines */
	
	while (!feof(inpt)) {
		if (DEBUG) printf("DEBUG: got into while loop -_-\n");

		fgets(line, LINE_LEN_MAX, inpt); 
		char *tmp = malloc(sizeof(full));
		strcpy(tmp, full);
		free(full);
		full = malloc(sizeof(tmp) + sizeof(char)*strlen(line));
		strcpy(full, tmp);
		strcat(full, line);
		free(tmp);

		/* 0. input string from file to linebuffer */
		/* 1. create temporary char pointer with a size of full (char ptr) */
		/* 2. copy full to tmp, so it can be later resized */
		/* 3. freeing memory of full, so it's not goin' anywhere B) */
		/* 4. allocating memory for full with size of tmp (itself a while ago) + line string size */
		/* 5. full = tmp (full = full) */
		/* 6. full += line */
		/* 7. freeing tmp so it's not wasted */

		if (DEBUG) printf("DEBUG: full is: \"%s\"", full);
	}

	/* ↑ this loop stored whole code to *full, and now comes the hard part */


	return 0;
}
