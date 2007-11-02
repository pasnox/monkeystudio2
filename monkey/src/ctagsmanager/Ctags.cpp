#include <QFSFileEngine>
#include <string>

#include "Ctags.h"

extern "C" void freeSTagEntryList ( sTagEntryListItem* tag );
extern "C" sTagEntryListItem* parseFile ( const char* fileName, const char* langName);
extern "C" void installLanguageMapDefaults (void);
extern "C" void initializeParsing (void);
extern "C" void freeParserResources (void);

Ctags::Ctags ()
{
	initializeParsing ();
	installLanguageMapDefaults ();	
}


Ctags::~Ctags ()
{
	freeParserResources ();
}

bool Ctags::updateFileRecord (QString file) //reparse file if need, or parse first time
{
	FileRecord* record = fileRecords[file];
	Q_ASSERT (record); //just already existing record can be updated
	QFSFileEngine fsEng (file);
	QDateTime fileTime = fsEng.fileTime ( QAbstractFileEngine::ModificationTime );

	if ( record->time >= fileTime )  //record up to date, not need parse
		return false;
	if ( record->firstTagEntry ) //exist old list, need to free it
			freeTagEntryList ( record->firstTagEntry);

	record->firstTagEntry = get_tags (file);
	record->time = fileTime;
	return true;
}

FileRecord* Ctags::GetTagsForFile (QString file )
{
	FileRecord* result = fileRecords[file];
	if ( not result )  //not parsed yet
	{
		result = new FileRecord;
		result->firstTagEntry = NULL;
		result->time = QDateTime(); //null Time
		fileRecords[file] =result;
	}		
	//ok,  now we can fill record, if it's new, or update, if if's already exist
	updateFileRecord (file);
	return result;
}

sTagEntryListItem* Ctags::get_tags ( QString file )
{
	return parseFile ( file.toStdString().c_str(), NULL);
}

//RecordsList* Ctags::GetTagsForAllFiles ()
//{
// 	checkForChanges ();
// 	return &records;
//}

//void Ctags::checkForChanges ()
//{
// 	QVector<FileRecord*> updatedRecords;
//  	for ( int i = 0; i< records.size(); i++)
// 	{
// 		FileRecord* precord = records.data()+i ; //pointer to records[i]
// 		bool reparsed = updateRecord (  precord ) ;  
// 		if ( reparsed )
// 			updatedRecords.append (precord);
// 	}
// 	if ( updatedRecords.size() >0 )
// 		emit  tagUpdates ( updatedRecords );
//}


void Ctags::freeTagEntryList (sTagEntryListItem* item)
{
	while ( item != NULL )
	{
		sTagEntryInfo* entry = &item->tag;
		if (entry->language)
			free ( (void*)entry->language );
		if ( entry->sourceFileName)
			free ( (void*)entry->sourceFileName );
		if ( entry->name)
			free ( (void*)entry->name );
		if ( entry->kindName)
			free ( (void*)entry->kindName );
		if ( entry->extensionFields.access)
		{
			free ( (void*)entry->extensionFields.access );
		}
		if ( entry->extensionFields.fileScope)
		{
			free ( (void*)entry->extensionFields.fileScope );
		}
		if ( entry->extensionFields.implementation)
		{
			free ( (void*)entry->extensionFields.implementation  );
		}
		if ( entry->extensionFields.inheritance)
		{
			free ( (void*)entry->extensionFields.inheritance );
		}
		if ( entry->extensionFields.scope[0])
		{
			free ( (void*)entry->extensionFields.scope[0] );
		}
		if ( entry->extensionFields.scope[1])
		{
			free (  (void*)entry->extensionFields.scope[1] );
		}
		if ( entry->extensionFields.typeRef[0])
		{
			free ( (void*)entry->extensionFields.typeRef[0] );
		}
		if ( entry->extensionFields.typeRef[1])
		{
			free ( (void*)entry->extensionFields.typeRef[1] );
		}
		if ( entry->extensionFields.signature)
		{
			free ( (void*)entry->extensionFields.signature );
		}
		sTagEntryListItem* temp = item->next;
		delete item;
		item = temp;
	}
}