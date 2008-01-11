#ifndef PRECENTSMANAGER_H
#define PRECENTSMANAGER_H

#include "MonkeyExport.h"

#include <QObject>

class QAction;

class Q_MONKEY_EXPORT pRecentsManager : public QObject
{
	Q_OBJECT
	friend class MonkeyCore;

public:
	int maxRecentFiles() const;
	int maxRecentProjects() const;

	QString recentFileOpenPath() const;
	QString recentProjectOpenPath() const;

protected:
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
