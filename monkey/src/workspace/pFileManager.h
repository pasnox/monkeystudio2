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
	void currentFileChanged( const QString& );
	// projects
	void aboutToClose( ProjectItem* );
	void closed( ProjectItem* );
	void modifiedChanged( ProjectItem*, bool );
	void currentChanged( ProjectItem* );
	void opened( ProjectItem* );

};

#endif // PFILEMANAGER_H
