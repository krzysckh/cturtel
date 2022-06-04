#include "turtel.h"

/* stdio is included */
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

#define MAX_CODE_MACRO 1024

typedef struct {
  Program code;
  char *name;
} LexerCodeMacro;

#define SEPERATOR ':'
int curr_line;
int step_ctr;
int in_macro = 0;

LexerCodeMacro code_macro[MAX_CODE_MACRO];
int code_macro_n = 0;
int code_macro_step_ctr = 0;

void dbg_print_prog_tree(Program p) {
  int i;

  for (i = 0; i < p.steps; i++) {
    switch (p.expr[i].type) {
      case NOOP:
        break;
      case LEX_STAT:
        warn("step %d: !! LEXER MACRO");
        break;
      case NVAR:
        warn("step %d: new varaible %s", i, p.expr[i].argv[0]);
        break;
      case PRINT:
      case READ:
        warn("step %d: I/O action", i);
        break;
      case IF:
        warn("step %d: if", i);
        break;
      case SRUN:
        warn("step %d: srun", i);
        break;
      case EXIT:
        warn("step %d: exit", i);
        break;
      case GOTO:
        warn("step %d: goto", i);
        break;
      case GOTOTAG:
        warn("step %d: goto tag", i);
        break;
      case ADD:
      case SUB:
      case MUL:
      case DIV:
      case MOD:
      case NOWEQU:
        warn("step %d: num/str op (add/sub/.../nowequ)", i);
        break;
      case WIN_CREATE:
      case WIN_DRAW:
      case WIN_DELETE:
        warn("step %d: window operation", i);
        break;
      default:
        break;
    }
  }

}

/* appends prg2 to prg1 at prg1_step
 * assuming realloc_prog was called before, so no overflow will appear
 */
Program append_prog(Program prg1, Program prg2, int prg1_step) {
  int i,
      j,
      k;

  /* literally cannot happen - world would burn */
  if (prg1.steps < prg2.steps) {
    err("there was an error allocating memory");
  }

  for (i = prg1_step, j = 0; j < prg2.steps; i++, j++) {
    prg1.expr[i].argc = prg2.expr[j].argc;
    prg1.expr[i].type = prg2.expr[j].type;
    prg1.expr[i].argv = malloc(sizeof(char*) * prg1.expr[i].argc);

    for (k = 0; k < prg1.expr[i].argc; k++) {
      prg1.expr[i].argv[k] = malloc(sizeof(char) *
          (strlen(prg2.expr[j].argv[k]) + 1));

      strcpy(prg1.expr[i].argv[k], prg2.expr[j].argv[k]);

      prg1.expr[i].argv[k][strlen(prg2.expr[j].argv[k])] = '\0';
      /* this line ends the string based on strlen of the same string
       * in prg2
       */
    }
  }

  return prg1;
}

/* reallocates Program (used for @includes and @defmacros)
 * usage: prg = (prg, prg.steps, step_ctr, prg.steps + 1);
 */
Program realloc_prog(Program prg, int old_sz, int cpy, int new_sz) {
  Program ret;
  int i,
      j;

  ret.steps = new_sz;
  ret.expr = malloc(sizeof(Expr) * new_sz);

  /* copy prg to ret */
  for (i = 0; i < cpy; i++) {
    ret.expr[i].type = prg.expr[i].type;
    ret.expr[i].argc = prg.expr[i].argc;

    ret.expr[i].argv = malloc(sizeof(char*) * prg.expr[i].argc);
    for (j = 0; j < prg.expr[i].argc; j++) {
      ret.expr[i].argv[j] = malloc(sizeof(char) * 
          (strlen(prg.expr[i].argv[j])+1));
      strcpy(ret.expr[i].argv[j], prg.expr[i].argv[j]);

      ret.expr[i].argv[j][strlen(prg.expr[i].argv[j])] = '\0';
    }
  }

  /* it will leak a lil bit of memory until i find out how to write
   * a free_prog() func that doesn't make turtel unstable
   * -____-
   */

  /*free_prog(prg);*/

  return ret;
}

LexerMacroType get_lexer_macro_type(char *t) {
  if (strcmp(t, "@include") == 0) return LEX_INCLUDE;
  else if (strcmp(t, "@defmacro") == 0) return LEX_DEFMACRO;
  else if (strcmp(t, "@endmacro") == 0) return LEX_ENDMACRO;
  else if (strcmp(t, "@run") == 0) return LEX_RUN;
  else err("%d: unknown lexer macro \"%s\"", curr_line, t);

  /* unreachable */
  return -1;
}

