#ifndef  _GENERAL_H
#define  _GENERAL_H

# define errout	stderr

# define strncasecmp(s1,s2,n) strnuppercmp(s1,s2,n)
# define strcasecmp(s1,s2) strcmp(s1,s2)  //possible source of bugs!!!!


#if defined (WIN32)

# if defined (_MSC_VER)
#  define currentdrive() (_getdrive() + 'A' - 1)
#  define _CRT_SECURE_NO_DEPRECATE
#  define PATH_MAX  _MAX_PATH

# elif defined (__BORLANDC__)
#  define PATH_MAX  MAXPATH
#  define currentdrive() (getdisk() + 'A')

# elif defined (DJGPP)
#  define currentdrive() (getdisk() + 'A')

# else
#  define currentdrive() 'C'
# endif
#endif

#ifndef boolean
# define boolean int
#endif
#ifndef FALSE
# define FALSE 0
#endif
#ifndef TRUE
# define TRUE 1
#endif

/*  This is a helpful internal feature of later versions (> 2.7) of GCC
 *  to prevent warnings about unused variables.
 */
#ifdef __GNUC__
# define __unused__  __attribute__((unused))
# define __printf__(s,f)  __attribute__((format (printf, s, f)))
#else
# define __unused__
# define __printf__(s,f)
#endif

//includes for parsers
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include "routines.h"

#ifdef WIN32
# include <direct.h>
#endif

/*  Hack for rediculous practice of Microsoft Visual C++.
 */
#if defined (WIN32)
# if defined (_MSC_VER)

#  define stricmp _stricmp
#  define stat    _stat
#  define getcwd  _getcwd
# endif
#endif




#endif  /* _GENERAL_H */
