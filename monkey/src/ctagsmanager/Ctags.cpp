/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : Ctags.cpp
** Date      : 2008-01-14T00:36:54
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
/*!
	\file Ctags.cpp
	\date 2008-01-14T00:36:54
	\author Andrei Kopats
	\brief Implementation for \link Ctags Ctags class \endlink
*/


#include "Ctags.h"

#include <QFSFileEngine>

#include <string>

extern "C" void freeSTagEntryList ( TagEntryListItem* tag );
extern "C" TagEntryListItem* parseFile ( const char* fileName, const char* langName);
extern "C" void installLanguageMapDefaults (void);
extern "C" void initializeParsing (void);
extern "C" void freeParserResources (void);

/*!
	Constructor of class
	\param parent Parent objects
*/
Ctags::Ctags (QObject* parent) :QObject (parent)
{
	initializeParsing ();
	installLanguageMapDefaults ();	
}

/*!
	Destructor of class
*/
Ctags::~Ctags ()
{
	freeParserResources ();
}

/*!
	Parse/reparse file
	
	Record for file must de already stored in the fileRecords hash
	\param file Absolute file name
	\return result of checking of file
	\retval true File was parsed/reparsed
	\retval false File is up to date
*/
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

/*!
	Get tags for file
	
	One interface of class for recieving tags from file
	\param file Absolute file name
	\return result FileRecord structure
	\retval pointer Pointer to FileRecord structure for file
	\retval NULL some error ocurred
*/

FileRecord* Ctags::GetTagsForFile (QString file )
{
	FileRecord* result = fileRecords[file];
	if ( !result )  //not parsed yet
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

/*!
	Parse file by calling ctags library function and revieve allocated TagEntryListItem list
	\note List should be freed after using by calling freeTagEntryList function
	\param file Absolute file name
	\return First item of single-linked list of tags for file
	\retval pointer Pointer to first item of list
	\retval NULL Some error ocurred
*/
TagEntryListItem* Ctags::get_tags ( QString file )
{
	return parseFile ( file.toLocal8Bit().data(), NULL);
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

/*!
	Frees memory, used by TagEntryListItem's single-linked list
	\param item Pointer to first item of list
*/
void Ctags::freeTagEntryList (TagEntryListItem* item)
{
	while ( item != NULL )
	{
		tagEntryInfo* entry = &item->tag;
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
		TagEntryListItem* temp = item->next;
		delete item;
		item = temp;
	}
}
