#include "turtel.h"

#include <sys/stat.h>
/* including sys/stat.h only in lexer, 'cause interpreter doesn't need it */

typedef struct LexerMacro {
	char *name;
	FILE *content;
} LexerMacro;
/* define data structure for macros */

int MACRO_COUNT = 0;
int LINE_SYNC = 0;

LexerMacro MacroInfo[VAR_MAX];
/* cannot be in main, cause scope shit probably */
char *rmtab (char *str) {
	int i,j;
	i = 0;
	while (i < (int)strlen(str)) {
		if (str[i] == '\t') {
			for (j = i; j < (int)strlen(str); j++) {
				str[j] = str[j+1];
			}
		} else {
			i ++;
		}
	}
	return str;
}

void codeErr (int line, char *code, int pos) {
	bool snl = false;

	if (strstr(code, "\n") == NULL) snl = true;

	fprintf(stderr, "\t%d | [...] %s", line, code);
	if (snl) fprintf(stderr, "\n");
	fprintf(stderr, "\t");

	int err_i;
	int num_count = 0;
	do {
		line /= 10;
		num_count ++;
	} while (line != 0);

	for (err_i = 0; err_i < num_count; err_i ++) {
		fprintf(stderr, " ");
	}

	for (err_i = 0; err_i <= pos; err_i ++) {
		fprintf(stderr, " ");
	}

	fprintf(stderr, "         ↑\n");
}

int argLen(char *full, int linenn) {
	int i, ret = 0;
	for (i = 0; i < (int)strlen(full); i++) {
		if (full[i] != SEPARATOR) {
			ret ++;
		} else {
			return ret;
		}
	}
	
	fprintf(stderr, "turtel_lex: fatal err: SEPARATOR (%c) not found\n", SEPARATOR);
	codeErr(linenn, full, strlen(full));
	return -1;
}

int checkLegal(char *what, int linee) {
	int c_i;
	for (c_i = 0; c_i < (int)strlen(what); c_i++) {
		if ((what[c_i] == ILLEGAL_C[0]) || (what[c_i] == ILLEGAL_C[1]) || (what[c_i] == ILLEGAL_C[2]) || (what[c_i] == ILLEGAL_C[3])) {
			fprintf(stderr, "turtel_lex: fatal err: illegal char\n");
			codeErr(linee, what, c_i);
			return 1;
		}
	}
	return 0;
}


char *getArg(char *arguments, int linenn) {
	arguments = rmtab(arguments);
	char *arg = NULL;
	arg = malloc(sizeof(char) * strlen(arguments));
	int i;

	for (i = 0; i < (int)strlen(arguments); i++) {
		arg[i] = '\0';
	}

	for (i = 0; i < (int)strlen(arguments); i++) {
		if (arguments[i] == SEPARATOR) {
			if (checkLegal(arg, linenn) != 0) {
				return NULL;
			} else {
				return arg;
			}
		} else {
			arg[i] = (arguments[i] == '\t') ? 0 : arguments[i];
		}
	}
	
	fprintf(stderr, "turtel_lex: fatal err: SEPARATOR (%c) not found\n", SEPARATOR);
	codeErr(linenn, arguments, strlen(arguments));
	return NULL;
}

char *getLexerArg(char *arguments, int linenn) {
	char *arg = NULL;
	arg = malloc(sizeof(char) * strlen(arguments));
	int i;

	for (i = 0; i < (int)strlen(arguments); i++) {
		arg[i] = '\0';
	}

	for (i = 0; i < (int)strlen(arguments); i++) {
		if ((arguments[i] == ' ') || (arguments[i] == '\n')) {
			return arg;
		} else {
			arg[i] = (arguments[i] == '\t') ? 0 : arguments[i];
		}
	}
	
	fprintf(stderr, "turtel_lex: fatal err: man. i don't understand this lexer definition\n");
	codeErr(linenn, arguments, 0);
	return NULL;
}

