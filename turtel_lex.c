#include "turtel.h"

int argLen(char *full, int linenn) {
	int i, ret = 0;
	for (i = 0; i < (int)strlen(full); i++) {
		if (full[i] != SEPARATOR) {
			ret ++;
		} else {
			return ret;
		}
	}
	
	fprintf(stderr, "turtel_lex: fatal err: SEPARATOR (%c) not found in line %d\n\t/\\-- around \"%s\"\n", SEPARATOR, linenn, full);
	return -1;
}



char *getArg(char *arguments, int linenn) {
	char *arg = NULL;
	arg = malloc(sizeof(char) * strlen(arguments));
	int i;

	for (i = 0; i < (int)strlen(arguments); i++) {
		arg[i] = '\0';
	}

	for (i = 0; i < (int)strlen(arguments); i++) {
		if (arguments[i] == SEPARATOR) {
			return arg;
		} else {
			arg[i] = arguments[i];
		}
	}
	
	fprintf(stderr, "turtel_lex: fatal err: SEPARATOR (%c) not found in line %d\n\t/\\-- around \"%s\"\n", SEPARATOR, linenn, arguments);
	return NULL;
}

char *getRest(char *full, int from, int linenn) {
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


int tokenize(char* info, int linenn, FILE *out) {
	if (startswith(info, "\n")) {
	} else if (startswith(info, "#")) {
		/* a comment */
	} else if (strcmp(getArg(info, linenn), LEX_INCLUDE) == 0) {
		char *rest = getRest(info, strlen(LEX_INCLUDE)+1, linenn);
		char *name = getArg(rest, linenn);
		if (name == NULL) {
			return 1;
		}
		
		FILE *TMP_INCFILE = fopen(name, "r");
		char TMP_INCLINE[LINE_LEN_MAX];
		int TMP_INCN = 0;

		if (TMP_INCFILE == NULL) {
			fprintf(stderr, "turtel: fatal err: could not open file %s\n"\
					"\t↑ near \"%s\" at line %d\n",
					name, info, linenn
			       );
			return 1;
		}

		while (fgets(TMP_INCLINE, LINE_LEN_MAX, TMP_INCFILE)) {
			if ( tokenize(TMP_INCLINE, TMP_INCN, out) ) {
				return 1;
			}
			TMP_INCN ++;
		}

		fclose(TMP_INCFILE);
		free(name);
		free(rest);
	} else if (strcmp(getArg(info, linenn), PRINT) == 0) {
		fprintf(out, "0");
		/* print the type for interpreter */
		char *rest = getRest(info, strlen(PRINT)+1, linenn);
		
		char *type = getArg(rest, linenn);

		if (type == NULL) {
			return 1;
		}
		
		if (strlen(type) > 3) {
			fprintf(stderr, "turtel_lex: fatal err: expected data structure, got %s. on line %d, near \"%s\"\n", type, linenn, info);
			return 1;
		}

		if (startswith(type, NUM)) {
			fprintf(out, "a");
		} else if (startswith(type, STR)) {
			fprintf(out, "b");
		} else if (startswith(type, TOF)) {
			fprintf(out, "c");
		} else {
			fprintf(stderr, "turtel_lex: fatal err: could not recognise data structure. got \"%s\" on line %d near \"%s\"\n"
					"expected NUM (%s) / STR (%s) / TOF (%s)\n",
					type, linenn, info, NUM, STR, TOF
			       );
			return 1;
		}

		free(rest);
		rest = NULL;
		rest = getRest(info, strlen(PRINT) + 4 + 1, linenn);

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
		char *rest = getRest(info, strlen(READ)+1, linenn);
		
		char *type = getArg(rest, linenn);

		if (type == NULL) {
			return 1;
		}
		
		if (strlen(type) > 3) {
			fprintf(stderr, "turtel_lex: fatal err: expected data structure, got %s. on line %d, near \"%s\"\n", type, linenn, rest);
			return 1;
		}

		if (startswith(type, NUM)) {
			fprintf(out, "a");
		} else if (startswith(type, STR)) {
			fprintf(out, "b");
		} else if (startswith(type, TOF)) {
			fprintf(out, "c");
		} else {
			fprintf(stderr, "turtel_lex: fatal err: could not recognise data structure. got \"%s\" on line %d near \"%s\"\n"
					"expected NUM (%s) / STR (%s) / TOF (%s)\n",
					type, linenn, info, NUM, STR, TOF
			       );
			return 1;
		}

		free(rest);
		rest = NULL;
		rest = getRest(info, strlen(READ) + 4 + 1, linenn);

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

		char *rest = getRest(info, strlen(IF)+1, linenn);
		char *tmpArg = getArg(rest, linenn);
		int restLen = strlen(rest);

		if (tmpArg == NULL) {
			return 1;
		}
		
		fprintf(out, "%s:", tmpArg);

		free(rest);
		rest = NULL;

		rest = getRest(info, strlen(info) - restLen + strlen(tmpArg) + 1, linenn);
		restLen = strlen(rest);

		free(tmpArg);
		tmpArg = NULL;

		tmpArg = getArg(rest, linenn);
		if (tmpArg == NULL) {
			return 1;
		}

		if (strcmp(tmpArg, EQ) == 0) {
			fprintf(out, "=:");
		} else if (strcmp(tmpArg, LESSTHAN) == 0) {
			fprintf(out, "<:");
		} else if (strcmp(tmpArg, GREATERTHAN) == 0) {
			fprintf(out, ">:");
		} else if (strcmp(tmpArg, NOTEQ) == 0) {
			fprintf(out, "!:");
		} else {
			fprintf(
					stderr, 
					"turtel: fatal err at line %d near %s"
					"\t↑ expected %s / %s / %s / %s → got \"%s\"\n",
					linenn, info,
					EQ, LESSTHAN, GREATERTHAN, NOTEQ, tmpArg
			       );
			return 1;
		}

		free(rest);
		rest = NULL;

		rest = getRest(info, strlen(info) - restLen + strlen(tmpArg) + 1, linenn);
		restLen = strlen(rest);

		free(tmpArg);
		tmpArg = NULL;

		tmpArg = getArg(rest, linenn);

		if (tmpArg == NULL) {
			return 1;
		}

		fprintf(out, "%s:", tmpArg);
		
		free(rest);
		rest = NULL;

		rest = getRest(info, strlen(info) - restLen + strlen(tmpArg) + 1, linenn);
		restLen = strlen(rest);

		free(tmpArg);
		tmpArg = NULL;

		tmpArg = getArg(rest, linenn);

		if (tmpArg == NULL) {
			return 1;
		}

		fprintf(out, "%s:", tmpArg);

		free(rest);
		rest = NULL;

		rest = getRest(info, strlen(info) - restLen + strlen(tmpArg) + 1, linenn);

		if (tmpArg == NULL) {
			return 1;
		}

		fprintf(out, "%s;", tmpArg);

		free(rest);
		free(tmpArg);
	} else if (strcmp(getArg(info, linenn), GOTO) == 0) {
		char *rest = getRest(info, strlen(GOTO)+1, linenn);
		char *where = getArg(rest, linenn);
		if (where == NULL) {
			return 1;
		}

		fprintf(out, "5");
		fprintf(out, "%s;", where);
		free(where);
		free(rest);
	} else if (strcmp(getArg(info, linenn), GOTOTAG) == 0) {
		char *rest = getRest(info, strlen(GOTOTAG)+1, linenn);
		char *tag = getArg(rest, linenn);
		if (tag == NULL) {
			return 1;
		}

		fprintf(out, "6");
		fprintf(out, "%s;", tag);
		free(tag);
		free(rest);
	} else if (strcmp(getArg(info, linenn), NOW_EQU) == 0) {
		printf("nowequ not implemented\n");
		/*
		 * NOWEQU - usage
		 * nowequ:type:destvar:type:var:
		 * ex.
		 * nowequ:num:bruh:tof:moment:
		 *
		 * interpreter WILL convert all types to all types
		 */

	} else if (
			strcmp(getArg(info, linenn), ADD) == 0 ||
			strcmp(getArg(info, linenn), SUB) == 0 ||
			strcmp(getArg(info, linenn), MUL) == 0 ||
			strcmp(getArg(info, linenn), DIV) == 0
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
		}

		/* ↑ print what is needed to do, so the interpreter knows co jest sześć B)) */

		char *rest = getRest(info, strlen(ADD)+1, linenn);
		char *dest = getArg(rest, linenn);
		
		if (dest == NULL) {
			return 1;
		}

		fprintf(out, "%s:", dest);

		free(rest);
		rest = NULL;
		rest = getRest(info, strlen(ADD) + 1 + strlen(dest) + 1, linenn);
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
		char *rest = getRest(info, argLen(info, linenn)+1, linenn);
		char *type = getArg(rest, linenn);

		if (type == NULL) {
			return 1;
		}

		if (strlen(type) > 3) {
			fprintf(stderr, "turtel_lex: fatal err: expected data structure, got %s. on line %d, near \"%s\"\n", type, linenn, rest);
			return 1;
		}

		
		if (startswith(type, NUM)) {
			fprintf(out, "a");
		} else if (startswith(type, STR)) {
			fprintf(out, "b");
		} else if (startswith(type, TOF)) {
			fprintf(out, "c");
		} else {
			fprintf(stderr, "turtel_lex: fatal err: could not recognise data structure. got \"%s\" on line %d near \"%s\"\n"
					"expected NUM (%s) / STR (%s) / TOF (%s)\n",
					type, linenn, info, NUM, STR, TOF
			       );
			return 1;
		}

		free(rest);
		rest = NULL;
		rest = getRest(info, argLen(info, linenn) + 1 + 4, linenn);
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

	int opt;

	FILE *tmpf = tmpfile();

	while ((opt = getopt(argc, argv, "f:o:h")) != -1) {
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
				break;
			case 'h':
				printf(
						"turtel_lex - lexical analiser for turtel\n"
						"usage: turtel_lex [-o outfile] [-f infile] [-h]\n"
				      );
				return 0;
		}
	}

	while (fgets(line, LINE_LEN_MAX, inpt)) {
		if ( tokenize(line, line_n, tmpf) ) {
			return 1;
		}
		line_n++;
	}

	char c;
	rewind(tmpf);
	while ((c = fgetc(tmpf)) != EOF) {
		fputc(c, outpt);
	}

	return 0;
}
