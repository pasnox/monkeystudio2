/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pRecentsManager.h
** Date      : 2007-11-04T22:46:18
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef PRECENTSMANAGER_H
#define PRECENTSMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

class QAction;

class Q_MONKEY_EXPORT pRecentsManager : public QObject, public QSingleton<pRecentsManager>
{
	Q_OBJECT
	friend class QSingleton<pRecentsManager>;

public:
	int maxRecentFiles() const;
	int maxRecentProjects() const;

	QString recentFileOpenPath() const;
	QString recentProjectOpenPath() const;

private:
	pRecentsManager( QObject* = 0 );

	QList<QAction*> mRecentFiles;
	QList<QAction*> mRecentProjects;

public slots:
	void setMaxRecentFiles( int );
	void setMaxRecentProjects( int );

	void setRecentFileOpenPath( const QString& );
	void setRecentProjectOpenPath( const QString& );

	void recentFiles_triggered( QAction* );
	void updateRecentFiles();
	void addRecentFile( const QString& );
	void removeRecentFile( const QString& );

	void recentProjects_triggered( QAction* );
	void updateRecentProjects();
	void addRecentProject( const QString& );
	void removeRecentProject( const QString& );

signals:
	void openFileRequested( const QString& );
	void openProjectRequested( const QString& );

};

#endif // PRECENTSMANAGER_H
