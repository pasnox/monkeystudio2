#ifndef  _PARSE_H
#define  _PARSE_H

#include <stdio.h>
#include "vstring.h"
#include "entry.h"
#include "strlist.h"

#define KIND_COUNT(kindTable) (sizeof(kindTable)/sizeof(kindOption))

#define LANG_AUTO   (-1)
#define LANG_IGNORE (-2)

typedef int langType;

typedef void (*createRegexTag) (const vString* const name);
typedef void (*simpleParser) (void);
typedef boolean (*rescanParser) (const unsigned int passCount);
typedef void (*parserInitialize) (langType language);

typedef struct {
	size_t start;   /* character index in line where match starts */
	size_t length;  /* length of match */
} regexMatch;
typedef void (*regexCallback) (const char *line, const regexMatch *matches, unsigned int count);

typedef struct sKindOption {
	boolean enabled;          /* are tags for kind enabled? */
	int letter;               /* kind letter */
	const char* name;         /* kind name */
	const char* description;  /* displayed in --help output */
} kindOption;

typedef struct {
	/* defined by parser */
	char* name;                    /* name of language */
	kindOption* kinds;             /* tag kinds handled by parser */
	unsigned int kindCount;        /* size of `kinds' list */
	const char *const *extensions; /* list of default extensions */
	const char *const *patterns;   /* list of default file name patterns */
	parserInitialize initialize;   /* initialization routine, if needed */
	simpleParser parser;           /* simple parser (common case) */
	rescanParser parser2;          /* rescanning parser (unusual case) */
	boolean regex;                 /* is this a regex parser? */

	/* used internally */
	unsigned int id;               /* id assigned to language */
	boolean enabled;               /* currently enabled? */
	stringList* currentPatterns;   /* current list of file name patterns */
	stringList* currentExtensions; /* current list of extensions */
} parserDefinition;

typedef parserDefinition* (parserDefinitionFunc) (void);

extern parserDefinitionFunc PARSER_LIST;

extern parserDefinition** LanguageTable;

/* Legacy interface */
extern boolean includingDefineTags (void);

/* Language processing and parsing */
extern void makeSimpleTag (const vString* const name, kindOption* const kinds, const int kind);
extern parserDefinition* parserNew (const char* name);
extern void addTagRegex (const langType language, const char* const regex, const char* const name, const char* const kinds, const char* const flags);
extern const char *getLanguageName (const langType language);
extern langType getFileLanguage (const char *const fileName) ;
extern langType getNamedLanguage (const char *const name);
extern void installLanguageMapDefault (const langType language);
extern void installLanguageMapDefaults (void);
extern void initializeParsing (void);
extern void freeParserResources (void);

#endif // _PARSE_H
