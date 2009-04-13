#include "entry.h"
#include "parse.h"

/*procedure will return list for  file. Every time new list will be generated. For every generated list memory must be freed using Ctags::freeTagEntryList
*/

extern void initCtags();
extern void deInitCtags();
extern TagEntryListItem* createTagEntryListItem( const char* fileName, const char* langName );
extern void freeTagEntryListItem( TagEntryListItem* item );
extern void setLanguageTypeKinds( const langType language, const char* kinds );
extern void setLanguageKinds( const char* const language, const char* kinds );
extern const char* getFileNameLanguageName( const char* fileName );

/*
extern langType getFileLanguage (const char *const fileName)
extern const char *getLanguageName (const langType language)
*/
