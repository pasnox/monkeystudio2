#ifndef  _GENERAL_H
#define  _GENERAL_H
#include <stdbool.h>
#include "string.h"

# define errout	stderr
# define __unused__  __attribute__((unused))
#  define strcasecmp(s1,s2) strcmp(s1,s2)  //possible source of bugs!!!!


#include <stdlib.h>
#include "ctype.h" //need for parsers

typedef bool boolean;
#define FALSE false
#define TRUE true

#endif  /* _GENERAL_H */
