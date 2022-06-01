#include "turtel.h"

#include <stdarg.h>
#include <stdlib.h>

void err(char *fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  fprintf(stderr, "error: ");
  vfprintf(stderr, fmt, vl);
  fprintf(stderr, "\n");
  va_end(vl);
  exit(1);
}

void warn(char *fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  fprintf(stderr, "warning: ");
  vfprintf(stderr, fmt, vl);
  fprintf(stderr, "\n");
  va_end(vl);
}
