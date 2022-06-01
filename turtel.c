#include "turtel.h"

#include <stdlib.h>

void free_prog(Program p) {
  int i, j;
  for (i = 0; i < p.steps; i++) {
    for (j = 0; j < p.expr[i].argc; j++)
      free(p.expr[i].argv[j]);
    if (p.expr[i].argc > 0)
      free(p.expr[i].argv);
  }

  free(p.expr);
}

int main (int argc, char *argv[]) {
  FILE *test = fopen("test.trl", "r");
  Program testp;
  testp = trl_lex(test);

  run(testp);

  free_prog(testp);

  fclose(test);
  return 0;
}
