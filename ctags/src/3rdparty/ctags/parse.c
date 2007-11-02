#include "parse.h"
#include "entry.h"
#include "vstring.h"
#include "routines.h"
#include "options.h"
#include "debug.h"
/*
*   DATA DEFINITIONS
*/
#define PARSER_LIST \
	AsmParser, \
	AspParser, \
	AwkParser, \
	BasicParser, \
	BetaParser, \
	CParser, \
	CppParser, \
	CsharpParser, \
	CobolParser, \
	EiffelParser, \
	ErlangParser, \
	FortranParser, \
	HtmlParser, \
	JavaParser, \
	JavaScriptParser, \
	LispParser, \
	LuaParser, \
	MakefileParser, \
	PascalParser, \
	PerlParser, \
	PhpParser, \
	PythonParser, \
	RexxParser, \
	RubyParser, \
	SchemeParser, \
	ShParser, \
	SlangParser, \
	SmlParser, \
	SqlParser, \
	TclParser, \
	VeraParser, \
	VerilogParser, \
	VimParser, \
	YaccParser

parserDefinitionFunc PARSER_LIST;

parserDefinitionFunc* BuiltInParsers[] = { PARSER_LIST };
parserDefinition** LanguageTable = NULL;
unsigned int LanguageCount = 0;

extern void makeSimpleTag (
		const vString* const name, kindOption* const kinds, const int kind)
{
	if (kinds [kind].enabled  &&  name != NULL  &&  vStringLength (name) > 0)
	{
	    tagEntryInfo e;
	    initTagEntry (&e, vStringValue (name));

	    e.kindName = kinds [kind].name;
	    e.kind     = kinds [kind].letter;

	    makeTagEntry (&e);
	}
}

extern parserDefinition* parserNew (const char* name)
{
	parserDefinition* result = xCalloc (1, parserDefinition);
	result->name = eStrdup (name);
	return result;
}

extern void installLanguageMapDefault (const langType language)
{
	parserDefinition* lang;
	Assert (0 <= language  &&  language < (int) LanguageCount);
	lang = LanguageTable [language];
	if (lang->currentPatterns != NULL)
		stringListDelete (lang->currentPatterns);
	if (lang->currentExtensions != NULL)
		stringListDelete (lang->currentExtensions);

	if (lang->patterns == NULL)
		lang->currentPatterns = stringListNew ();
	else
	{
		lang->currentPatterns =
			stringListNewFromArgv (lang->patterns);
	}
	if (lang->extensions == NULL)
		lang->currentExtensions = stringListNew ();
	else
	{
		lang->currentExtensions =
			stringListNewFromArgv (lang->extensions);
	}
}

extern void installLanguageMapDefaults (void)
{
	unsigned int i;
	for (i = 0  ;  i < LanguageCount  ;  ++i)
	{
		verbose ("    %s: ", getLanguageName (i));
		installLanguageMapDefault (i);
	}
}

extern void clearLanguageMap (const langType language)
{
	Assert (0 <= language  &&  language < (int) LanguageCount);
	stringListClear (LanguageTable [language]->currentPatterns);
	stringListClear (LanguageTable [language]->currentExtensions);
}
static langType getExtensionLanguage (const char *const extension)
{
	langType result = LANG_IGNORE;
	unsigned int i;
	for (i = 0  ;  i < LanguageCount  &&  result == LANG_IGNORE  ;  ++i)
	{
		stringList* const exts = LanguageTable [i]->currentExtensions;
		if (exts != NULL  &&  stringListExtensionMatched (exts, extension))
			result = i;
	}
	return result;
}

static langType getPatternLanguage (const char *const fileName)
{
	langType result = LANG_IGNORE;
	const char* base = baseFilename (fileName);
	unsigned int i;
	for (i = 0  ;  i < LanguageCount  &&  result == LANG_IGNORE  ;  ++i)
	{
		stringList* const ptrns = LanguageTable [i]->currentPatterns;
		if (ptrns != NULL  &&  stringListFileMatched (ptrns, base))
			result = i;
	}
	return result;
}

extern langType getFileLanguage (const char *const fileName) 
{
	langType language = Option.language;
	if (language == LANG_AUTO)
	{
		language = getExtensionLanguage (fileExtension (fileName));
		if (language == LANG_IGNORE)
			language = getPatternLanguage (fileName);
	}
	return language;
};

extern const char *getLanguageName (const langType language)
{
	const char* result;
	if (language == LANG_IGNORE)
		result = "unknown";
	else
	{
		Assert (0 <= language  &&  language < (int) LanguageCount);
		result = LanguageTable [language]->name;
	}
	return result;
}

extern void enableLanguage (const langType language, const boolean state)
{
	Assert (0 <= language  &&  language < (int) LanguageCount);
	LanguageTable [language]->enabled = state;
}

extern void enableLanguages (const boolean state)
{
	unsigned int i;
	for (i = 0  ;  i < LanguageCount  ;  ++i)
		enableLanguage (i, state);
}

static void initializeParsers (void)
{
	unsigned int i;
	for (i = 0  ;  i < LanguageCount  ;  ++i)
		if (LanguageTable [i]->initialize != NULL)
			(LanguageTable [i]->initialize) ((langType) i);
}

extern void initializeParsing (void)
{
	unsigned int builtInCount;
	unsigned int i;

	builtInCount = sizeof (BuiltInParsers) / sizeof (BuiltInParsers [0]);
	LanguageTable = xMalloc (builtInCount, parserDefinition*);

	verbose ("Installing parsers: ");
	for (i = 0  ;  i < builtInCount  ;  ++i)
	{
		parserDefinition* const def = (*BuiltInParsers [i]) ();
		if (def != NULL)
		{
			boolean accepted = FALSE;
			if (def->name == NULL  ||  def->name[0] == '\0')
				error (FATAL, "parser definition must contain name\n");
			else if (def->regex)
			{
#ifdef HAVE_REGEX
				def->parser = findRegexTags;
				accepted = TRUE;
#endif
			}
			else if ((def->parser == NULL)  ==  (def->parser2 == NULL))
				error (FATAL,
		"%s parser definition must define one and only one parsing routine\n",
					   def->name);
			else
				accepted = TRUE;
			if (accepted)
			{
				verbose ("%s%s", i > 0 ? ", " : "", def->name);
				def->id = LanguageCount++;
				LanguageTable [def->id] = def;
			}
		}
	}
	verbose ("\n");
	enableLanguages (TRUE);
	initializeParsers ();
}

extern void freeParserResources (void)
{
	unsigned int i;
	for (i = 0  ;  i < LanguageCount  ;  ++i)
	{
		parserDefinition* const lang = LanguageTable [i];
		freeList (&lang->currentPatterns);
		freeList (&lang->currentExtensions);
		eFree (lang->name);
		lang->name = NULL;
		eFree (lang);
	}
	if (LanguageTable != NULL)
		eFree (LanguageTable);
	LanguageTable = NULL;
	LanguageCount = 0;
}

extern langType getNamedLanguage (const char *const name)
{
	langType result = LANG_IGNORE;
	unsigned int i;
	Assert (name != NULL);
	for (i = 0  ;  i < LanguageCount  &&  result == LANG_IGNORE  ;  ++i)
	{
		const parserDefinition* const lang = LanguageTable [i];
		if (lang->name != NULL)
			if (strcasecmp (name, lang->name) == 0)
				result = i;
	}
	return result;
}
