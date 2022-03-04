
#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#define global    static
#define internal  static

#define i32 int
#define i64 long long
#define b32 i32

/* Save the fingies. */
#define elif else if

#define stringify(x)  stringify_(x)
#define stringify_(x) #x

#define IS_DELIMETER(c)   (c == ',')
#define IS_TERMINATOR(c)  (c == '\0')

#endif  /* BASE_TYPES_H */
