#include "turtel.h"

/* stdio is included */
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

#define SEPERATOR ':'
int curr_line;

ExprType get_exprtype(char *pt) {
  if ((int)strlen(pt) < 1) return NOOP;
  else if (strcmp(pt, "print") == 0) return PRINT;
  else if (strcmp(pt, "read") == 0) return READ;
  else if (strcmp(pt, "if") == 0) return IF;
  else if (strcmp(pt, "srun") == 0) return SRUN;
  else if (strcmp(pt, "exit") == 0) return EXIT;
  else if (strcmp(pt, "goto") == 0) return GOTO;
  else if (strcmp(pt, "gototag") == 0) return GOTOTAG;
  else if (strcmp(pt, "add") == 0) return ADD;
  else if (strcmp(pt, "sub") == 0) return SUB;
  else if (strcmp(pt, "mul") == 0) return MUL;
  else if (strcmp(pt, "div") == 0) return DIV;
  else if (strcmp(pt, "nowequ") == 0) return NOWEQU;
  else if (strcmp(pt, "mod") == 0) return MOD;
  else if (strcmp(pt, "_str_mv") == 0) return STR_MV;
  else if (strcmp(pt, "_str_fc") == 0) return STR_FC;
  else if (strcmp(pt, "win_create") == 0) return WIN_CREATE;
  else if (strcmp(pt, "win_draw") == 0) return WIN_DRAW;
  else if (strcmp(pt, "win_delete") == 0) return WIN_DELETE;
  else return NVAR;
}

Expr get_expr(char *line) {
  Expr ret;
  char *tmp;
  extern int curr_line;
  int i,
      linesz;

  /* ignore whitespace */
  while (iswspace(*line++))
    ;
  --line;

  /* abort if comment or empty line */
  if (*line == '#' || *line == '\0') {
    ret.type = NOOP;
    return ret;
  }

  for (i = 0; line[i] != SEPERATOR; i++) {
    if (line[i] == '\0')
      err("%d: %s\n\t! seperator not found", curr_line, line);
  }

  tmp = malloc(sizeof(char) * (i+1));
  strncpy(tmp, line, i);
  tmp[i] = '\0';

  ret.type = get_exprtype(tmp);
  line += strlen(tmp) + 1;


  ret.argc = 0;
  if (ret.type == NVAR) {
    ret.argc ++;  
  }
  
  for (i = 0; i < (int)strlen(line); i++)
    if (line[i] == SEPERATOR)
      ret.argc ++;

  if (ret.argc == 0) {
    free(tmp);
    return ret; /* save time */
  }
  if (ret.argc == 1 && ret.type == NVAR) {
    free(tmp);
    warn("%d: %s\nare you sure about this syntax?", curr_line, line);
    return ret;
  }

  ret.argv = malloc(sizeof(char*) * ret.argc);

  if (ret.type == NVAR) {
    ret.argv[0] = malloc(sizeof(char) * (strlen(tmp) + 1));
    strcpy(ret.argv[0], tmp);
    ret.argv[0][strlen(tmp)] = '\0';
    i = 1;
  } else
    i = 0;

  free(tmp);

  for (; i < ret.argc; i++) {
    tmp = line;
    linesz = 0;
    while (*tmp++ != SEPERATOR)
      linesz++;
    
    ret.argv[i] = malloc(sizeof(char) * (linesz+1));
    strncpy(ret.argv[i], line, linesz);
    ret.argv[i][linesz] = '\0';

    line += ++linesz;
  }

  return ret;
}

Program trl_lex(FILE *fp) {
  Program ret;
  int sz,
      i,
      j,
      nl = 0,
      linesz,
      step_ctr = 0;
  char *in,
       **line,
       *tmp;
  extern int curr_line;
  curr_line = 1;

  fseek(fp, 0L, SEEK_END);
  sz = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  in = malloc(sizeof(char) * (sz+1));
  fread(in, sizeof(char), sz, fp);
  in[sz] = '\0';

  for (i = 0; i < sz; i++)
    if (in[i] == '\n')
      nl ++;

  line = malloc(sizeof(char*) * nl);

  tmp = in;
  for (i = 0; i < nl; i++) {
    linesz = 0;

    for (j = 0; tmp[j] != '\n' && tmp[j] != '\0'; j++)
      linesz ++;

    /*iprintd(i);*/

    line[i] = malloc(sizeof(char) * ++linesz);
    strncpy(line[i], tmp, --linesz);
    line[i][linesz] = '\0';

    tmp += ++linesz;
  }

  /* fp --> **line */

  ret.steps = nl;
  /* remember to decrease when blank line found (!!) */
  ret.expr = malloc(sizeof(Expr) * nl);

  for (i = 0; i < nl; i++) {
    ret.expr[step_ctr] = get_expr(line[i]);
    if (ret.expr[step_ctr].type == NOOP)
      ret.steps --;
    else
      step_ctr ++;

    curr_line ++;
  }

  for (i = 0; i < nl; i++)
    free(line[i]);
  free(in);
  free(line);
  return ret;
}
