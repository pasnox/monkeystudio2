/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Andrei Kopats aka hlamer <hlamer@tut.by>
** Project   : Monkey Studio IDE
** FileName  : CtagsManager.h
** Date      : 2008-01-14T00:36:54
** License   : GPL
** Comment   : 
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
	\file CtagsManager.h
	\date 2008-01-14T00:36:54
	\author Andrei Kopats
	\brief Header for \link CtagsManager CtagsManager class \endlink
*/
#ifndef CTAGS_H
#define CTAGS_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QList>
#include <QVector>
#include <QApplication>

#include <fresh.h>
#include <ctags.h>

/*!
	\brief Structure containing information about already parsed file
*/
struct Q_MONKEY_EXPORT FileRecord {
	TagEntryListItem*  firstTagEntry; 	/*!< Pointer to first item of tag's list. May be NULL. 	*/
	QDateTime time; 					/*!< Time of getting of tags							*/
};

/*!
	\brief Frontend for ctags library
	
	Providing support of parsing source 
	files on different languages, and giving list of tags, containing 
	information about file contense
	See Exuberant CtagsManager project for more information about 
	possibilityes of ctags library.
*/
class Q_MONKEY_EXPORT CtagsManager: public QObject, public QSingleton<CtagsManager>
{
	Q_OBJECT
	friend class QSingleton<CtagsManager>;
private:

	CtagsManager (QObject* parent = QApplication::instance());
	~CtagsManager ();
	
	/*!
		\brief Hash, containing links to \link FileRecord \endlink structures for files
		
		ClassBrouser and other objects will use pointers to the "QString file" of FileRecord for economy of memory, 
		so, if record created it must NEVER be deleted while MkS working.
		If need free some memory, possible to free TagEntryList and set time to QDateTime::null
		
	*/
	QHash<QString, FileRecord*> fileRecords;  //records for already parsed files

	bool updateFileRecord (QString file);

	//internal function for call exuberant ctags
	TagEntryListItem* get_tags ( QString file );
	void freeTagEntryList (TagEntryListItem*);

public:
	FileRecord* GetTagsForFile (QString);
//	RecordsList* GetTagsForAllFiles ();
 	//void checkForChanges ();   //not realised yet
signals:
	// void tagUpdates ( QVector<FileRecord*> ); //not realised yet
};

#endif //CTAGS_H