Program get_lexer_macro(Program prg, char *line) {
  Program ret,
          tmp_prog;
  LexerMacroType type;
  char *tmp;
  int i = 0,
      fnd = -1,
      step_ctr_bak = step_ctr,
      step_ctr_bak_bak; /* fuck off */

  FILE *m_file;

  /* omit whitespace */
  while (iswspace(*line++))
    ;
  --line;

  tmp = line;

  while (*tmp != '\0' && !iswspace(*tmp++)) {
    /*if (*tmp == '\0')*/
      /*err("%d: fatal: macro \"%s\" with no argument", curr_line, line);*/
    i ++;
  }

  tmp = malloc(sizeof(char) * (i+1));
  strncpy(tmp, line, i);
  tmp[i] = '\0';
  line += ++i;

  type = get_lexer_macro_type(tmp);
  free(tmp);

  switch (type) {
    case LEX_INCLUDE:
      i = 0;
      tmp = line;
      while (*tmp++ != '\0')
        ++i;

      tmp = malloc(sizeof(char) * (i+1));
      strcpy(tmp, line);
      tmp[i] = '\0';

      m_file = fopen(tmp, "r");
      if (m_file == NULL)
        err("fatal: %d cannot open file %s (referenced by a macro)", 
            curr_line, tmp);

      tmp_prog = trl_lex(m_file);

      step_ctr_bak_bak = step_ctr;
      step_ctr = step_ctr_bak;

      ret = realloc_prog(prg, prg.steps, step_ctr, prg.steps + tmp_prog.steps);
      ret = append_prog(ret, tmp_prog, step_ctr);
      free_prog(tmp_prog);

      step_ctr = step_ctr_bak_bak;
      break;
    case LEX_DEFMACRO:
      if (in_macro)
        err("fatal: %d: trying to define macro in a macro", curr_line);
      in_macro = 1;

      i = 0;
      tmp = line;
      while (*tmp++ != '\0')
        i++;

      tmp = malloc(sizeof(char) * (i+1));
      strcpy(tmp, line);
      tmp[i] = '\0';

      for (i = 0; i < code_macro_n; i++)
        if (strcmp(tmp, code_macro[i].name) == 0)
          err("fatal: %d: trying to re-define macro \"%s\"", curr_line, tmp);

      code_macro[code_macro_n].code.expr = malloc(sizeof(Expr) * 1);
      code_macro[code_macro_n].name = malloc(sizeof(char) * strlen(tmp)+1);
      code_macro[code_macro_n].code.steps = 0;
      strcpy(code_macro[code_macro_n].name, tmp);
      code_macro[code_macro_n].name[strlen(tmp)] = '\0';

      free(tmp);
      
      return prg;

      break;
    case LEX_ENDMACRO:
      if (!in_macro)
        err("fatal: %d: trying to end a macro not within a macro", curr_line);

      in_macro = 0;
      /*code_macro[code_macro_n].code.steps--;*/
      code_macro_n ++;
      code_macro_step_ctr = 0;

      prg.steps--;

      return prg;
      break;
    case LEX_RUN:
      i = 0;
      tmp = line;
      while (*tmp++ != '\0')
        ++i;

      tmp = malloc(sizeof(char) * (i+1));
      strcpy(tmp, line);
      tmp[i] = '\0';

      for (i = 0; i < code_macro_n; i++)
        if (strcmp(tmp, code_macro[i].name) == 0)
          fnd = i;

      if (fnd < 0)
        err("fatal: %d: macro %s doesn't exist", curr_line, tmp);
      
      ret = realloc_prog(
        prg,
        prg.steps,
        step_ctr,
        prg.steps + code_macro[fnd].code.steps
      );
      ret = append_prog(ret, code_macro[fnd].code, step_ctr);

      step_ctr += code_macro[fnd].code.steps;

      free(tmp);
      return ret;
      break;
  }

  return ret;
}

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

  if (*line == '@') {
    /* it's a lexer statement */
    ret.type = LEX_STAT;
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
      linesz;
  char *in,
       **line,
       *tmp;
  extern int curr_line;
  curr_line = 1;
  step_ctr = 0;

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
    if (!in_macro) {
      ret.expr[step_ctr] = get_expr(line[i]);

      if (ret.expr[step_ctr].type == LEX_STAT) {
        ret.steps--;
        ret = get_lexer_macro(ret, line[i]);
      } else
        if (ret.expr[step_ctr].type == NOOP)
          ret.steps--;
        else
          step_ctr++;

    } else {
      ret.steps--;

      code_macro[code_macro_n].code = realloc_prog(
        code_macro[code_macro_n].code,
        code_macro[code_macro_n].code.steps,
        code_macro[code_macro_n].code.steps,
        code_macro[code_macro_n].code.steps+1
      );

      code_macro[code_macro_n].code.expr[code_macro_step_ctr] = 
        get_expr(line[i]);

      if (code_macro[code_macro_n].code.expr[code_macro_step_ctr].type 
          == LEX_STAT) {
        code_macro[code_macro_n].code = get_lexer_macro(
          code_macro[code_macro_n].code,
          line[i]
        );
      } else
        if (code_macro[code_macro_n].code.expr[code_macro_step_ctr].type 
            != NOOP) code_macro_step_ctr++;
        else
          code_macro[code_macro_n].code.steps--;
    }
    
    curr_line ++;
  }

  /*
  for (i = 0; i < code_macro_n; i++) {
    printf("Debug Tree for %s\n----------\n", code_macro[i].name);
    iprintd(code_macro[i].code.steps);
    dbg_print_prog_tree(code_macro[i].code);
    printf("-------\n");
  }
  */

  if (in_macro)
    err("fatal: macro %s didn't end on EOF", code_macro[code_macro_n].name);

  for (i = 0; i < code_macro_n; i++)
    free_prog(code_macro[i].code);
  for (i = 0; i < nl; i++)
    free(line[i]);
  free(in);
  free(line);
  return ret;
}
