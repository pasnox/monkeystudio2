#include "general.h"
#include "entry.h"
#include "string.h"
#include "parse.h"
#include "read.h"

extern TagEntryListItem* parseFile ( const char* fileName, const char* langName)
{
	const parserDefinition*  language;
	langType lang; 
	unsigned int passCount;
	boolean retried;

	firstTagEntry = NULL; // generate new list
	if ( (langName != NULL ) && (strlen ( langName) != 0 ))
		lang = getNamedLanguage ( langName );
	else
		lang = getFileLanguage ( fileName) ;
	if ( lang  <=0)
	{
		printf ( "Will not parse %s\n",fileName);
		return NULL;
	}
	language = LanguageTable [lang];
	if ( language == NULL)
		return NULL;
	if (!fileOpen (fileName, lang))
		return NULL;
	
	passCount = 0;
	retried = FALSE;
	if (language->parser != NULL)
		language->parser ();
	else if (language->parser2 != NULL)
		do
		{
			retried = language->parser2 (++passCount);
		}while ( retried);
	return firstTagEntry;
}
