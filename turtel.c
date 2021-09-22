#include "turtel.h"

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
	
	char *full = (char*) malloc(1); /* initializing char pointer. full code will be stored here */

	/*if (DEBUG) { printf("DEBUG: initialized *full with size of %ld\n", sizeof(full)) ; }*/

	char sizeInChars = 1;
	char c;
	
	while (c != EOF) {
		c = fgetc(inpt);

		if (DEBUG) { printf("DEBUG: got char %c from in\n", c); }

		/*tmp = malloc(sizeof(full));*/
		/*strcpy(tmp, full);*/
		/*free(full);*/
		/*full = NULL;*/
		/*full = malloc(sizeof(tmp) + sizeof(char));*/
		/*strcpy(full, tmp);*/
		/*full[strlen(full)+1] = c;*/
		/*free(tmp);*/
		/*tmp = NULL;*/

		sizeInChars++;
		full = (char*) realloc(full, (size_t) sizeInChars*sizeof(char));

		if (DEBUG) { printf("DEBUG: reallocated full with size (ic chars) of %d\n", sizeInChars); }
		full[strlen(full)] = c;

		if (DEBUG) { printf("DEBUG: full is now \"%s\"\n", full); }
		if (DEBUG) { printf("DEBUG: last char of full is %c (%d)\n", full[strlen(full)+1], full[strlen(full)+1]); }
		if (DEBUG) {
			printf("======= DEBUG =======\n");
			int i;
			for (i = 0; i < strlen(full)+1; i++) {
				printf("char %d in full is '%c' (%d)\n", i, full[i], full[i]);
			}
			printf("==== END DEBUG ======\n");
		}
	}

	if (DEBUG) { printf("full is \"%s\"\n", full); }

	/* ↑ this loop stored whole code to *full, and now comes the hard part */
/*
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
		if (localnewlines == newlines) {
			int j;
			for (j = i; j < strlen(full); i++) {
				full[i] = '\0';
			}
		}
	}
	*/
	/* this newline crap needs to stay here, because the whole program dies if it gets deleted, or replaced with something better -_- */

	/* and the hard part comes now, i lied */

	/*if (DEBUG) {printf("\"%s\"", full); }*/

	/*tokenize(full);*/
	free(full);
	return 0;
}