char *getRest(char *full, int from) {
	char *ret = malloc(sizeof(char)*strlen(full));
	int i, j = 0;
	for (i = from; i < (int)strlen(full); i++) {
		ret[j] = full[i];
		j++;
	}
	return ret;
}


int startswith(char* what, char* withwhat) {
	bool doesIt = true, shouldSkip = false;
	int i;

	if ((int)strlen(what) < (int)strlen(withwhat)) {
		return false;
	}

	for (i = 0; i < (int)strlen(what) && shouldSkip == false; i++) {
		if (what[i] == '\t' || what[i] == 32) {
			int j;
			for (j = i; j < (int)strlen(what); j++) {
				what[j] = what[j+1];
			}
			what[j] = '\0';
		} else {
			shouldSkip = true;
		}
	}

	/* delete all tabs and spaces from line, so it wont get counted as a char */

	for (i = 0; i < (int)strlen(withwhat); i++) {
		if (what[i] != withwhat[i]) {
			doesIt = false;
			continue;
		}
	}
	
	return doesIt;
}


int tokenize(char* info, int linenn, FILE *out, FILE *in) {
	if (startswith(info, "\n")) {
	} else if (startswith(info, "#")) {
		/* a comment */
	} else if (strcmp(getLexerArg(info, linenn), LEX_INCLUDE) == 0) {
		char *rest = getRest(info, strlen(LEX_INCLUDE)+1);
		char *name = getLexerArg(rest, linenn);
		if (name == NULL) {
			return 1;
		}
		
		FILE *TMP_INCFILE = fopen(name, "r");
		char TMP_INCLINE[LINE_LEN_MAX];
		int TMP_INCN = 0;

		if (TMP_INCFILE == NULL) {
			fprintf(stderr, "turtel: fatal err: could not open file %s\n", name);
			codeErr(linenn, info, strlen(LEX_INCLUDE)+1);
			return 1;
		}

		while (fgets(TMP_INCLINE, LINE_LEN_MAX, TMP_INCFILE)) {
			if ( tokenize(TMP_INCLINE, TMP_INCN, out, in) ) {
				return 1;
			}
			TMP_INCN ++;
		}

		fclose(TMP_INCFILE);
		free(name);
		free(rest);
	} else if (strcmp(getLexerArg(info, linenn), LEX_NEWMACRO) == 0) {
		char *rest = getRest(info, strlen(LEX_NEWMACRO)+1);
		char *name = getLexerArg(rest, linenn);
		if (name == NULL) {
			return 1;
		}

		int mac_i;

		for (mac_i = 0; mac_i < MACRO_COUNT; mac_i ++) {
			if (strcmp(name, MacroInfo[mac_i].name) == 0) {
				fprintf(stderr, "turtel_lex: fatal err: macro %s was defined earlier\n", name);
				codeErr(linenn, info, 0);
				return 1;
			}
		}
		
		char mac_line[LINE_LEN_MAX];
		bool mac_get = true;

		MacroInfo[MACRO_COUNT].content = tmpfile();

		fgets(mac_line, LINE_LEN_MAX, in);

		linenn ++;
		while (mac_get) {
			int mac_err = tokenize(mac_line, linenn, MacroInfo[MACRO_COUNT].content, in);

			if (mac_err == 100) {
				mac_get = false;
			} else if (mac_err != 0) {
				return 1;
			}
			if (mac_get) {
				if (!fgets(mac_line, LINE_LEN_MAX, in)) {
					mac_get = false;
				}
			}

			linenn ++;
			LINE_SYNC = linenn;
		}

		MacroInfo[MACRO_COUNT].name = malloc(sizeof(char) * strlen(name)+1);
		strncpy(MacroInfo[MACRO_COUNT].name, name, strlen(name));

		MACRO_COUNT ++;
		free(name);
		free(rest);

	} else if (strcmp(getLexerArg(info, linenn), LEX_ENDMACRO) == 0) {
		return 100;
	} else if (strcmp(getLexerArg(info, linenn), LEX_RUNMACRO) == 0) {
		char *rest = getRest(info, strlen(LEX_RUNMACRO)+1);
		char *name = getLexerArg(rest, linenn);

		if (name == NULL) {
			return 1;
		}

		int run_i;
		char run_c;

		for (run_i = 0; run_i < MACRO_COUNT; run_i ++) {
			if (strcmp(MacroInfo[run_i].name, name) == 0) {
				FILE *run_fp = MacroInfo[run_i].content;
				rewind(run_fp);

				while ((run_c = fgetc(run_fp)) != EOF) {
					fputc(run_c, out);
				}
				free(name);
				free(rest);
				return 0;
			}
		}
		
		/* if still here, macro doesnt exist */

		fprintf(stderr, "turtel_lex: fatal err: macro %s doesn't exist\n", name);
		codeErr(linenn, info, 0);
		free(name);
		free(rest);
		return 1;
	} else if (strcmp(getArg(info, linenn), PRINT) == 0) {
		fprintf(out, "0");
		/* print the type for interpreter */
		char *rest = getRest(info, strlen(PRINT)+1);
		
		char *type = getArg(rest, linenn);

		if (type == NULL) {
			return 1;
		}
		
		if (strlen(type) > 3) {
			fprintf(stderr, "turtel_lex: fatal err: expected data structure, got %s\n", type);
			codeErr(linenn, info, strlen(PRINT));
			return 1;
		}

		if (strcmp(type, NUM) == 0) {
			fprintf(out, "a");
		} else if (strcmp(type, STR) == 0) {
			fprintf(out, "b");
		} else if (strcmp(type, TOF) == 0) {
			fprintf(out, "c");
		} else {
			fprintf(stderr, "turtel_lex: fatal err: could not recognise data structure\n"
					"expected NUM (%s) / STR (%s) / TOF (%s)\n",
					NUM, STR, TOF
			       );
			codeErr(linenn, info, strlen(PRINT)+1);
			return 1;
		}

		free(rest);
		rest = NULL;
		rest = getRest(info, strlen(PRINT) + 4 + 1);

		char *varn = getArg(rest, linenn);
		if (varn == NULL) {
			return 1;
		}

		fprintf(out, "%s;", varn);

		/* free vars at end */
		free(type);
		free(varn);
		free(rest);
	} else if (strcmp(getArg(info, linenn), READ) == 0) {
		fprintf(out, "1");
		/* print the type for interpreter */
		char *rest = getRest(info, strlen(READ)+1);
		
		char *type = getArg(rest, linenn);

		if (type == NULL) {
			return 1;
		}
		
		if (strlen(type) > 3) {
			fprintf(stderr, "turtel_lex: fatal err: expected data structure, got %s\n", type);
			codeErr(linenn, info, strlen(PRINT)+1);
			return 1;
		}

		if (strcmp(type, NUM) == 0) {
			fprintf(out, "a");
		} else if (strcmp(type, STR) == 0) {
			fprintf(out, "b");
		} else if (strcmp(type, TOF) == 0) {
			fprintf(out, "c");
		} else {
			fprintf(stderr, "turtel_lex: fatal err: could not recognise data structure\n"
					"expected NUM (%s) / STR (%s) / TOF (%s)\n",
					NUM, STR, TOF
			       );
			codeErr(linenn, info, strlen(PRINT)+1);
			return 1;
		}

		free(rest);
		rest = NULL;
		rest = getRest(info, strlen(READ) + 4 + 1);

		char *varn = getArg(rest, linenn);
		if (varn == NULL) {
			return 1;
		}

		fprintf(out, "%s;", varn);

		/* free vars at end */
		free(type);
		free(varn);
		free(rest);
	} else if (strcmp(getArg(info, linenn), IF) == 0) {
		/*
		 * ex.
		 * 	a:num:5:
		 * 	b:num:6:
		 *
		 * 	if:a:eq:b:__aeqb:__aneqb:
		 *
		 * 	gototag:__aeqb:
		 * 	info:str:a is equal to b:
		 * 	goto:__ret:
		 *
		 * 	gototag:__aneqb:
		 * 	info:str:a is not equal to b:
		 * 	goto:__ret:
		 *
		 * 	gototag:__ret:
		 * 	print:str:__ret:
		 * 	print:str:__newline:
		 *
		 *	syntax:
		 *		if:num eq/lt/gt/ne num:goto_if_true:goto_if_false:
		 */
		fprintf(out, "2");

		char *rest = getRest(info, strlen(IF)+1);
		char *arg1, *arg2, *op, *goto1, *goto2;

		arg1 = getArg(rest, linenn);

		if (arg1 == NULL) {
			return 1;
		}
		
		fprintf(out, "%s:", arg1);

		free(rest);
		rest = NULL;

		rest = getRest(info, strlen(IF) + 1 + strlen(arg1) + 1);

		op = getArg(rest, linenn);
		if (op == NULL) {
			return 1;
		}

		if (strcmp(op , EQ) == 0) {
			fprintf(out, "=:");
		} else if (strcmp(op, LESSTHAN) == 0) {
			fprintf(out, "<:");
		} else if (strcmp(op, GREATERTHAN) == 0) {
			fprintf(out, ">:");
		} else if (strcmp(op, NOTEQ) == 0) {
			fprintf(out, "!:");
		} else {
			fprintf(
					stderr, 
					"turtel_lex: fatal err: expected %s / %s / %s / %s, got \"%s\"\n",
					EQ, LESSTHAN, GREATERTHAN, NOTEQ, op
			       );
			codeErr(linenn, info, strlen(IF)+1+strlen(arg1));
			return 1;
		}

		free(rest);
		rest = NULL;

		rest = getRest(info, strlen(IF) + 1 + strlen(arg1) + 1 + strlen(op) + 1);

		arg2 = getArg(rest, linenn);

		if (arg2 == NULL) {
			return 1;
		}

		fprintf(out, "%s:", arg2);
		
		free(rest);
		rest = NULL;

		rest = getRest(info, strlen(IF) + 1 + strlen(arg1) + 1 + strlen(op) + 1 + strlen(arg2) + 1);

		goto1 = getArg(rest, linenn);

		if (goto1 == NULL) {
			return 1;
		}

		fprintf(out, "5%s;", goto1);

		free(rest);
		rest = NULL;

		rest = getRest(info, strlen(IF) + 1 + strlen(arg1) + 1 + strlen(op) + 1 + strlen(arg2) + 1 + strlen(goto1) + 1);
		goto2 = getArg(rest, linenn);

		if (goto2 == NULL) {
			return 1;
		}

		fprintf(out, "5%s;", goto2);

		free(rest);
		free(arg1);
		free(arg2);
		free(op);
		free(goto1);
		free(goto2);
	} else if (strcmp(getArg(info, linenn), GOTO) == 0) {
		char *rest = getRest(info, strlen(GOTO)+1);
		char *where = getArg(rest, linenn);
		if (where == NULL) {
			return 1;
		}

		fprintf(out, "5");
		fprintf(out, "%s;", where);
		free(where);
		free(rest);
	} else if (strcmp(getArg(info, linenn), GOTOTAG) == 0) {
		char *rest = getRest(info, strlen(GOTOTAG)+1);
		char *tag = getArg(rest, linenn);
		if (tag == NULL) {
			return 1;
		}

		fprintf(out, "6");
		fprintf(out, "%s;", tag);
		free(tag);
		free(rest);
	} else if (strcmp(getArg(info, linenn), EXIT) == 0) {
		fprintf(out, "4");
	} else if (strcmp(getArg(info, linenn), SRUN) == 0) {
		char *run = getArg(getRest(info, strlen(SRUN)+1), linenn);
		if (run == NULL) {
			return 1;
		}
		fprintf(out, "3");
		fprintf(out, "%s;", run);
		free(run);
	} else if (strcmp(getArg(info, linenn), NOWEQU) == 0) {
		/*
		 * NOWEQU - usage
		 * nowequ:type:destvar:type:var:
		 * ex.
		 * nowequ:num:bruh:tof:moment:
		 *
		 * interpreter WILL convert all types to all types
		 */

		fprintf(out, "b");

		char *rest = getRest(info, strlen(NOWEQU)+1);
		char *type1 = getArg(rest, linenn);
		if (type1 == NULL) {
			return 1;
		}

		free(rest);
		rest = getRest(info, strlen(NOWEQU) + strlen(type1) + 2);

		char *var1 = getArg(rest, linenn);
		if (var1 == NULL) {
			return 1;
		}

		free(rest);
		rest = getRest(info, strlen(NOWEQU) + strlen(type1) + strlen(var1) + 3);

		char *type2 = getArg(rest, linenn);
		if (type2 == NULL) {
			return 1;
		}

		free(rest);
		rest = getRest(info, strlen(NOWEQU) + strlen(type1) + strlen(var1) + strlen(type2) + 4);

		char *var2 = getArg(rest, linenn);
		if (var2 == NULL) {
			return 1;
		}


		if (strcmp(type1, NUM) == 0) {
			fprintf(out, "a");
		} else if (strcmp(type1, STR) == 0) {
			fprintf(out, "b");
		} else if (strcmp(type1, TOF) == 0) {
			fprintf(out, "c");
		} else {
			fprintf(stderr, "turtel_lex: fatal err: could not recognise data structure\n"
					"expected NUM (%s) / STR (%s) / TOF (%s)\n",
					NUM, STR, TOF
			       );
			codeErr(linenn, info, strlen(NOWEQU)+1);
			return 1;
		}

		fprintf(out, "%s:", var1);

		if (strcmp(type2, NUM) == 0) {
			fprintf(out, "a");
		} else if (strcmp(type2, STR) == 0) {
			fprintf(out, "b");
		} else if (strcmp(type2, TOF) == 0) {
			fprintf(out, "c");
		} else {
			fprintf(stderr, "turtel_lex: fatal err: could not recognise data structure\n"
					"expected NUM (%s) / STR (%s) / TOF (%s)\n",
					NUM, STR, TOF
			       );
			codeErr(linenn, info, strlen(NOWEQU)+strlen(var1)+5);
			return 1;
		}

		fprintf(out, "%s;", var2);

		free(type1);
		free(type2);
		free(var1);
		free(var2);
		free(rest);

	} else if (
			strcmp(getArg(info, linenn), ADD) == 0 ||
			strcmp(getArg(info, linenn), SUB) == 0 ||
			strcmp(getArg(info, linenn), MUL) == 0 ||
			strcmp(getArg(info, linenn), DIV) == 0 ||
			strcmp(getArg(info, linenn), MOD) == 0
			) {

		/* adding, subtracting, multiplying, dividing */

		if (strcmp(getArg(info, linenn), ADD) == 0) {
			fprintf(out, "7");
		} else if (strcmp(getArg(info, linenn), SUB) == 0) {
			fprintf(out, "8");
		} else if (strcmp(getArg(info, linenn), MUL) == 0) {
			fprintf(out, "9");
		} else if (strcmp(getArg(info, linenn), DIV) == 0) {
			fprintf(out, "a");
		} else if (strcmp(getArg(info, linenn), MOD) == 0) {
			fprintf(out, "c");
		}

		/* ↑ print what is needed to do, so the interpreter knows co jest sześć B)) */

		char *rest = getRest(info, strlen(ADD)+1);
		char *dest = getArg(rest, linenn);
		
		if (dest == NULL) {
			return 1;
		}

		fprintf(out, "%s:", dest);

		free(rest);
		rest = NULL;
		rest = getRest(info, strlen(ADD) + 1 + strlen(dest) + 1);
		char *from = getArg(rest, linenn);
		if (from == NULL) {
			return 1;
		}

		fprintf(out, "%s;", from);
		free(from);
		free(dest);
	} else {
		/* declaration */
		fprintf(out, "N");
		char *name = getArg(info, linenn);
		fprintf(out, "%s:", name);
		free(name);
		char *rest = getRest(info, argLen(info, linenn)+1);
		char *type = getArg(rest, linenn);

		if (type == NULL) {
			free(rest);
			free(type);
			return 1;
		}

		if (strlen(type) > 3) {
			fprintf(stderr, "turtel_lex: fatal err: expected data structure, got %s\n", type);
			codeErr(linenn, info, strlen(name)+1);
			return 1;
		}

		
		if (strcmp(type, NUM) == 0) {
			fprintf(out, "a");
		} else if (strcmp(type, STR) == 0) {
			fprintf(out, "b");
		} else if (strcmp(type, TOF) == 0) {
			fprintf(out, "c");
		} else {
			fprintf(stderr, "turtel_lex: fatal err: could not recognise data structure\n"
					"expected NUM (%s) / STR (%s) / TOF (%s)\n",
					NUM, STR, TOF
			       );
			codeErr(linenn, info, strlen(PRINT)+1);
			return 1;
		}

		free(rest);
		rest = NULL;
		rest = getRest(info, argLen(info, linenn) + 1 + 4);
		char *val = getArg(rest, linenn);
		if (val == NULL) {
			return 1;
		}

		fprintf(out, "%s;", val);

		/* free memory */
		free(type);
		free(rest);
		free(val);

	}
	return 0;
}


