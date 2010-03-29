#ifndef EXUBERANT_CTAGS_H
#define EXUBERANT_CTAGS_H
#include "entry.h"
#include "parse.h"

/*
	procedure will return list for  file. Every time new list will be generated.
	For every generated list memory must be freed using Ctags::freeTagEntryList
	The const char* returned by languageTypeKinds & languageKinds must be freed by the caller.
*/

extern void initCtags();
extern void deInitCtags();
extern TagEntryListItem* createTagEntryListItem( const char* fileName, const char* langName );
extern void freeTagEntryListItem( TagEntryListItem* item );
extern void setLanguageTypeKinds( const langType language, const char* kinds );
extern void setLanguageKinds( const char* const language, const char* kinds );
extern const char* languageTypeKinds( const langType language );
extern const char* languageKinds( const char* const language );
extern const char* getFileNameLanguageName( const char* fileName );
extern int languageCount();
extern const parserDefinition* parser( const langType language );

/*
extern langType getFileLanguage (const char *const fileName)
extern const char *getLanguageName (const langType language)
*/
#endif // EXUBERANT_CTAGS_H
