#include "turtel.h"

char *getArg(char *arguments, int linenn) {
	char *arg = malloc(sizeof(char) * strlen(arguments));
	int i;

	for (i = 0; i < strlen(arguments); i++) {
		if (arguments[i] == SEPARATOR) {
			return arg;
		} else {
			arg[i] = arguments[i];
		}
	}
	
	fprintf(stderr, "fatal err: SEPARATOR (%c) not found in line %d\n\t/\\-- around %s\n", SEPARATOR, linenn, arguments);
	return NULL;
}

char *getRest(char *full, int from, int linenn) {
	char *ret = malloc(sizeof(char)*strlen(full));
	int i, j = 0;
	for (i = from; i < strlen(full); i++) {
		ret[j] = full[i];
		j++;
	}
	printf("rest is %s\n", ret);
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


int tokenize(char* info, int linenn) {
	if (startswith(info, PRINT)) {
		printf("0");
		/* print the type for interpreter */
		char *rest = getRest(info, strlen(PRINT)+1, linenn);
		
		char rArgs[strlen(rest)];

		char *type = getArg(rArgs, linenn);
		
		if (strlen(type) > 3) {
			fprintf(stderr, "fatal err: expected data structure, got %s. on line %d, near %s\n", type, linenn, rest);
		}

		printf("type: %s\n", type);
		free(rest);

	} else {
		/* declaration */
		printf("DECLARATION NOT IMPLEMENTED\n");
	}
}


int main (int argc, char *argv[]) {
	FILE *in = stdin;
	FILE *out = stdout;
	char line[LINE_LEN_MAX];

	fgets(line, LINE_LEN_MAX, in);
	if ( tokenize(line, 1) ) {
		return 1;
	}

	return 0;
}
