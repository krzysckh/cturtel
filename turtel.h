#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define turtel

#define VAR_MAX 100
#define LINE_LEN_MAX 1024

typedef int bool;
#define true 1
#define false 0

#define DEBUG true

#include "stdturtel.h"

typedef struct TurtelString {
	int len;
	char content[];
} TurtelString;

typedef struct TurtelNum {
	int content;
} TurtelNum;

typedef struct TurtelBool {
	bool content;
} TurtelBool;
