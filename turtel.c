#include "turtel.h"

int NUM_COUNT = 0;
int STR_COUNT = 0;
int TOF_COUNT = 0;

void err(char *errm) {
	fprintf(stderr, "turtel: fatal error while interpreting. error message is \"%s\"\n", errm);
}

int main (int argc, char *argv[]) {
	FILE *inpt = stdin;

	TurtelString StringInfo[VAR_MAX];
	TurtelNum NumInfo[VAR_MAX];
	TurtelBool TofInfo[VAR_MAX];

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
				char vtmpc;
				char varName[LINE_LEN_MAX];
				int i;
				bool found = false;
				for (i = 0; i < LINE_LEN_MAX; i++) {
					varName[i] = ' ';
				}

				i = 0;

				switch (tmpc) {
					case 'a': ;
						while ((vtmpc = fgetc(inpt)) != ';') {
							varName[i] = vtmpc;
							i++;
						}

						varName[i] = '\0';

						for (i = 0; i < NUM_COUNT; i++) {
							if (NumInfo[i].name != NULL) {
								if (strcmp(varName, NumInfo[i].name) == 0) {
									printf("%d", NumInfo[i].content);
									found = true;
								}
							}
						}
						if (found == false) {
							err("num was not defined");
							return 1;
						}
					
						break;
					case 'b': ;
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
							for (i = 0; i < STR_COUNT; i++) {
								if (StringInfo[i].name != NULL) {
									if (strcmp(varName, StringInfo[i].name) == 0) {
										printf("%s", StringInfo[i].content);
										found = true;
									}
								}
							}
							if (found == false) {
								err("string was not defined");
								printf("varName is %s\n", varName);
								return 1;
							}
						}
						break;
					case 'c': ;
						while ((vtmpc = fgetc(inpt)) != ';') {
							varName[i] = vtmpc;
							i++;
						}

						varName[i] = '\0';

						if (strcmp(varName, OS[0]) == 0) {
							printf("%d", atoi(OS[1]));
						} else {
							for (i = 0; i < TOF_COUNT; i++) {
								if (TofInfo[i].name != NULL) {
									if (strcmp(varName, TofInfo[i].name) == 0) {
										printf("%d", TofInfo[i].content);
										found = true;
									}
								}
							}
							if (found == false) {
								err("tof was not defined");
								printf("varName is %s\n", varName);
								return 1;
							}
						}

						break;
				}
				break;
			case '1':

				break;
			case 'N':
				err("declaring not implemented yet");
				return 2;
				break;
			default:
				err("null");
				return 1;
				break;
		}
	}
	
	return 0;
}
