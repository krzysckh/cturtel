#include "turtel.h"

#include <stdlib.h>
#include <string.h>

int var_pt;
Variable *var;

Type gettype(char *t) {
  if (strcmp(t, "num") == 0) return Num;
  if (strcmp(t, "str") == 0) return Str;
  if (strcmp(t, "tof") == 0) return Tof;
  
  err("runtime: unknown type %s", t);
  /* will die :( */
  return Num;
}

void freevar() {
  int i;
  extern int var_pt;
  extern Variable *var;

  for (i = 0; i < var_pt; i++) {
    free(var[i].content);
    free(var[i].name);
  }
  
  free(var);
}

char *getvar(char *name, Type type) {
  int i;
  extern int var_pt;
  extern Variable *var;

  for (i = 0; i < var_pt; i++) {
    if (var[i].type == type)
      if (strcmp(var[i].name, name) == 0)
        return var[i].content;
  }
  
  /* does not exist */
  err("variable %s doesn't exist", name);
  return NULL;
}

void setvar(char *name, Type type, char *value) {
  int i;
  extern int var_pt;
  extern Variable *var;

  for (i = 0; i < var_pt; i++) {
    if (var[i].type == type)
      if (strcmp(var[i].name, name) == 0) {
        free(var[i].content);

        var[i].content = malloc(sizeof(char) * (strlen(value) + 1));
        strcpy(var[i].content, value);
        var[i].content[strlen(value)] = '\0';
        return;
      }
  }

  var[var_pt].name = malloc(sizeof(char) * (strlen(name)+1));
  var[var_pt].content = malloc(sizeof(char) * (strlen(value)+1));
  var[var_pt].type = type;

  strcpy(var[var_pt].name, name);
  var[var_pt].name[strlen(name)] = '\0';
  strcpy(var[var_pt].content, value);
  var[var_pt].content[strlen(value)] = '\0';

  var_pt++;
}

int run(Program prog) {
  extern Variable *var;
  char *nl_ghetto[] = {"__newline", "str", "\n"};
  int i,
      n_var_approx = 0,
      gototag_found,
      gototag_back;
  /*extern int var_p;*/
  var_pt = 0; /* curr *var stack end ptr */

  for (i = 0; i < prog.steps; i++) {
    if (prog.expr[i].type == NVAR || prog.expr[i].type == READ)
      n_var_approx ++;
  }

  n_var_approx ++;
  var = malloc(sizeof(Variable) * n_var_approx);
  trl_nvar(3, nl_ghetto); /* weird flex but ok */


  i = 0;
  while (i < prog.steps) {
    /*printf("%d\n", prog.expr[i].type);*/
    switch (prog.expr[i].type) {
      case IF: {
        if (prog.expr[i].argc != 5 && prog.expr[i].argc != 4) {
          err("runtime: error using builtin if: expected 5 || 4 arguments, "
              "got %d", 
            prog.expr[i].argc);
        }
        

        int a = atoi(getvar(prog.expr[i].argv[0], Num)),
            b = atoi(getvar(prog.expr[i].argv[2], Num));
        char *op = prog.expr[i].argv[1],
             *goto_if = NULL;


        if (strcmp(op, "eq") == 0) {
          if (a == b)
            goto_if = prog.expr[i].argv[3];
          else
            goto_if = prog.expr[i].argv[4];
        } else if (strcmp(op, "lt") == 0) {
          if (a < b)
            goto_if = prog.expr[i].argv[3];
          else
            goto_if = prog.expr[i].argv[4];
        } else if (strcmp(op, "gt") == 0) {
          if (a > b)
            goto_if = prog.expr[i].argv[3];
          else
            goto_if = prog.expr[i].argv[4];
        } else if (strcmp(op, "ne") == 0) {
          if (a != b)
            goto_if = prog.expr[i].argv[3];
          else
            goto_if = prog.expr[i].argv[4];
        } else {
          err("runtime: invalid if operand \"%s\"", op);
        }

        if ((int)strlen(goto_if) == 0) {
          i ++;
          continue;
        }

        /** copy-pasted from goto **/

        gototag_found = 0;
        gototag_back = i;
        for (; i > 0 && !gototag_found; i--) {
          if (prog.expr[i].type == GOTOTAG && 
              prog.expr[i].argc > 0 &&
              strcmp(prog.expr[i].argv[0], goto_if) \
              == 0) {

              gototag_found = 1;
          }
        }
      
        if (gototag_found) {
          i ++;
          continue;
        }

        i = gototag_back;
        for (; i < prog.steps && !gototag_found; i++) {
          if (prog.expr[i].type == GOTOTAG) 
            if (prog.expr[i].argc > 0)
              if (strcmp(prog.expr[i].argv[0], 
                  goto_if) == 0)
                gototag_found = 1;
        }

        if (!gototag_found)
          err("runtime: \"%s\": no such gototag",
              goto_if);
        continue;
        break;
      case GOTOTAG:
        if (prog.expr[i].argc != 1) {
          err("runtime: error using builtin gototag: expected 1 argument, "
            "got %d", 
            prog.expr[i].argc);
        }
               }
        break;
      case GOTO:
        if (prog.expr[i].argc != 1) {
          err("runtime: error using builtin goto: expected 1 argument, got %d", 
            prog.expr[i].argc);
        }

        gototag_found = 0;
        gototag_back = i;
        for (; i >= 0 && !gototag_found; i--) {
          if (prog.expr[i].type == GOTOTAG)
            if (prog.expr[i].argc > 0)
              if (strcmp(prog.expr[i].argv[0], prog.expr[gototag_back].argv[0])
                  == 0) {

              gototag_found = 1;
          }
        }

      
        if (gototag_found) {
          i ++;
          continue;
        }

        i = gototag_back;
        for (; i < prog.steps && !gototag_found; i++) {
          if (prog.expr[i].type == GOTOTAG) 
            if (prog.expr[i].argc > 0)
              if (strcmp(prog.expr[i].argv[0], 
                  prog.expr[gototag_back].argv[0]) == 0)
                gototag_found = 1;
        }

        if (!gototag_found)
          err("runtime: \"%s\": no such gototag",
              prog.expr[gototag_back].argv[0]);
        continue;
        break;
      case PRINT:
        trl_print(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case NVAR:
        trl_nvar(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case READ:
        trl_read(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case ADD:
        trl_add(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case SUB:
        trl_sub(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case MUL:
        trl_mul(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case DIV:
        trl_div(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case MOD:
        trl_mod(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case NOWEQU:
        trl_nowequ(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case EXIT:
        trl_exit(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case SRUN:
        trl_srun(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case NOOP:
        break;
      case WIN_CREATE:
        trl_win_create(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case WIN_DRAW:
        trl_win_draw(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case WIN_DELETE:
        trl_win_delete(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case STR_MV:
        trl_str_mv(prog.expr[i].argc, prog.expr[i].argv);
        break;
      case STR_FC:
        trl_str_fc(prog.expr[i].argc, prog.expr[i].argv);
        break;
      default:
        err("fatal");
    }
    i ++;
  }

  freevar();
  return 0;
}
