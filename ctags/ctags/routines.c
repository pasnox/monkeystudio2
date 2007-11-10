/*
*   $Id: routines.c,v 1.22 2006/05/30 04:37:12 darren Exp $
*
*   Copyright (c) 2002-2003, Darren Hiebert
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License.
*
*   This module contains a lose assortment of shared functions.
*/

/*
*   INCLUDE FILES
*/
#include "general.h"  /* must always come first */
# include <sys/types.h>
# include <stdlib.h>  /* to declare malloc (), realloc () */
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>  /* to declare tempnam(), and SEEK_SET (hopefully) */
#include <sys/stat.h>

#include "routines.h"
#include "debug.h"
#include "keyword.h"
#include "options.h"
#include "read.h"


/*
*   MACROS
*/

/*  File type tests.
 */
#  define S_ISREG(mode)		TRUE  /* assume regular file */ //FIXME

#ifndef S_ISLNK
# ifdef S_IFLNK
#  define S_ISLNK(mode)		(((mode) & S_IFMT) == S_IFLNK)
# else
#  define S_ISLNK(mode)		FALSE  /* assume no soft links */
# endif
#endif

#ifndef S_ISDIR
# ifdef S_IFDIR
#  define S_ISDIR(mode)		(((mode) & S_IFMT) == S_IFDIR)
# else
#  define S_ISDIR(mode)		FALSE  /* assume no soft links */
# endif
#endif

#ifndef S_IFMT
# define S_IFMT 0
#endif

#ifndef S_IXUSR
# define S_IXUSR 0
#endif
#ifndef S_IXGRP
# define S_IXGRP 0
#endif
#ifndef S_IXOTH
# define S_IXOTH 0
#endif

#ifndef S_IRUSR
# define S_IRUSR 0400
#endif
#ifndef S_IWUSR
# define S_IWUSR 0200
#endif

#ifndef S_ISUID
# define S_ISUID 0
#endif

#ifndef PATH_MAX
# define PATH_MAX 256
#endif

/*
 *  Miscellaneous macros
 */
#define selected(var,feature)	(((int)(var) & (int)(feature)) == (int)feature)

/*
*   DATA DEFINITIONS
*/
#if defined (WIN32)
const char *const PathDelimiters = ":/\\";
#endif

char *CurrentDirectory;

/*
*   FUNCTION PROTOTYPES
*/
#if defined WIN32
# define lstat(fn,buf) stat(fn,buf)
#endif


/*
*   FUNCTION DEFINITIONS
*/

extern void error (
		const errorSelection selection, const char *const format, ...)
{
	va_list ap;

	va_start (ap, format);
	fprintf (errout, "%s: %s", "ctags lib",
			selected (selection, WARNING) ? "Warning: " : "");
	vfprintf (errout, format, ap);
	if (selected (selection, PERROR))
		fprintf (errout, " : %s", strerror (errno));
	fputs ("\n", errout);
	va_end (ap);
	if (selected (selection, FATAL))
		exit (1);
}

/*
 *  Memory allocation functions
 */

extern void *eMalloc (const size_t size)
{
	void *buffer = malloc (size);

	if (buffer == NULL)
		error (FATAL, "out of memory");

	return buffer;
}

extern void *eCalloc (const size_t count, const size_t size)
{
	void *buffer = calloc (count, size);

	if (buffer == NULL)
		error (FATAL, "out of memory");

	return buffer;
}

extern void *eRealloc (void *const ptr, const size_t size)
{
	void *buffer;
	if (ptr == NULL)
		buffer = eMalloc (size);
	else
	{
		buffer = realloc (ptr, size);
		if (buffer == NULL)
			error (FATAL, "out of memory");
	}
	return buffer;
}

extern void eFree (void *const ptr)
{
	Assert (ptr != NULL);
	free (ptr);
}

/*
 *  String manipulation functions
 */

/*
 * Compare two strings, ignoring case.
 * Return 0 for match, < 0 for smaller, > 0 for bigger
 * Make sure case is folded to uppercase in comparison (like for 'sort -f')
 * This makes a difference when one of the chars lies between upper and lower
 * ie. one of the chars [ \ ] ^ _ ` for ascii. (The '_' in particular !)
 */
extern int struppercmp (const char *s1, const char *s2)
{
	int result;
	do
	{
		result = toupper ((int) *s1) - toupper ((int) *s2);
	} while (result == 0  &&  *s1++ != '\0'  &&  *s2++ != '\0');
	return result;
}

extern int strnuppercmp (const char *s1, const char *s2, size_t n)
{
	int result;
	do
	{
		result = toupper ((int) *s1) - toupper ((int) *s2);
	} while (result == 0  &&  --n > 0  &&  *s1++ != '\0'  &&  *s2++ != '\0');
	return result;
}


