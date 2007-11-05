/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pFileManager.h
** Date      : 2007-11-04T22:46:58
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PFILEMANAGER_H
#define PFILEMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QObject>
#include <QApplication>

class pAbstractChild;
class ProjectItem;

class Q_MONKEY_EXPORT pFileManager : public QObject, public QSingleton<pFileManager>
{
	Q_OBJECT
	friend class QSingleton<pFileManager>;
	friend class pWorkspace;

public:
	pFileManager( QObject* = QApplication::instance() );
	ProjectItem* currentProject() const;
	QString currentProjectFile() const;
	QString currentProjectPath() const;
	pAbstractChild* currentChild() const;
	QString currentChildFile() const;
	QString currentChildPath() const;
	ProjectItem* currentItem() const;
	QString currentItemFile() const;
	QString currentItemPath() const;

public slots:
	pAbstractChild* openFile( const QString&, const QPoint& = QPoint() );
	void closeFile( const QString& );
	void goToLine( const QString&, const QPoint&, bool );
	void openProject( const QString& );
	void closeProject( const QString& );

signals:
	// files
	void fileOpened( const QString& );
	void fileClosed( const QString& );
	void currentFileChanged( pAbstractChild*, const QString& );
	// projects
	void aboutToClose( ProjectItem* );
	void closed( ProjectItem* );
	void modifiedChanged( ProjectItem*, bool );
	void currentChanged( ProjectItem* );
	void opened( ProjectItem* );

};

#endif // PFILEMANAGER_H
