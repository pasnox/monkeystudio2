#include "entry.h"
#include "string.h"
#include "parse.h"
#include "read.h"

extern sTagEntryListItem* parseFile ( const char* fileName, const char* langName)
{
	firstTagEntry = NULL; // generate new list
	langType lang; 
	if ( (langName != NULL ) && (strlen ( langName) != 0 ))
		lang = getNamedLanguage ( langName );
	else
		lang = getFileLanguage ( fileName) ;
	if ( lang  <=0)
	{
		printf ( "Will not parse %s\n",fileName);
		return NULL;
	}
	const parserDefinition* const language = LanguageTable [lang];
	if ( language == NULL)
		return NULL;
	if (!fileOpen (fileName, lang))
		return NULL;
	
	unsigned int passCount = 0;
	boolean retried = FALSE;
	if (language->parser != NULL)
		language->parser ();
	else if (language->parser2 != NULL)
		do
		{
			retried = language->parser2 (++passCount);
		}while ( retried);
	return firstTagEntry;
}