extern char* eStrdup (const char* str)
{
	char* result = xMalloc (strlen (str) + 1, char);
	strcpy (result, str);
	return result;
}

extern void toLowerString (char* str)
{
	while (*str != '\0')
	{
		*str = tolower ((int) *str);
		++str;
	}
}

extern void toUpperString (char* str)
{
	while (*str != '\0')
	{
		*str = toupper ((int) *str);
		++str;
	}
}

/*  Newly allocated string containing lower case conversion of a string.
 */
extern char* newLowerString (const char* str)
{
	char* const result = xMalloc (strlen (str) + 1, char);
	int i = 0;
	do
		result [i] = tolower ((int) str [i]);
	while (str [i++] != '\0');
	return result;
}

/*  Newly allocated string containing upper case conversion of a string.
 */
extern char* newUpperString (const char* str)
{
	char* const result = xMalloc (strlen (str) + 1, char);
	int i = 0;
	do
		result [i] = toupper ((int) str [i]);
	while (str [i++] != '\0');
	return result;
}

/*
 * File system functions
 */
/* For caching of stat() calls */
extern fileStatus *eStat (const char *const fileName)
{
	struct stat status;
	static fileStatus file;
	if (file.name == NULL  ||  strcmp (fileName, file.name) != 0)
	{
		eStatFree (&file);
		file.name = eStrdup (fileName);
		if (lstat (file.name, &status) != 0)
			file.exists = FALSE;
		else
		{
			file.isSymbolicLink = (boolean) S_ISLNK (status.st_mode);
			if (file.isSymbolicLink  &&  stat (file.name, &status) != 0)
				file.exists = FALSE;
			else
			{
				file.exists = TRUE;
				file.isDirectory = (boolean) S_ISDIR (status.st_mode);
				file.isNormalFile = (boolean) (S_ISREG (status.st_mode));
				file.isExecutable = (boolean) ((status.st_mode &
					(S_IXUSR | S_IXGRP | S_IXOTH)) != 0);
				file.isSetuid = (boolean) ((status.st_mode & S_ISUID) != 0);
				file.size = status.st_size;
			}
		}
	}
	return &file;
}

extern void eStatFree (fileStatus *status)
{
	if (status->name != NULL)
	{
		eFree (status->name);
		status->name = NULL;
	}
} 

extern boolean doesFileExist (const char *const fileName)
{
	fileStatus *status = eStat (fileName);
	return status->exists;
}


/*
 *  Pathname manipulation (O/S dependent!!!)
 */

static boolean isPathSeparator (const int c)
{
	boolean result;
#if defined (WIN32)
	result = (boolean) (strchr (PathDelimiters, c) != NULL);
#else
	result = (boolean) (c == PATH_SEPARATOR);
#endif
	return result;
}

extern const char *baseFilename (const char *const filePath)
{
#if defined (WIN32)
	const char *tail = NULL;
	unsigned int i;

	/*  Find whichever of the path delimiters is last.
	 */
	for (i = 0  ;  i < strlen (PathDelimiters)  ;  ++i)
	{
		const char *sep = strrchr (filePath, PathDelimiters [i]);

		if (sep > tail)
			tail = sep;
	}
#else
	const char *tail = strrchr (filePath, PATH_SEPARATOR);
#endif
	if (tail == NULL)
		tail = filePath;
	else
		++tail;  /* step past last delimiter */
	return tail;
}

extern const char *fileExtension (const char *const fileName)
{
	const char *extension;
	const char *pDelimiter = NULL;
	const char *const base = baseFilename (fileName);
	if (pDelimiter == NULL)
	    pDelimiter = strrchr (base, '.');

	if (pDelimiter == NULL)
		extension = "";
	else
		extension = pDelimiter + 1;  /* skip to first char of extension */

	return extension;
}

extern boolean isAbsolutePath (const char *const path)
{
	boolean result = FALSE;
#if defined (WIN32)
	if (isPathSeparator (path [0]))
		result = TRUE;
	else if (isalpha (path [0])  &&  path [1] == ':')
	{
		if (isPathSeparator (path [2]))
			result = TRUE;
		else
			/*  We don't support non-absolute file names with a drive
			 *  letter, like `d:NAME' (it's too much hassle).
			 */
			error (FATAL,
				"%s: relative file names with drive letters not supported",
				path);
	}
#else
	result = isPathSeparator (path [0]);
#endif
	return result;
}

extern vString *combinePathAndFile (
	const char *const path, const char *const file)
{
	vString *const filePath = vStringNew ();
	const int lastChar = path [strlen (path) - 1];
	boolean terminated = isPathSeparator (lastChar);

	vStringCopyS (filePath, path);
	if (! terminated)
	{
		vStringPut (filePath, OUTPUT_PATH_SEPARATOR);
		vStringTerminate (filePath);
	}
	vStringCatS (filePath, file);

	return filePath;
}

