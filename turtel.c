#include "turtel.h"

int NUM_COUNT = 0;
int STR_COUNT = 0;
int TOF_COUNT = 0;

void err(char *errm) {
	fprintf(stderr, "turtel: fatal error while interpreting. error message is \"%s\"\n", errm);
}

/* all of this code assumes that turtel code is lexed properly */


int main (int argc, char *argv[]) {
	FILE *finpt = stdin;

	if (argv[1] != NULL) {
		finpt = fopen(argv[1], "rb+");
		if (finpt == NULL) {
			fprintf(stderr, "turtel: couldn't open file %s\n", argv[1]);
			return 2;
		}
	}

	FILE *inpt = tmpfile();

	char fc;
	while ((fc = fgetc(finpt)) != EOF) {
		fputc(fc, inpt);
	}
	rewind(inpt);

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
			case '\n':
				break;
			case '#': ;
				/* a hasbang probably */
				char hash_c;
				while ((hash_c = fgetc(inpt)) != '\n') {}
				break;
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
									printf("%lld", NumInfo[i].content);
									found = true;
								}
							}
						}
						if (found == false) {
							err("num was not defined");
							printf("(%s)\n", varName);
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
								printf("(%s)\n", varName);
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
								printf("(%s)\n", varName);
								return 1;
							}
						}

						break;
				}
				break;
			case '1': ;
				char read_tmpc = fgetc(inpt);
				char read_vtmpc;
				char read_varName[LINE_LEN_MAX];
				int read_i;
				bool read_found = false;

				char read_buff[LINE_LEN_MAX];
				for (read_i = 0; read_i < LINE_LEN_MAX; read_i++) {
					read_varName[read_i] = '\0';
					read_buff[read_i] = '\0';
				}

				read_i = 0;

				fgets(read_buff, LINE_LEN_MAX, stdin);

				while ((read_vtmpc = fgetc(inpt)) != ';') {
					read_varName[read_i] = read_vtmpc;
					read_i++;
				}
				read_varName[read_i] = '\0';

				switch (read_tmpc) {
					case 'a': ;
						for (read_i = 0; read_i < NUM_COUNT; read_i++) {
							if (NumInfo[read_i].name != NULL) {
								if (strcmp(read_varName, NumInfo[read_i].name) == 0) {
									NumInfo[read_i].content = atoi(read_buff);

									read_found = true;
								}
							}
						}

						if (read_found == false) {
							NumInfo[NUM_COUNT].name = malloc(sizeof(char) * (strlen(read_varName)+1));
							strncpy(NumInfo[NUM_COUNT].name, read_varName, strlen(read_varName)+1);

							NumInfo[NUM_COUNT].content = atoi(read_buff);

							NUM_COUNT ++;
						}
					
						break;
					case 'b': ;
						for (read_i = 0; read_i < STR_COUNT; read_i++) {
							if (StringInfo[read_i].name != NULL) {
								if (strcmp(read_varName, StringInfo[read_i].name) == 0) {
									StringInfo[read_i].content = NULL;
									free(StringInfo[read_i].content);

									StringInfo[read_i].content = malloc(sizeof(char) * (strlen(read_buff)+1));

									if (strlen(read_buff) <= 1) {
										strncpy(StringInfo[read_i].content, " ", 2);
									} else {
										strncpy(StringInfo[read_i].content, read_buff, strlen(read_buff)-1);
										/* copying strlen() -1 to get rid of newline */
									}


									read_found = true;
								}
							}
						}

						if (read_found == false) {
							StringInfo[STR_COUNT].name = malloc(sizeof(char) * (strlen(read_varName)+1));
							strncpy(StringInfo[STR_COUNT].name, read_varName, strlen(read_varName)+1);

							StringInfo[STR_COUNT].content = malloc(sizeof(char) * (strlen(read_buff)+1));

							if (strlen(read_buff) <= 1) {
								strncpy(StringInfo[STR_COUNT].content, " ", 2);
							} else {
								strncpy(StringInfo[STR_COUNT].content, read_buff, strlen(read_buff)-1);
								/* copying strlen() -1 to get rid of newline */
							}

							STR_COUNT ++;
						}
						break;
					case 'c': ;
						for (read_i = 0; read_i < TOF_COUNT; read_i++) {
							if (TofInfo[read_i].name != NULL) {
								if (strcmp(read_varName, TofInfo[read_i].name) == 0) {
									TofInfo[read_i].content = (atoi(read_buff) == 0) ? false : true;

									read_found = true;
								}
							}
						}

						if (read_found == false) {
							TofInfo[TOF_COUNT].name = malloc(sizeof(char) * (strlen(read_varName)+1));
							strncpy(TofInfo[TOF_COUNT].name, read_varName, strlen(read_varName)+1);

							TofInfo[TOF_COUNT].content = (atoi(read_buff) == 0) ? false : true;

							TOF_COUNT ++;
						}

						break;
				}
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
						if ((strcmp(newVarName, NEWLINE[0]) == 0) || (strcmp(newVarName, SPACE[0]) == 0)) {
							err("cannot set value to predefined turtel constants");
							return 1;
						}

						while ((valc = fgetc(inpt)) != ';') {
							val[vali] = valc;
							vali ++;
						}
						val[vali] = '\0';

						for (i = 0; i < STR_COUNT; i++) {
							if (strcmp(StringInfo[i].name, newVarName) == 0) {
								free(StringInfo[i].content);

								StringInfo[i].len = strlen(val);

								StringInfo[i].content = malloc(sizeof(char) * (strlen(val)+1));
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
					case 'c':
						if (strcmp(newVarName, OS[0]) == 0) {
							err("cannot set value to predefined turtel constants");
							return 1;
						}
						while ((valc = fgetc(inpt)) != ';') {
							val[vali] = valc;
							vali ++;
						}
						val[vali] = '\0';

						for (i = 0; i < TOF_COUNT; i++) {
							if (strcmp(TofInfo[i].name, newVarName) == 0) {
								TofInfo[i].content = (atoi(val) == 0) ? false : true;
								exists = true;
							}
						}

						if (!exists) {
							TofInfo[TOF_COUNT].name = malloc(sizeof(char) * (strlen(newVarName)+1));
							strncpy(TofInfo[TOF_COUNT].name, newVarName, strlen(newVarName)+1);
							
							TofInfo[TOF_COUNT].content = (atoi(val) == 0) ? false : true;
							TOF_COUNT ++;
						}
						break;
				}
				break;
			case '6': ;
				while (fgetc(inpt) != ';') {}
				break;
			case '5': ;
				char gotoWhere[LINE_LEN_MAX];
				for (i = 0; i < LINE_LEN_MAX; i++) {
					gotoWhere[i] = '\0';
				}

				int goto_tmpi = 0;

				char goto_tmpc = 0;
				while ((goto_tmpc = fgetc(inpt)) != ';') {
					gotoWhere[goto_tmpi] = goto_tmpc;
					goto_tmpi ++;
				}

				if (gotoWhere[0] == '\0') break;

				char goto_nameBuff[LINE_LEN_MAX];
				int goto_nameBuff_cptr = 0;
				bool goto_found = false;

				for (i = 0; i < LINE_LEN_MAX; i++) {
					goto_nameBuff[i] = '\0';
				}

				rewind(inpt);
				/* rewinds input back to 0, so it can search for gototag from start */
				while ((goto_tmpc = fgetc(inpt)) != EOF && goto_found == false) {
					if (goto_tmpc == '6') {
						while ((goto_tmpc = fgetc(inpt)) != ';') {
							goto_nameBuff[goto_nameBuff_cptr] = goto_tmpc;
							goto_nameBuff_cptr ++;
						}
						goto_nameBuff[goto_nameBuff_cptr] = '\0';

						if (strcmp(goto_nameBuff, gotoWhere) == 0) {
							fseek(inpt, -1L, SEEK_CUR);
							goto_found = true;
						} else {
							/* clear vars so they can be reused */
							goto_tmpc = 0;
							for (i = 0; i < LINE_LEN_MAX; i++) {
								goto_nameBuff[i] = '\0';
							}
							goto_nameBuff_cptr = 0;
						}
					}
				}
				

				if (goto_found == false) {
					err("gototag not defined");
					fprintf(stderr, "\t(%s)\n", gotoWhere);
					return 1;
				}

				break;
			case '2': ;
				char if_tmpc = 0;
				int if_i = 0;
				bool ifarg1_exists = false, ifarg2_exists = false;

				int arg1 = 0, arg2 = 0;
				char arg1Name[LINE_LEN_MAX], arg2Name[LINE_LEN_MAX];
				for (if_i = 0; if_i < LINE_LEN_MAX; if_i ++) {
					arg1Name[if_i] = arg2Name[if_i] = '\0';
				}

				char operation;
				bool if_isStatementTrue;

				if_i = 0;

				while ((if_tmpc = fgetc(inpt)) != ':') {
					arg1Name[if_i] = if_tmpc;
					if_i ++;
				}

				operation = fgetc(inpt);

				fgetc(inpt); /* to get rid of ':' from fpointer */

				if_i = 0;

				while ((if_tmpc = fgetc(inpt)) != ':') {
					arg2Name[if_i] = if_tmpc;
					if_i ++;
				}
				
				
				for (if_i = 0; if_i < NUM_COUNT; if_i++) {
					if (strcmp(NumInfo[if_i].name, arg1Name) == 0) {
						arg1 = NumInfo[if_i].content;
						ifarg1_exists = true;
					}
				}

				for (if_i = 0; if_i < NUM_COUNT; if_i++) {
					if (strcmp(NumInfo[if_i].name, arg2Name) == 0) {
						arg2 = NumInfo[if_i].content;
						ifarg2_exists = true;
					}
				}

				if (ifarg1_exists == false || ifarg2_exists == false) {
					err("num not defined");
					fprintf(stderr, "\t(%s or %s)\n", arg1Name, arg2Name);
					return 1;
				}

				switch (operation) {
					case '=':
						if (arg1 == arg2) {
							if_isStatementTrue = true;
						} else {
							if_isStatementTrue = false;
						}
						break;
					case '<':
						if (arg1 < arg2) {
							if_isStatementTrue = true;
						} else {
							if_isStatementTrue = false;
						}
						break;
					case '>':
						if (arg1 > arg2) {
							if_isStatementTrue = true;
						} else {
							if_isStatementTrue = false;
						}
						break;
					case '!':
						if (arg1 != arg2) {
							if_isStatementTrue = true;
						} else {
							if_isStatementTrue = false;
						}
						break;
					default:
						err("what");
						return 1;
						break;
				}

				if (if_isStatementTrue) {
					break;
				} else {
					while ((if_tmpc = fgetc(inpt)) != ';') {}
					/* move fpointer to second gototag */
					break;
				}

				break;
			case '7': ;
			case '8': ;
			case '9': ;
			case 'a': ;
			case 'c': ;
				int op_arg1, op_arg2, op_i = 0;
				char op_arg1Name[LINE_LEN_MAX], op_arg2Name[LINE_LEN_MAX];
				for (op_i = 0; op_i < LINE_LEN_MAX; op_i ++) {
					op_arg1Name[op_i] = '\0';
					op_arg2Name[op_i] = '\0';
				}
				op_i = 0;

				bool op_arg1_exists = false, op_arg2_exists = false;
				char op_tmpc;

				while ((op_tmpc = fgetc(inpt)) != ':') { 
					op_arg1Name[op_i] = op_tmpc;
					op_i ++;
				}

				op_i = 0;

				while ((op_tmpc = fgetc(inpt)) != ';') { 
					op_arg2Name[op_i] = op_tmpc;
					op_i ++;
				}

				for (op_i = 0; op_i < NUM_COUNT; op_i++) {
					if (strcmp(NumInfo[op_i].name, op_arg1Name) == 0) {
						op_arg1 = NumInfo[op_i].content;
						op_arg1_exists = true;
					}
				}

				for (op_i = 0; op_i < NUM_COUNT; op_i++) {
					if (strcmp(NumInfo[op_i].name, op_arg2Name) == 0) {
						op_arg2 = NumInfo[op_i].content;
						op_arg2_exists = true;
					}
				}

				if (op_arg1_exists == false || op_arg2_exists == false) {
					err("num not defined");
					fprintf(stderr, "\t(%s or %s)\n", op_arg1Name, op_arg2Name);
					return 1;
				}
				
				switch (c) {
					case '7':
						op_arg1 = op_arg1 + op_arg2;
						break;
					case '8':
						op_arg1 = op_arg1 - op_arg2;
						break;
					case '9':
						op_arg1 = op_arg1 * op_arg2;
						break;
					case 'a':
						op_arg1 = op_arg1 / op_arg2;
						break;
					case 'c':
						op_arg1 = op_arg1 % op_arg2;
						break;
				}


				for (op_i = 0; op_i < NUM_COUNT; op_i++) {
					if (strcmp(NumInfo[op_i].name, op_arg1Name) == 0) {
						NumInfo[op_i].content = op_arg1;
						break;
					}
				}

				break;
			case '4':
				/* very poggers way to exit */
				return 0;
				/* man that was hard */
				break;
			case '3': ;
				/* shell command (just system()) */
				char srun_varName[LINE_LEN_MAX];
				int srun_i;
				char srun_c;
				bool srun_found = false;
				for (srun_i = 0; srun_i < LINE_LEN_MAX; srun_i ++) {
					 srun_varName[srun_i] = '\0';
				}

				srun_i = 0;

				while ((srun_c = fgetc(inpt)) != ';') {
					srun_varName[srun_i] = srun_c;
					srun_i ++;
				}

				for (srun_i = 0; srun_i < STR_COUNT; srun_i ++) {
					if (strcmp(StringInfo[srun_i].name, srun_varName) == 0) {
						system(StringInfo[srun_i].content);
						srun_found = true;
					}
				}

				if (srun_found == false) {
					err("str not defined");
					fprintf(stderr, "\t(name is %s)\n", srun_varName);
					return 1;
				}

				break;
			case 'b': ;
				/* nowequ - converting types of variables */
				char nowequ_c_type1;
				char nowequ_c_type2;
				char nowequ_tmpc;
				char nowequ_varn1[LINE_LEN_MAX];
				char nowequ_varn2[LINE_LEN_MAX];

				char nowequ_var1_val[LINE_LEN_MAX];
				char nowequ_var2_val[LINE_LEN_MAX];
				int nowequ_i;

				for (nowequ_i = 0; nowequ_i < LINE_LEN_MAX; nowequ_i ++) {
					nowequ_varn1[nowequ_i] = '\0';
					nowequ_varn2[nowequ_i] = '\0';
					nowequ_var1_val[nowequ_i] = '\0';
					nowequ_var2_val[nowequ_i] = '\0';
				}

				nowequ_c_type1 = fgetc(inpt);
				/* char representing 1 var -> nowequ_c_type1 */

				nowequ_i = 0;
				while ((nowequ_tmpc = fgetc(inpt)) != ':') {
					nowequ_varn1[nowequ_i] = nowequ_tmpc;
					nowequ_i ++;
				}
				/* name of var1 -> nowequ_var1 */

				nowequ_c_type1 = fgetc(inpt);
				/* char representing 2 var -> nowequ_c_type2 */

				nowequ_i = 0;
				while ((nowequ_tmpc = fgetc(inpt)) != ';') {
					nowequ_varn2[nowequ_i] = nowequ_tmpc;
					nowequ_i ++;
				}

				/* idea: write value of variable 2 to nowequ_var2_val as a char[], so it can be later refered to with atoi() */
				switch (nowequ_c_type2) {
					case 'a':
						
						break;
					case 'b':

						break;
					case 'c':

						break;
				}
				
				
				break;
			default:
				err("not implemended");
				fprintf(stderr, "died on char %c\n", c);
				return 1;
				break;
		}
	}

	int i;
	for (i = 0; i < STR_COUNT; i ++) {
		free(StringInfo[i].name);
		free(StringInfo[i].content);
	}

	return 0;
}
