#ifndef _GET_H
#define _GET_H

/*
*   INCLUDE FILES
*/
#include "general.h"  /* must always come first */


#define isident(c)  (isalnum(c) || (c) == '_' || (c) == '$')

/*  Is the character valid as the first character of a C identifier?
 *  C++ allows '~' in destructors.
 *  VMS allows '$' in identifiers.
 */
#define isident1(c)  (isalpha(c) || (c) == '_' || (c) == '~' || (c) == '$')

/*
*   FUNCTION PROTOTYPES
*/
extern boolean isBraceFormat (void);
extern unsigned int getDirectiveNestLevel (void);
extern void cppInit (const boolean state, const boolean hasAtLiteralStrings);
extern void cppTerminate (void);
extern void cppBeginStatement (void);
extern void cppEndStatement (void);
extern void cppUngetc (const int c);
extern int cppGetc (void);
extern int skipOverCComment (void);

#endif  /* _GET_H */

/* vi:set tabstop=4 shiftwidth=4: */
