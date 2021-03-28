#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void
fail (char *fmt, ...) {
  va_list args;
  va_start (args, fmt);
  fprintf (stderr, fmt, args);
  va_end (args);

  exit (1);
}