/* Return a newly-allocated string whose contents concatenate those of
 * s1, s2, s3.
 * Routine adapted from Gnu etags.
 */
static char* concat (const char *s1, const char *s2, const char *s3)
{
  int len1 = strlen (s1), len2 = strlen (s2), len3 = strlen (s3);
  char *result = xMalloc (len1 + len2 + len3 + 1, char);

  strcpy (result, s1);
  strcpy (result + len1, s2);
  strcpy (result + len1 + len2, s3);
  result [len1 + len2 + len3] = '\0';

  return result;
}

/* Return a newly allocated string containing the absolute file name of FILE
 * given CWD (which should end with a slash).
 * Routine adapted from Gnu etags.
 */
extern char* absoluteFilename (const char *file)
{
	char *slashp, *cp;
	char *res = NULL;
	if (isAbsolutePath (file))
	{
#ifdef WIN32
		if (file [1] == ':')
			res = eStrdup (file);
		else
		{
			char drive [3];
			sprintf (drive, "%c:", currentdrive ());
			res = concat (drive, file, "");
		}
#else
		res = eStrdup (file);
#endif
	}
	else
		res = concat (CurrentDirectory, file, "");

	/* Delete the "/dirname/.." and "/." substrings. */
	slashp = strchr (res, PATH_SEPARATOR);
	while (slashp != NULL  &&  slashp [0] != '\0')
	{
		if (slashp[1] == '.')
		{
			if (slashp [2] == '.' &&
				(slashp [3] == PATH_SEPARATOR || slashp [3] == '\0'))
			{
				cp = slashp;
				do
					cp--;
				while (cp >= res  &&  ! isAbsolutePath (cp));
				if (cp < res)
					cp = slashp;/* the absolute name begins with "/.." */
#ifdef WIN32
				/* Under MSDOS and NT we get `d:/NAME' as absolute file name,
				 * so the luser could say `d:/../NAME'. We silently treat this
				 * as `d:/NAME'.
				 */
				else if (cp [0] != PATH_SEPARATOR)
					cp = slashp;
#endif
				strcpy (cp, slashp + 3);
				slashp = cp;
				continue;
			}
			else if (slashp [2] == PATH_SEPARATOR  ||  slashp [2] == '\0')
			{
				strcpy (slashp, slashp + 2);
				continue;
			}
		}
		slashp = strchr (slashp + 1, PATH_SEPARATOR);
	}

	if (res [0] == '\0')
		return eStrdup ("/");
	else
	{
#ifdef WIN32
		/* Canonicalize drive letter case. */
		if (res [1] == ':'  &&  islower (res [0]))
			res [0] = toupper (res [0]);
#endif

		return res;
	}
}

/* Return a newly allocated string containing the absolute file name of dir
 * where `file' resides given `CurrentDirectory'.
 * Routine adapted from Gnu etags.
 */
extern char* absoluteDirname (char *file)
{
	char *slashp, *res;
	char save;
	slashp = strrchr (file, PATH_SEPARATOR);
	if (slashp == NULL)
		res = eStrdup (CurrentDirectory);
	else
	{
		save = slashp [1];
		slashp [1] = '\0';
		res = absoluteFilename (file);
		slashp [1] = save;
	}
	return res;
}

/* Return a newly allocated string containing the file name of FILE relative
 * to the absolute directory DIR (which should end with a slash).
 * Routine adapted from Gnu etags.
 */
extern char* relativeFilename (const char *file, const char *dir)
{
	const char *fp, *dp;
	char *absdir, *res;
	int i;

	/* Find the common root of file and dir (with a trailing slash). */
	absdir = absoluteFilename (file);
	fp = absdir;
	dp = dir;
	while (*fp++ == *dp++)
		continue;
	fp--;
	dp--;  /* back to the first differing char */
	do
	{  /* look at the equal chars until path sep */
		if (fp == absdir)
			return absdir;  /* first char differs, give up */
		fp--;
		dp--;
	} while (*fp != PATH_SEPARATOR);

	/* Build a sequence of "../" strings for the resulting relative file name.
	 */
	i = 0;
	while ((dp = strchr (dp + 1, PATH_SEPARATOR)) != NULL)
		i += 1;
	res = xMalloc (3 * i + strlen (fp + 1) + 1, char);
	res [0] = '\0';
	while (i-- > 0)
		strcat (res, "../");

	/* Add the file name relative to the common root of file and dir. */
	strcat (res, fp + 1);
	free (absdir);

	return res;
}

/* vi:set tabstop=4 shiftwidth=4: */
