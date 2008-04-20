/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : Ctags.h
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

/*record in the table, in wich are file names and pointers to the tag list for it*/

struct Q_MONKEY_EXPORT FileRecord {
	TagEntryListItem*  firstTagEntry;
	QDateTime time;
};

class Q_MONKEY_EXPORT Ctags: public QObject, public QSingleton<Ctags>
{
	Q_OBJECT
	friend class QSingleton<Ctags>;
private:
	Ctags (QObject* parent = QApplication::instance());
	~Ctags ();
	/* ClassBrouser and other objects will use pointers to the "QString file" of FileRecord for economy of memory, 
	*so, if record created it must NEVER be deleted while monkeyDS working.
	If need free some memory, possible to free TagEntryList and set time to QDateTime::null
	*/
	QHash<QString, FileRecord*> fileRecords;  //records for already parsed files

	//reparse file if need, or parse first time
	//returns true if record was parsed/reparsed, or false, if record up to date
	//record for file must already exist in the hash
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
