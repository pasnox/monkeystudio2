#ifndef PFILEMANAGER_H
#define PFILEMANAGER_H

#include "MonkeyExport.h"

#include <QObject>
#include <QPoint>

class pAbstractChild;
class ProjectItem;

class Q_MONKEY_EXPORT pFileManager : public QObject
{
	Q_OBJECT
	friend class MonkeyCore;
	friend class pWorkspace;

public:
	ProjectItem* currentProject() const;
	QString currentProjectFile() const;
	QString currentProjectPath() const;
	pAbstractChild* currentChild() const;
	QString currentChildFile() const;
	QString currentChildPath() const;
	ProjectItem* currentItem() const;
	QString currentItemFile() const;
	QString currentItemPath() const;

protected:
	pFileManager( QObject* = 0 );

public slots:
	pAbstractChild* openFile( const QString& );
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
