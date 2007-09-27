#ifndef PFILEMANAGER_H
#define PFILEMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QObject>
#include <QApplication>

class pAbstractChild;

class Q_MONKEY_EXPORT pFileManager : public QObject, public QSingleton<pFileManager>
{
	Q_OBJECT
	friend class QSingleton<pFileManager>;
	friend class pWorkspace;

public:
	pFileManager( QObject* = QApplication::instance() );

public slots:
	// open a file and set cursor to position for proxy
	pAbstractChild* openFile( const QString&, const QPoint& = QPoint() );
	// close file
	void closeFile( const QString& );
	// goto to position inside file, highlight line according to bool, opening it if needed
	void goToLine( const QString&, const QPoint&, bool );
	// open a project
	void openProject( const QString& );
	// close a project
	void closeProject( const QString& );

signals:
	// a file has been opened
	void fileOpened( const QString& );
	// a file has been closed
	void fileClosed( const QString& );
	// current file changed
	void currentFileChanged( const QString& );
/*
	// a file is selected in the project manager view
	void fileSelected( const QString&, pAbstractProjectProxy* );
	// breakpoint changed
	void breakPointChanged( const QString&, int, bool, pAbstractProjectProxy* );
	// a project has been opened
	void projectOpened( const QString&, pAbstractProjectProxy* );
	// a project has been closed
	void projectClosed( const QString&, pAbstractProjectProxy* );
	// about to close a project
	void aboutToCloseProject( const QString&, pAbstractProjectProxy* );
*/

};

#endif // PFILEMANAGER_H
