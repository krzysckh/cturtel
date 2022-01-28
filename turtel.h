#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#define turtel

#define VAR_MAX 100 /* still used for lexer macros */
#define LINE_LEN_MAX 1024

typedef int bool;
#define true 1
#define false 0

#define DEBUG false

#include "stdturtel.h"

typedef struct TurtelString {
	char *name;
	int len;
	char *content;
} TurtelString;

typedef struct TurtelNum {
	char *name;
	long long content;
} TurtelNum;

typedef struct TurtelBool {
	char *name;
	bool content;
} TurtelBool;
