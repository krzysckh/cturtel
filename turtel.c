#include "turtel.h"

int NUM_COUNT = 0;
int STR_COUNT = 0;
int TOF_COUNT = 0;

void err(char *errm) {
	fprintf(stderr, "turtel: fatal error while interpreting. error message is \"%s\"\n", errm);
}

int main (int argc, char *argv[]) {
	FILE *inpt = stdin;

	if (argv[1] != NULL) {
		inpt = fopen(argv[1], "r");
		if (inpt == NULL) {
			fprintf(stderr, "turtel: couldn't open file %s\n", argv[1]);
			return 2;
		}
	}

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
					varName[i] = '\0';
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
							printf("varName is %s\n", varName);
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
								err("str was not defined");
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
			case 'N': ;
				char newVarName[LINE_LEN_MAX];
				for (i = 0; i < LINE_LEN_MAX; i++) {
					newVarName[i] = '\0';
				}
				char ntmpc;
				int tmpi = 0;
				while ((ntmpc = fgetc(inpt)) != ':') {
					newVarName[tmpi] = ntmpc;
					tmpi ++;
				}
				
				/* ↑ get var name */

				newVarName[tmpi] = '\0';
				
				ntmpc = fgetc(inpt);
				/* get the type */

				char val[LINE_LEN_MAX];

				for (i = 0; i < LINE_LEN_MAX; i++) {
					val[i] = '\0';
				}

				char valc = 0;
				int vali = 0;
				bool exists = false;
				switch (ntmpc) {
					case 'a':
						while ((valc = fgetc(inpt)) != ';') {
							val[vali] = valc;
							vali ++;
						}
						val[vali] = '\0';

						for (i = 0; i < NUM_COUNT; i++) {
							if (strcmp(NumInfo[i].name, newVarName) == 0) {
								NumInfo[i].content = atoi(val);
								exists = true;
							}
						}

						if (!exists) {
							NumInfo[NUM_COUNT].name = malloc(sizeof(char) * (strlen(newVarName)+1));
							strncpy(NumInfo[NUM_COUNT].name, newVarName, strlen(newVarName)+1);
							
							NumInfo[NUM_COUNT].content = atoi(val);
							NUM_COUNT ++;
						}
						break;
					case 'b':
						while ((valc = fgetc(inpt)) != ';') {
							val[vali] = valc;
							vali ++;
						}
						val[vali] = '\0';

						for (i = 0; i < STR_COUNT; i++) {
							if (strcmp(StringInfo[i].name, newVarName) == 0) {
								if (StringInfo[i].len < strlen(val)) {
									err("new string won't fit in adressed space");
									printf("\t↑ (adressed = %d, needed = %d)\n", StringInfo[i].len, (int)strlen(val));
									return 1;
								}
								strncpy(StringInfo[i].content, val, strlen(val)+1);
								exists = true;
							}
						}

						if (!exists) {
							StringInfo[STR_COUNT].name = malloc(sizeof(char) * (strlen(newVarName)+1));
							strncpy(StringInfo[STR_COUNT].name, newVarName, strlen(newVarName)+1);
							StringInfo[STR_COUNT].len = strlen(val);

							StringInfo[STR_COUNT].content = malloc(sizeof(char) * (strlen(val)+1));
							strncpy(StringInfo[STR_COUNT].content, val, strlen(val)+1);
							STR_COUNT ++;
						}
						break;
					default:
						err("not yet");
						return 1;
						break;
				}
				break;
			default:
				err("null");
				return 1;
				break;
		}
	}
	
	return 0;
}
