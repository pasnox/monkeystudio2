#ifndef _OPTIONS_H
#define _OPTIONS_H

# define DEFAULT_FILE_FORMAT  2

/*
*   INCLUDE FILES
*/
#include "general.h"  /* must always come first */

#include <stdarg.h>

#include "parse.h"
#include "strlist.h"
#include "vstring.h"

/*
*   DATA DECLARATIONS
*/

typedef enum { OPTION_NONE, OPTION_SHORT, OPTION_LONG } optionType;

typedef enum eLocate {
	EX_MIX,      /* line numbers for defines, patterns otherwise */
	EX_LINENUM,  /* -n  only line numbers in tag file */
	EX_PATTERN   /* -N  only patterns in tag file */
} exCmd;

typedef enum sortType {
	SO_UNSORTED,
	SO_SORTED,
	SO_FOLDSORTED
} sortType;

struct sInclude {
	boolean fileNames;      /* include tags for source file names */
	boolean qualifiedTags;  /* include tags for qualified class members */
	boolean fileScope;      /* include tags of file scope only */
};

struct sExtFields {  /* extension field content control */
	boolean access;
	boolean fileScope;
	boolean implementation;
	boolean inheritance;
	boolean kind;
	boolean kindKey;
	boolean kindLong;
	boolean language;
	boolean lineNumber;
	boolean scope;
	boolean signature;
	boolean typeRef;
};

/*  This stores the command line options.
 */
typedef struct sOptionValues {
	struct sInclude include;/* --extra  extra tag inclusion */
	struct sExtFields extensionFields;/* --fields  extension field control */
	stringList* ignore;     /* -I  name of file containing tokens to ignore */
	boolean append;         /* -a  append to "tags" file */
	boolean backward;       /* -B  regexp patterns search backwards */
	boolean etags;          /* -e  output Emacs style tags file */
	exCmd locate;           /* --excmd  EX command used to locate tag */
	boolean recurse;        /* -R  recurse into directories */
	sortType sorted;        /* -u,--sort  sort tags */
	boolean verbose;        /* -V  verbose */
	boolean xref;           /* -x  generate xref output instead */
	char *fileList;         /* -L  name of file containing names of files */
	char *tagFileName;      /* -o  name of tags file */
	stringList* headerExt;  /* -h  header extensions */
	stringList* etagsInclude;/* --etags-include  list of TAGS files to include*/
	unsigned int tagFileFormat;/* --format  tag file format (level) */
	boolean if0;            /* --if0  examine code within "#if 0" branch */
	boolean kindLong;       /* --kind-long */
	langType language;      /* --lang specified language override */
	boolean followLinks;    /* --link  follow symbolic links? */
	boolean filter;         /* --filter  behave as filter: files in, tags out */
	char* filterTerminator; /* --filter-terminator  string to output */
	boolean tagRelative;    /* --tag-relative file paths relative to tag file */
	boolean printTotals;    /* --totals  print cumulative statistics */
	boolean lineDirectives; /* --linedirectives  process #line directives */
	boolean entryList;      /* This used in the library mode, for generate just 
								list of the entryes, without file */
#ifdef DEBUG
	long debugLevel;        /* -D  debugging output */
	unsigned long breakLine;/* -b  source line at which to call lineBreak() */
#endif
} optionValues;
/*
*   GLOBAL VARIABLES
*/
extern  optionValues		Option;

extern void verbose (const char *const format, ...) ;
extern boolean isIgnoreToken (const char *const name, boolean *const pIgnoreParens, const char **const replacement);
extern boolean isIncludeFile (const char *const fileName);
extern void freeList (stringList** const pString);

#endif  /* _OPTIONS_H */

/* vi:set tabstop=4 shiftwidth=4: */
