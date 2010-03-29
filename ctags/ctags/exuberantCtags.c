#include "general.h"
#include "string.h"
#include "read.h"
#include "entry.h"

#include <assert.h>

extern parserDefinition** LanguageTable;
extern unsigned int LanguageCount;
static int mIsInitializedRefCount = 0;

extern void initCtags()
{
	if ( mIsInitializedRefCount == 0 ) {
		initializeParsing();
		installLanguageMapDefaults();
	}
	
	mIsInitializedRefCount++;
}

extern void deInitCtags()
{
	mIsInitializedRefCount--;
	
	assert( mIsInitializedRefCount >= 0 );
	
	if ( mIsInitializedRefCount == 0 ) {
		freeParserResources();
	}
}

extern TagEntryListItem* createTagEntryListItem( const char* fileName, const char* langName )
{
	if ( !mIsInitializedRefCount ) {
		return NULL;
	}
	
	const parserDefinition* language;
	langType lang; 
	unsigned int passCount;
	boolean retried;

	firstTagEntry = NULL; // generate new list
	
	if ( ( langName != NULL ) && ( strlen( langName ) != 0 ) ) {
		lang = getNamedLanguage( langName );
	}
	else {
		lang = getFileLanguage( fileName );
	}
	
	if ( lang <= 0 ) {
		//printf( "Will not parse %s\n",fileName );
		return NULL;
	}
	
	language = LanguageTable[ lang ];
	
	if ( language == NULL ) {
		return NULL;
	}
	
	if ( !fileOpen( fileName, lang ) ) {
		return NULL;
	}
	
	passCount = 0;
	retried = FALSE;
	
	if ( language->parser != NULL ) {
		language->parser();
	}
	else if ( language->parser2 != NULL ) {
		do {
			retried = language->parser2( ++passCount );
		} while ( retried );
	}
	
	return firstTagEntry;
}

extern void freeTagEntryListItem( TagEntryListItem* item )
{
	while ( item != NULL ) {
		TagEntryListItem* temp = 0;
		tagEntryInfo* entry = &item->tag;
		
		if ( entry->language )
			free( (char*)entry->language );
		
		if ( entry->sourceFileName )
			free( (char*)entry->sourceFileName );
		
		if ( entry->name)
			free( (char*)entry->name );
		
		if ( entry->kindName )
			free( (char*)entry->kindName );
		
		if ( entry->extensionFields.access )
			free( (char*)entry->extensionFields.access );
		
		if ( entry->extensionFields.fileScope )
			free( (char*)entry->extensionFields.fileScope );
		
		if ( entry->extensionFields.implementation )
			free( (char*)entry->extensionFields.implementation  );
		
		if ( entry->extensionFields.inheritance )
			free( (char*)entry->extensionFields.inheritance );
		
		if ( entry->extensionFields.scope[0] )
			free( (char*)entry->extensionFields.scope[0] );
		
		if ( entry->extensionFields.scope[1] )
			free( (char*)entry->extensionFields.scope[1] );
		
		if ( entry->extensionFields.typeRef[0] )
			free( (char*)entry->extensionFields.typeRef[0] );
		
		if ( entry->extensionFields.typeRef[1] )
			free( (char*)entry->extensionFields.typeRef[1] );
		
		if ( entry->extensionFields.signature )
			free( (char*)entry->extensionFields.signature );
		
		temp = item->next;
		free( item );
		item = temp;
	}
}

extern void setLanguageTypeKinds( const langType language, const char* kinds )
{
	if ( !mIsInitializedRefCount ) {
		return;
	}
	
	parserDefinition* parser = LanguageTable[ language ];
	
	if ( parser != NULL ) {
		unsigned int i;
		
		for ( i = 0; i < parser->kindCount; ++i ) {
			parser->kinds[ i ].enabled = strchr( kinds, parser->kinds[ i ].letter ) != NULL;
		}
	}
}

extern void setLanguageKinds( const char *const language, const char* kinds )
{
	setLanguageTypeKinds( getNamedLanguage( language ), kinds );
}

extern const char* languageTypeKinds( const langType language )
{
	parserDefinition* parser = LanguageTable[ language ];
	
	if ( parser != NULL ) {
		char _kinds[ parser->kindCount +1 ];
		unsigned int i;
		int j =  0;
		
		for ( i = 0; i < parser->kindCount; ++i ) {
			if ( parser->kinds[ i ].enabled ) {
				_kinds[ j++ ] = parser->kinds[ i ].letter;
			}
		}
		
		_kinds[ j ] = '\0';
		
		return strdup( _kinds );
	}
	
	return NULL;
}

extern const char* languageKinds( const char* const language )
{
	return languageTypeKinds( getNamedLanguage( language ) );
}

extern const char* getFileNameLanguageName( const char* fileName )
{
	return getLanguageName( getFileLanguage( fileName ) );
}

extern int languageCount()
{
	return LanguageCount;
}

extern const parserDefinition* parser( const langType language )
{
	return LanguageTable[ language ];
}
