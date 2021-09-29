#include "turtel.h"

int NUM_COUNT = 0;
int STR_COUNT = 0;
int TOF_COUNT = 0;

void err(char *errm) {
	fprintf(stderr, "turtel: fatal error while interpreting. error message is \"%s\"\n", errm);
}

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

	char c;
	while ((c = fgetc(inpt)) != EOF) {
		switch (c) {
			case '0': ;
				char tmpc = fgetc(inpt);
				switch (tmpc) {
					case 'a':
						
						break;
					case 'b': ;
						char vtmpc;
						char varName[LINE_LEN_MAX];
						int i = 0;
						while ((vtmpc = fgetc(inpt)) != ';') {
							varName[i] = vtmpc;
							i++;
						}

						varName[i] = '\0';
						
						if (strcmp(varName, NEWLINE[0]) == 0) {
							printf(NEWLINE[1]);
						} else if (strcmp(varName, SPACE[0]) == 0) {
							printf(SPACE[1]);
						} else {
							bool found = false;
							for (i = 0; i < STR_COUNT; i++) {
								if (StringInfo[i].name != NULL) {
									if (strcmp(varName, StringInfo[i].name) == 0) {
										puts(StringInfo[i].content);
										found = true;
									}
								}
							}
							if (found == false) {
								err("string was not defined");
								return 1;
							}
						}
						break;
					case 'c':

						break;
				}
				break;
			case '1':

				break;
			default:
				err("null");
				return 1;
				break;
		}
	}
	
	return 0;
}
