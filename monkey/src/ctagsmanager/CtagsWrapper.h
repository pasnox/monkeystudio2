/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : CtagsWrapper.h
** Date      : 2007-11-04T22:45:07
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef CTAGS_H
#define CTAGS_H
#include <QObject>
#include <QDateTime>
#include <QString>
#include <QList>
#include<QVector>
#include "MonkeyExport.h"
#include "QSingleton.h"
#include "entry.h"


/*record in the table, in wich are file names and pointers to the tag list for it*/

struct Q_MONKEY_EXPORT FileRecord {
	tagEntryListItem*  firstTagEntry;
	QDateTime time;
};

class Q_MONKEY_EXPORT CtagsWrapper: public QObject, public QSingleton<Ctags>
{
	Q_OBJECT
	friend class QSingleton<CtagsWrapper>;
private:
	CtagsWrapper ();
	~CtagsWrapper ();
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
	tagEntryListItem* get_tags ( QString file );
	void freeTagEntryList (tagEntryListItem*);

public:
	FileRecord* GetTagsForFile (QString);
//	RecordsList* GetTagsForAllFiles ();
 	//void checkForChanges ();   //not realised yet
signals:
	// void tagUpdates ( QVector<FileRecord*> ); //not realised yet
};

#endif //CTAGS_H
