
#ifndef TERMINAL_H
#define TERMINAL_H

#include "base_types.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define TERM_SET_ATTR(attr) \
  SetConsoleTextAttribute(term.output_handle, attr)

struct Terminal {
  HANDLE  output_handle;
  WORD    base_style;
};

/* Attributes */
/* NOTE (sammynilla): This enum is laid out based on accumulative values.
 * FOREGROUND_BLUE: 0x0001, FOREGROUND_GREEN: 0x0002, FOREGROUND_RED: 0x0004
 * FOREGROUND_INTENSITY: 0x0008
 */
enum FG_COLORS {
  FG_BLACK = 0,
  FG_BLUE,          /* 0x0001 */
  FG_GREEN,         /* 0x0002 */
  FG_CYAN,          /* 0x0001 | 0x0002 */
  FG_RED,           /* 0x0004 */
  FG_MAGENTA,       /* 0x0001 | 0x0004 */
  FG_BROWN,         /* 0x0002 | 0x0004 */
  FG_LIGHTGRAY,     /* 0x0001 | 0x0002 | 0x0004 */
  FG_GRAY,          /* 0x0008 */
  FG_LIGHTBLUE,     /* 0x0001 | 0x0008 */
  FG_LIGHTGREEN,    /* 0x0002 | 0x0008 */
  FG_LIGHTCYAN,     /* 0x0001 | 0x0002 | 0x0008 */
  FG_LIGHTRED,      /* 0x0004 | 0x0008 */
  FG_LIGHTMAGENTA,  /* 0x0001 | 0x0004 | 0x0008 */
  FG_YELLOW,        /* 0x0002 | 0x0004 | 0x0008 */
  FG_WHITE,         /* 0x0001 | 0x0002 | 0x0004 | 0x0008 */

  FG_RESET,
};

global struct Terminal term;

/* Our print functions have no intentions of ever using wide strings. */
internal i32 term_puts(LPCSTR str) {
  return WriteConsoleA(term.output_handle, str, lstrlenA(str), 0, 0);
}

/* Color-format puts */
internal i32 term_cfputs(i32 argc, ...) {
/* FIXME (sammynilla): There is currently an issue with multiple FG_RESET's.
 * The function works fine if there is an FG_RESET anywhere in the arguments,
 * and at least one other FG_RESET at the end.
 * Consecutive FG_RESET's too close together end up not being processed??
 *
 * eg: term_cfputs(8, color, text, reset, text, color, text, reset, text);
 *                     THIS ARGUMENT ISN'T PROCESSED PROPERLY (^)
 */
  i32 args;
  i64 mask;
  void *p = {0};
  va_list vl;
  
  va_start(vl, argc);
  args = 0;
  while (args < argc) {
    p = va_arg(vl, void *);

    mask = (i64)p;
    if (mask < FG_RESET)
      TERM_SET_ATTR((WORD)mask);
    elif (mask == FG_RESET)
      TERM_SET_ATTR(term.base_style);

    term_puts(p);
    args++;
  }
  va_end(vl);

  return 0;
}

#endif /* TERMINAL_H */
