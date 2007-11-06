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
