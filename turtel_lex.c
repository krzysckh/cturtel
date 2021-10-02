#include "turtel.h"

int argLen(char *full, int linenn) {
	int i, ret = 0;
	for (i = 0; i < strlen(full); i++) {
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

	for (i = 0; i < strlen(arguments); i++) {
		arg[i] = '\0';
	}

	for (i = 0; i < strlen(arguments); i++) {
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
	for (i = from; i < strlen(full); i++) {
		ret[j] = full[i];
		j++;
	}
	return ret;
}


int startswith(char* what, char* withwhat) {
	bool doesIt = true, shouldSkip = false;
	int i;

	if (strlen(what) < strlen(withwhat)) {
		return false;
	}

	for (i = 0; i < strlen(what) && shouldSkip == false; i++) {
		if (what[i] == '\t' || what[i] == 32) {
			int j;
			for (j = i; j < strlen(what); j++) {
				what[j] = what[j+1];
			}
			what[j] = '\0';
		} else {
			shouldSkip = true;
		}
	}

	/* delete all tabs and spaces from line, so it wont get counted as a char */

	for (i = 0; i < strlen(withwhat); i++) {
		if (what[i] != withwhat[i]) {
			doesIt = false;
			continue;
		}
	}
	
	return doesIt;
}


int tokenize(char* info, int linenn, FILE *out) {
	if (startswith(info, PRINT)) {
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
	} else if (startswith(info, READ)) {
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
	} else if (startswith(info, "\n")) {
	} else {
		/* declaration */
		fprintf(out, "N");
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
		if ( tokenize(line, line_n, outpt) ) {
			return 1;
		}
		line_n++;
	}

	return 0;
}
