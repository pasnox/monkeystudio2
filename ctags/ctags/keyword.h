#ifndef _KEYWORD_H
#define _KEYWORD_H
#include "parse.h"
/*
*   FUNCTION PROTOTYPES
*/
extern void addKeyword (const char *const string, langType language, int value);
extern int lookupKeyword (const char *const string, langType language);
extern void freeKeywordTable (void);

#endif  /* _KEYWORD_H */
