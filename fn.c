#include "turtel.h"

#include <ctype.h>
#include <stdlib.h>

char *int_to_str(int d) {
  char *ret;
  int i = 0, tmp = d, j;

  do {
    tmp /= 10;
    i ++;
  } while (tmp != 0);

  ret = malloc(sizeof(char) * (i + 1));

  for (j = 0; j < i; j++)
    ret[j] = '\0';

  sprintf(ret, "%d", d);

  return ret;
}

void trl_print(int argc, char **argv) {
  if (argc != 2) {
    err("runtime: error using builtin print: expected 2 arguments, got %d", 
        argc);
  }

  printf("%s", getvar(argv[1], gettype(argv[0])));
  fflush(stdout);
}

void trl_nvar(int argc, char **argv) {
  Type t = gettype(argv[1]);

  if (argc != 3) {
    err("runtime: error creating var: expected 2 arguments, got %d", 
        argc-1);
    /* variable name provided as argv[0] */
  }

  setvar(argv[0], t, argv[2]);
}

void trl_read(int argc, char **argv) {
  if (argc != 2) {
    err("runtime: error using builtin read: expected 2 arguments, got %d", 
        argc);
  }

  char tread[MAX_TRL_READ] = { '\0' };

  fgets(tread, MAX_TRL_READ, stdin);

  setvar(argv[1], gettype(argv[0]), tread);
}

void trl_add(int argc, char **argv) {
  if (argc != 2) {
    err("runtime: error using builtin add: expected 2 arguments, got %d", 
        argc);
  }
  char *tmp = int_to_str(
    atoi(getvar(argv[0], Num)) + atoi(getvar(argv[1], Num)));
  setvar(argv[0], Num, tmp);
  free(tmp);
}

void trl_sub(int argc, char **argv) {
  if (argc != 2) {
    err("runtime: error using builtin sub: expected 2 arguments, got %d", 
        argc);
  }
  char *tmp = int_to_str(
    atoi(getvar(argv[0], Num)) - atoi(getvar(argv[1], Num)));
  setvar(argv[0], Num, tmp);
  free(tmp);
}

void trl_mul(int argc, char **argv) {
  if (argc != 2) {
    err("runtime: error using builtin mul: expected 2 arguments, got %d", 
        argc);
  }
  char *tmp = int_to_str(
    atoi(getvar(argv[0], Num)) * atoi(getvar(argv[1], Num)));
  setvar(argv[0], Num, tmp);
  free(tmp);
}

void trl_div(int argc, char **argv) {
  if (argc != 2) {
    err("runtime: error using builtin div: expected 2 arguments, got %d", 
        argc);
  }
  char *tmp = int_to_str(
    atoi(getvar(argv[0], Num)) / atoi(getvar(argv[1], Num)));
  setvar(argv[0], Num, tmp);
  free(tmp);
}

void trl_mod(int argc, char **argv) {
  if (argc != 2) {
    err("runtime: error using builtin sub: expected 2 arguments, got %d", 
        argc);
  }
  char *tmp = int_to_str(
    atoi(getvar(argv[0], Num)) % atoi(getvar(argv[1], Num)));
  setvar(argv[0], Num, tmp);
  free(tmp);
}
