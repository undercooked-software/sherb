
#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdint.h>

#define global static

#define i32 int32_t
#define b32 int32_t

/* Save the fingies. */
#define elif else if

#define stringify(x)  stringify_(x)
#define stringify_(x) #x

#define IS_DELIMETER(c)   (c == ',')
#define IS_TERMINATOR(c)  (c == '\0')

#endif  /* BASE_TYPES_H */
