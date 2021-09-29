/* standard functions (keywords) */

#define PRINT "print"
/* 0 */
#define READ "read"
/* 1 */
#define IF "if"
/* 2 */
#define EQ "eq"
/* 3 */
#define LESSTHAN "lt"
/* 4 */
#define GREATERTHAN "gt"
/* 6 */
#define ADD "add"
/* 7 */
#define SUB "sub"
/* 8 */


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
