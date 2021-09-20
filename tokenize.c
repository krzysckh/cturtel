int startsWith(char* what, char* withwhat) {
	bool doesIt = true;
	int i;

	for (i = 0; i < strlen(what); i++) {
		if (what[i] == '\t' || what[i] == ' ') {
			int j;
			for (j = i; j < strlen(what); j++) {
				what[j] = what[j+1];
			}
			what[j] = '\0';
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


void tokenize(char* info) {
	int howManyLines = 0, longestLine = 0, currentLine = 0;
	int i;
	for (i = 0; i < strlen(info); i++) {
		howManyLines = (info[i] == '\n') ? howManyLines + 1 : howManyLines;
		longestLine = (currentLine > longestLine) ? currentLine : longestLine;
		currentLine = (info[i] == '\n' || info[i] == '\0') ? 0 : currentLine + 1;
	}
	
	howManyLines++;
	/* it counts one line less than there actually are (engrish moment) */
	longestLine++;
	/* and it's also really bad at counting characters -____- */

	char lines[howManyLines] [longestLine];

	if (DEBUG) { printf("DEBUG: counted %d lines. longest line is %d chars\n", howManyLines, longestLine); }

	int whichLineAmiOn = 0, place = 0; /* im really creative */
	
	for (i = 0; i < strlen(info); i++) {
		if (info[i] == '\n' || info[i] == '\0') {
			whichLineAmiOn++;
			place = 0;
		} else {
			lines[whichLineAmiOn] [place] = info[i];
			if (DEBUG) { printf("DEBUG: put char %c in line %d at place %d from place %d. it gives %s\n", info[i], whichLineAmiOn, place, i, lines[whichLineAmiOn]); }
			place++;
		}
	}

	for (i = 0; i < howManyLines; i++) {
		printf("line %d is %s\n", i, lines[i]);
	}
}
