#include "general.h"
#include "string.h"
#include "read.h"
#include "entry.h"

extern parserDefinition** LanguageTable;

extern void initCtags()
{
	initializeParsing();
	installLanguageMapDefaults();
}

extern void deInitCtags()
{
	freeParserResources();
}

extern TagEntryListItem* createTagEntryListItem( const char* fileName, const char* langName )
{
	const parserDefinition* language;
	langType lang; 
	unsigned int passCount;
	boolean retried;

	firstTagEntry = NULL; // generate new list
	
	if ( ( langName != NULL ) && ( strlen( langName ) != 0 ) )
	{
		lang = getNamedLanguage( langName );
	}
	else
	{
		lang = getFileLanguage( fileName );
	}
	
	if ( lang <= 0 )
	{
		//printf( "Will not parse %s\n",fileName );
		return NULL;
	}
	
	language = LanguageTable[ lang ];
	
	if ( language == NULL )
	{
		return NULL;
	}
	
	if ( !fileOpen( fileName, lang ) )
	{
		return NULL;
	}
	
	passCount = 0;
	retried = FALSE;
	
	if ( language->parser != NULL )
	{
		language->parser();
	}
	else if ( language->parser2 != NULL )
	{
		do
		{
			retried = language->parser2( ++passCount );
		} while ( retried );
	}
	
	return firstTagEntry;
}

extern void freeTagEntryListItem( TagEntryListItem* item )
{
	while ( item != NULL )
	{
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
	parserDefinition* parser = LanguageTable[ language ];
	
	if ( parser != NULL )
	{
		unsigned int i;
		
		for ( i = 0; i < parser->kindCount; ++i )
		{
			parser->kinds[ i ].enabled = strchr( kinds, parser->kinds[ i ].letter ) != NULL;
		}
	}
}

extern void setLanguageKinds( const char *const language, const char* kinds )
{
	setLanguageTypeKinds( getNamedLanguage( language ), kinds );
}

extern const char* getFileNameLanguageName( const char* fileName )
{
	return getLanguageName( getFileLanguage( fileName ) );
}
