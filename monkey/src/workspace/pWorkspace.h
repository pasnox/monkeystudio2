/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : pWorkspace.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef PWORKSPACE_H
#define PWORKSPACE_H

#include "MonkeyExport.h"
#include "pExtendedWorkspace.h"

class pAbstractChild;
class QMainWindow;

class Q_MONKEY_EXPORT pWorkspace : public pExtendedWorkspace
{
	Q_OBJECT
	friend class MonkeyCore;

public:
	void loadSettings();
	pAbstractChild* currentChild() const;
	pAbstractChild* child( int ) const;
	QList<pAbstractChild*> children() const;

	// open a file and set cursor to position
	pAbstractChild* openFile( const QString& );
	// close the file
	void closeFile( const QString& );
	// goto to position inside file, highlight line according to bool, opening it if needed
	void goToLine( const QString&, const QPoint&, bool );

	void closeCurrentDocument();
	bool closeAllDocuments();

private:
	pWorkspace( QMainWindow* = 0 );

private slots:
	void internal_currentFileChanged( const QString& );
	void internal_currentChanged( int );
	//void internal_aboutToCloseTab( int, QCloseEvent* );
	void internal_urlsDropped( const QList<QUrl>& );
	void internal_listWidget_customContextMenuRequested( const QPoint& );

public slots:
	// file menu
	void fileNew_triggered();
	void fileOpen_triggered();
	void fileSessionSave_triggered();
	void fileSessionRestore_triggered();
	void fileSaveCurrent_triggered();
	void fileSaveAll_triggered();
	void fileCloseCurrent_triggered();
	void fileCloseAll_triggered();
	void fileSaveAsBackup_triggered();
	void fileQuickPrint_triggered();
	void filePrint_triggered();
	void fileExit_triggered();

	// edit menu
	void editSettings_triggered();
	void editTranslations_triggered();
	void editUndo_triggered();
	void editRedo_triggered();
	void editCut_triggered();
	void editCopy_triggered();
	void editPaste_triggered();
	void editSearchReplace_triggered();
	void editSearchPrevious_triggered();
	void editSearchNext_triggered();
	void editGoTo_triggered();
	void editExpandAbbreviation_triggered();
	void editPrepareAPIs_triggered();

	// view menu
	void agStyles_triggered( QAction* );

	// help menu
	void helpAboutApplication_triggered();
	void helpAboutQt_triggered();
#ifdef __COVERAGESCANNER__
	void helpTestReport_triggered();
#endif

signals:
	// a file has been opened
	void fileOpened( const QString& );
	// a file has been closed
	void fileClosed( const QString& );
	// current file changed
	void currentFileChanged( pAbstractChild*, const QString& );

};

#endif // PWORKSPACE_H
