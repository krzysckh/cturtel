#include "turtel.h"

#include <stdlib.h>
#include <getopt.h>

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
  int opt;

  while ((opt = getopt(argc, argv, "h")) != -1) {
    switch (opt) {
      case 'h':
        printf("usage: %s [-h] [file.trl]\n", argv[0]);
        exit(0);
        break;
      default:
        exit(1);
    }
  }

  FILE *in;

  if (argv[optind] != NULL) {
    in = fopen(argv[optind], "r");
    if (in == NULL) {
      fprintf(stderr, "failed to open %s\n", argv[optind]);
      exit(1);
    }
  } else {
    in = tmpfile();
    int c;

    while ((c = fgetc(stdin)) != EOF)
      fputc(c, in);

    rewind(in);
  }


  Program prg;
  prg = trl_lex(in);
  run(prg);
  free_prog(prg);

  fclose(in);
  return 0;
}
