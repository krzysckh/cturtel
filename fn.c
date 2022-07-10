#include "turtel.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char *int_to_str(int d) {
  char *ret;
  int i = 0, tmp = d, j;

  do {
    tmp /= 10;
    i ++;
  } while (tmp != 0);

  ret = malloc(sizeof(char) * (++i + 1));

  for (j = 0; j < i; j++)
    ret[j] = '\0';

  sprintf(ret, "%d", d);
  return ret;
}

void trl_str_fc(int argc, char **argv) {
  if (argc != 1) {
    err("runtime: error using builtin _str_fc: expected 1 argument, got %d", 
        argc);
  }

  if (getvar(argv[0], Str) == NULL) {
    setvar(argv[0], Str, "-1");
    /* yay! it's a NuLL ptr, or an empty empty string
     * (it's -1 because fuck you) */
  } else {
    char *val = int_to_str((int)getvar(argv[0], Str)[0]);
    setvar(argv[0], Str, val);
    free(val);
  }
}

void trl_str_mv(int argc, char **argv) {
  if (argc != 1) {
    err("runtime: error using builtin _str_mv: expected 1 argument, got %d", 
        argc);
  }

  if (getvar(argv[0], Str) == NULL)
    err("runtime: _str_mv %s is an empty variable. exiting.", argv[0]);

  /*printf("getvar argv[0] addr = %p\n", getvar(argv[0], Str));*/
  /*printf("getvar argv[0] = \"%s\"\n", getvar(argv[0], Str));*/

  char *val = malloc(sizeof(char) * (strlen(getvar(argv[0], Str)) + 1));
  strcpy(val, getvar(argv[0], Str));
  val[strlen(val)] = '\0';
  /*printf("val addr = %p\n", val);*/

   ++val;
  /*puts("after ↓");*/
  /*printf("val addr: %p\n", val);*/
  /*iprintd((int)strlen(val));*/
  /*iprints(val);*/

  setvar(argv[0], Str, val);

  free(--val);
}

void trl_win_create(int argc, char **argv) {
  if (argc != 2) {
    err("runtime: error using builtin win_create: expected 2 arguments, got %d", 
        argc);
  }

  x_create_win(atoi(getvar(argv[0], Num)), atoi(getvar(argv[1], Num)));
}

void trl_win_delete(int argc, char **argv) {
  if (argc != 0) {
    err("runtime: error using builtin win_delete: expected 0 arguments, got %d", 
        argc);
  }

  x_del_win();
}

void trl_win_draw(int argc, char **argv) {
  if (argc != 5) {
    err("runtime: error using builtin win_delete: expected 5 arguments, got %d", 
        argc);
  }

  x_draw(
    atoi(getvar(argv[0], Num)),
    atoi(getvar(argv[1], Num)),
    atoi(getvar(argv[2], Num)),
    atoi(getvar(argv[3], Num)),
    atoi(getvar(argv[4], Num))
  );
}

void trl_nowequ(int argc, char **argv) {
  if (argc != 4) {
    err("runtime: error using builtin nowequ: expected 4 arguments, got %d", 
        argc);
  }

  Type t1 = gettype(argv[0]),
       t2 = gettype(argv[2]);

  setvar(argv[1], t1, getvar(argv[3], t2));
}

void trl_exit(int argc, char **argv) {
  if (argc != 0) {
    err("runtime: error using builtin exit: expected 0 arguments, got %d", 
        argc);
  }

  exit(0);
}

void trl_srun(int argc, char **argv) {
  if (argc != 1) {
    err("runtime: error using builtin exit: expected 1 argument, got %d", 
        argc);
  }

  system(getvar(argv[0], Str));
}

void trl_print(int argc, char **argv) {
  if (argc != 2) {
    err("runtime: error using builtin print: expected 2 arguments, got %d", 
        argc);
  }
  Type t = gettype(argv[0]);
  char *v = getvar(argv[1], t);

  printf("%s", v);
  /*if (t == Num && isdigit(argv[1][0]))*/
    /*free(v);*/
  /* free printed ptr, only if it was allocated dynamically (is a digit) */
  fflush(stdout);
}

void trl_nvar(int argc, char **argv) {
  Type t = gettype(argv[1]);

  if (argc != 3) {
    err("runtime: error creating var: expected 2 arguments, got %d", 
        argc-1);
    /* variable name provided as argv[0] */
  }

  if (t == Num && (isdigit(argv[0][0]) || argv[0][0] == '-'))
    err("runtime: cannot write variables into numerical constants (%s)", 
        argv[0]);

  setvar(argv[0], t, argv[2]);
}

void trl_read(int argc, char **argv) {
  if (argc != 2) {
    err("runtime: error using builtin read: expected 2 arguments, got %d", 
        argc);
  }

  if (gettype(argv[0]) == Num && (isdigit(argv[1][0]) || argv[1][0] == '-'))
    err("runtime: read: cannot read into numerical constants");

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
