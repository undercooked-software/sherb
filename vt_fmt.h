
#ifndef VT_FMT_H
#define VT_FMT_H

/* NOTE (sammynilla): Below link is info how to use VT100 formatting.
 * http://xn--rpa.cc/irl/term.html
 */

/* NOTE (sammynilla):
 * col formula is 16 + 36r + 6g + b where rgb are integers between 0 and 5.
 * indicies 232 through 255 are a grayscale ramp.
 */
#define ANSI_ESC    "\x1b"
#define FGCOL(col)  "38;5;" #col

#ifdef VT_ENABLE
# define FMT(style)  ANSI_ESC "[" style "m"
#else
# define FMT(style) ""
#endif

/* Styles */
#define CORNFLOWER  FGCOL(111)
#define PEA         FGCOL(113)
#define PINK        FGCOL(211)
#define RED         FGCOL(124)
#define YELLOW      FGCOL(011)
#define PLAIN       ";"

#endif /* VT_FMT_H */