int main (int argc, char *argv[]) {
	FILE *inpt = stdin;
	FILE *outpt = stdout;
	char line[LINE_LEN_MAX];
	int line_n = 1;
	bool executable = false;
	char *ofname = NULL;

	int opt;

	FILE *tmpf = tmpfile();


	while ((opt = getopt(argc, argv, "f:o:he")) != -1) {
		switch(opt) {
			case 'f':
				inpt = fopen(optarg, "r");
				if (inpt == NULL) {
					fprintf(stderr, "file %s doesn't exist\n", optarg);
					return 1;
				}
				break;
			case 'o':
				outpt = fopen(optarg, "w");
				ofname = malloc(sizeof(char) * (strlen(optarg)+1));
				strcpy(ofname, optarg);

				break;
			case 'e':
				executable = true;
				break;
			case 'h':
				printf(
						"turtel_lex - lexical analiser for turtel\n"
						"usage: turtel_lex [-o outfile] [-f infile] [-he]\n"
				      );
				return 0;
		}
	}


	if (argv[optind] != NULL) {
		inpt = fopen(argv[optind], "r");
		if (inpt == NULL) {
			fprintf(stderr, "file %s doesn't exist\n", argv[optind]);
			return 1;
		}
	}



	if (executable) {
		if (ofname == NULL) {
			fprintf(stderr, "turtel_lex: fatal err: cannot make <stdin> executable. try using -o to set output file\n");
			return 1;
		}

		fprintf(tmpf, "#!/usr/bin/turtel\n");
	}


	while (fgets(line, LINE_LEN_MAX, inpt)) {
		int err;
		if ((err = tokenize(line, line_n, tmpf, inpt)) != 0) {
			if (err == 100) {
				fprintf(stderr, "turtel_lex: fatal_err: got %s, but no macro was started\n", LEX_ENDMACRO);
				codeErr(line_n, line, 0);
			}
			fclose(inpt);
			fclose(outpt);
			fclose(tmpf);
			return 1;
		}
		line_n = (LINE_SYNC > line_n) ? LINE_SYNC : line_n + 1;
	}

	char c;
	rewind(tmpf);
	while ((c = fgetc(tmpf)) != EOF) {
		fputc(c, outpt);
	}

	if (executable) {
		if (chmod(ofname, strtol("0777", 0, 0)) < 0) {
			fprintf(stderr, "turtel_lex: warning: couldn't make %s executable. try 'chmod +x %s' to do it yourself\n",
					ofname, ofname);
		}
	}

	fclose(tmpf);
	fclose(outpt);

	return 0;
}
