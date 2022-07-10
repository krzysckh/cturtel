#include <stdio.h>

#define MAX_TRL_READ 1024

#define iprintd(x) printf(#x " = %d\n", x)
#define iprints(x) printf(#x " = \"%s\"\n", x)

/* data structures */
typedef enum {
  Str,
  Num,
  Tof
} Type;

/* "functions" */
typedef enum {
  NOOP,
  LEX_STAT,
  NVAR,
  PRINT,
  READ,
  IF,
  SRUN,
  EXIT,
  GOTO,
  GOTOTAG,
  ADD,
  SUB,
  MUL,
  DIV,
  NOWEQU,
  MOD,
  STR_MV,
  STR_FC,
  WIN_CREATE,
  WIN_DRAW,
  WIN_DELETE
} ExprType;

typedef enum {
  LEX_INCLUDE,
  LEX_DEFMACRO,
  LEX_ENDMACRO,
  LEX_RUN
} LexerMacroType;

typedef struct {
  Type type;
  char *content;
  char *name;
} Variable;

typedef struct {
  int argc;
  char **argv;

  ExprType type;
} Expr;

typedef struct {
  int steps;
  Expr *expr;

  /* variables are created and allocated at runtime */
} Program;

void free_prog(Program p);
void err(char *fmt, ...);
void warn(char *fmt, ...);
Program trl_lex(FILE *fp);
int run(Program prog);
void setvar(char *name, Type type, char *value);
char *getvar(char *name, Type type);
Type gettype(char *t);
char *int_to_str(int);

void dbg_print_prog_tree(Program p);

void trl_print(int argc, char **argv);
void trl_nvar(int argc, char **argv);
void trl_read(int argc, char **argv);
void trl_add(int argc, char **argv);
void trl_sub(int argc, char **argv);
void trl_mul(int argc, char **argv);
void trl_div(int argc, char **argv);
void trl_mod(int argc, char **argv);
void trl_exit(int argc, char **argv);
void trl_srun(int argc, char **argv);
void trl_nowequ(int argc, char **argv);

void x_del_win();
void x_draw(int x, int y, int r, int g, int b);
void x_create_win(int X, int Y);

void trl_win_create(int argc, char **argv);
void trl_win_draw(int argc, char **argv);
void trl_win_delete(int argc, char **argv);

void trl_str_mv(int argc, char **argv);
void trl_str_fc(int argc, char **argv);
