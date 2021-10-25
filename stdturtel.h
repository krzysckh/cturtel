/* standard functions (keywords) */

#define PRINT "print"
/* 0 */
#define READ "read"
/* 1 */
#define IF "if"
/* 2 */
#define SRUN "srun"
/* 3 */
#define EXIT "exit"
/* 4 */
#define GOTO "goto"
/* 5 */
#define GOTOTAG "gototag"
/* 6 */
#define ADD "add"
/* 7 */
#define SUB "sub"
/* 8 */
#define MUL "mul"
/* 9 */
#define DIV "div"
/* a */
#define NOWEQU "nowequ"
/* b */
#define MOD "mod"
/* c */

/* standard constants */
char *NEWLINE[] = {"__newline", "\n"};
char *SPACE[] = {"__space", " "};

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
char *OS[] = {"__OS", "1"};
#else
char *OS[] = {"__OS", "0"};
#endif

/* data structures */
#define NUM "num"
/* a */
#define STR "str"
/* b */
#define TOF "tof"
/* c */

#define SEPARATOR ':'


/* (for interpreter):
 * new: N
 */

/* statements for if */
#define EQ "eq"
/* = */
#define LESSTHAN "lt"
/* < */
#define GREATERTHAN "gt"
/* > */
#define NOTEQ "ne"
/* ! */


/* lexer definitions */
/* lexer statements start with @, so they can be easily distinguished */

#define LEX_INCLUDE "@include"
#define LEX_NEWMACRO "@defmacro"
#define LEX_ENDMACRO "@endmacro"
#define LEX_RUNMACRO "@run"

/* illegal chars */
char ILLEGAL_C[] = {';', '[', ']', '#'};
