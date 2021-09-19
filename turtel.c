#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define turtel

#define VAR_MAX 100
#define LINE_LEN_MAX 1024

typedef int bool;
#define true 1
#define false 0

#define DEBUG false

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
	
	char *full = (char*)malloc(10*(sizeof(char))); /* initializing char pointer. full code will be stored here */
	strcpy(full, "");
	char *tmp;

	/*if (DEBUG) { printf("DEBUG: initialized *full with size of %ld\n", sizeof(full)) ; }*/

	char line[LINE_LEN_MAX]; /* initializing linebuffer. it's a temporary buffer for lines */
	
	bool read = true;
	while (read) {
		printf("%c", '\0');
		/* program crashes without this line what the fuck */

		fgets(line, LINE_LEN_MAX, inpt); 

		tmp = malloc(sizeof(full));
		if (DEBUG) { printf("DEBUG: allocated for *tmp with sizeof(): %ld\n", sizeof(tmp)) ; }
		strcpy(tmp, full);
		if (DEBUG) { printf("DEBUG: copied from full* to tmp*\n") ; }
		free(full);
		if (DEBUG) { printf("DEBUG: freed full\n") ; }
		full = malloc(sizeof(tmp) + sizeof(strlen(line)));
		if (DEBUG) { printf("DEBUG: alocated for full*\n") ; }
		strcpy(full, tmp);
		if (DEBUG) { printf("DEBUG: copied tmp into full\n") ; }
		strcat(full, line);
		if (DEBUG) { printf("DEBUG: added line to full\n") ; }
		free(tmp);
		if (DEBUG) { printf("DEBUG: freed tmp\n") ; }

		/* 0. input string from file to linebuffer */
		/* 1. create temporary char pointer with a size of full (char ptr) */
		/* 2. copy full to tmp, so it can be later resized */
		/* 3. freeing memory of full, so it's not goin' anywhere B) */
		/* 4. allocating memory for full with size of tmp (itself a while ago) + line string size */
		/* 5. full = tmp (full = full) */
		/* 6. full += line */
		/* 7. freeing tmp so it's not wasted */

		if (DEBUG) { printf("DEBUG: full is: \"%s\"", full) ; }

		if (feof(inpt)) {
			read = false;
		}
	}

	int newlines = 0, localnewlines = 0;
	int i;

	for (i = 0; i < strlen(full); i++) {
		if (full[i] == '\n') {
			newlines++;
		}
	}

	for (i = 0; i < strlen(full); i++) {
		if (full[i] == '\n') {
			localnewlines++;
		}
		if (localnewlines == newlines-1) {
			int j;
			for (j = i; j < strlen(full); i++) {
				full[i] = '\0';
			}
		}
	}

	printf("\"%s\"", full);


	/* ↑ this loop stored whole code to *full, and now comes the hard part */


	return 0;
}
