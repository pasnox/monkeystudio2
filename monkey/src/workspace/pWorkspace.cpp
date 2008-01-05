/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : pWorkspace.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "pWorkspace.h"
#include "pSettings.h"
#include "pAbstractChild.h"
#include "pMenuBar.h"
#include "pRecentsManager.h"
#include "pFileManager.h"
#include "UISettings.h"
#include "UITranslator.h"
#include "UISaveFiles.h"
#include "UIAbout.h"
#include "UITemplatesWizard.h"
#include "pAbbreviationsManager.h"
#include "pMonkeyStudio.h"
#include "pTemplatesManager.h"
#include "UIProjectsManager.h"
#include "PluginsManager.h"
#include "pFilesListWidget.h"

#include "pChild.h"
#include "pEditor.h"
#include "pSearch.h"

#include <QToolButton>
#include <QCloseEvent>
#include <QMainWindow>

using namespace pMonkeyStudio;

pWorkspace::pWorkspace( QMainWindow* p )
	: pExtendedWorkspace( p )
{
	Q_ASSERT( p );
	// add dock to main window
	p->addDockWidget( Qt::LeftDockWidgetArea, listWidget() );
	// set background
	//setBackground( ":/application/icons/application/background.png" );

	// set right corner button pixmap
	//cornerWidget()->findChild<QToolButton*>()->defaultAction()->setIcon( QPixmap( ":/application/icons/application/closetab.png" ) );
	
	// load settings
	loadSettings();

	// connections
	connect( this, SIGNAL( currentChanged( int ) ), this, SLOT( internal_currentChanged( int ) ) );
	/*
	connect( this, SIGNAL( aboutToCloseTab( int, QCloseEvent* ) ), this, SLOT( internal_aboutToCloseTab( int, QCloseEvent* ) ) );
	connect( this, SIGNAL( closeAllRequested() ), this, SLOT( fileCloseAll_triggered() ) );
	connect( tabBar(), SIGNAL( urlsDropped( const QList<QUrl>& ) ), this, SLOT( internal_urlsDropped( const QList<QUrl>& ) ) );
	*/
	connect( listWidget(), SIGNAL( urlsDropped( const QList<QUrl>& ) ), this, SLOT( internal_urlsDropped( const QList<QUrl>& ) ) );
	connect( listWidget(), SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( internal_listWidget_customContextMenuRequested( const QPoint& ) ) );
}

void pWorkspace::loadSettings()
{
	// restore tabs settings
	/*
	tabBar()->setTabsHaveCloseButton( tabsHaveCloseButton() );
	tabBar()->setTabsHaveShortcut( tabsHaveShortcut() );
	tabBar()->setTabsElided( tabsElided() );
	tabBar()->setTabsColor( tabsTextColor() );
	tabBar()->setCurrentTabColor( currentTabTextColor() );
	*/
	setDocMode( pMonkeyStudio::docMode() );
}

pAbstractChild* pWorkspace::currentChild() const
{ return qobject_cast<pAbstractChild*>( currentDocument() ); }

pAbstractChild* pWorkspace::child( int i ) const
{ return qobject_cast<pAbstractChild*>( document( i ) ); }

QList<pAbstractChild*> pWorkspace::children() const
{
	QList<pAbstractChild*> l;
	foreach ( QWidget* w, documents() )
		l << qobject_cast<pAbstractChild*>( w );
	return l;
}

pAbstractChild* pWorkspace::openFile( const QString& s )
{
	// if it not exists
	if ( !QFile::exists( s ) || !QFileInfo( s ).isFile() )
		return 0;
	
	// check if file is already opened
	foreach ( pAbstractChild* c, children() )
	{
		foreach ( QString f, c->files() )
		{
			if ( isSameFile( f, s ) )
			{
				setCurrentDocument( c );
				c->showFile( f );
				return c;
			}
		}
	}

	// try opening file with child plugins
	pAbstractChild* c = PluginsManager::instance()->openChildFile( s ); // TODO: repalce by a childForFileName member witch will return a child that will open file
	
	// open it with pChild instance if no c
	if ( !c )
		c = new pChild;
	
	// made connection if worksapce don t contains this child
	if ( !children().contains( c ) )
	{
		//
		connect( c, SIGNAL( currentFileChanged( const QString& ) ), this, SLOT( internal_currentFileChanged( const QString& ) ) );
		// opened/closed file
		connect( c, SIGNAL( fileOpened( const QString& ) ), this, SIGNAL( fileOpened( const QString& ) ) );
		connect( c, SIGNAL( fileClosed( const QString& ) ), this, SIGNAL( fileClosed( const QString& ) ) );
		// update file menu
		connect( c, SIGNAL( modifiedChanged( bool ) ), pMenuBar::instance()->action( "mFile/mSave/aCurrent" ), SLOT( setEnabled( bool ) ) );
		// update edit menu
		connect( c, SIGNAL( undoAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aUndo" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( redoAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aRedo" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( copyAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aCut" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( copyAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aCopy" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( pasteAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aPaste" ), SLOT( setEnabled( bool ) ) );
		//connect( c, SIGNAL( searchReplaceAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aSearchReplace" ), SLOT( setEnabled( bool ) ) );
		//connect( c, SIGNAL( goToAvailableChanged( bool ) ), pMenuBar::instance()->action( "mEdit/aGoTo" ), SLOT( setEnabled( bool ) ) );
		// update status bar
		//connect( c, SIGNAL( cursorPositionChanged( const QPoint& ) ), statusBar(), SLOT( setCursorPosition( const QPoint& ) ) );
		//connect( c, SIGNAL( modifiedChanged( bool ) ), statusBar(), SLOT( setModified( bool ) ) );
		//connect( c, SIGNAL( documentModeChanged( AbstractChild::DocumentMode ) ), statusBar(), SLOT( setDocumentMode( AbstractChild::DocumentMode ) ) );
		//connect( c, SIGNAL( layoutModeChanged( AbstractChild::LayoutMode ) ), statusBar(), SLOT( setLayoutMode( AbstractChild::LayoutMode ) ) );
		//connect( c, SIGNAL( currentFileChanged( const QString& ) ), statusBar(), SLOT( setFileName( const QString& ) ) );
	}
	
	// open file
	c->openFile( s );
	
	// add child to workspace if needed
	if ( !children().contains( c ) )
		pWorkspace::instance()->addDocument( c, c->currentFileName() );
		
	// set modification state because file is open before put in worksapce so workspace can't know it
	c->setWindowModified( c->isModified() );
	
	// set correct document if needed ( sdi hack )
	if ( currentDocument() != c )
	{
		setCurrentDocument( c );
		c->showFile( s );
	}

	// return child instance
	return c;
}

void pWorkspace::closeFile( const QString& s )
{
	foreach ( pAbstractChild* c, children() )
	{
		foreach ( QString f, c->files() )
		{
			if ( isSameFile( f, s ) )
			{
				c->closeFile( f );
				return;
			}
		}
	}
}

void pWorkspace::goToLine( const QString& s, const QPoint& p, bool b )
{
	if ( b )
		openFile( s );
	foreach ( pAbstractChild* c, children() )
	{
		foreach ( QString f, c->files() )
		{
			if ( isSameFile( f, s ) )
			{
				setCurrentDocument( c );
				c->goTo( s, p, b );
				return;
			}
		}
	}
}

void pWorkspace::internal_currentFileChanged( const QString& )
{ internal_currentChanged( indexOf( qobject_cast<pAbstractChild*>( sender() ) ) ); }

void pWorkspace::internal_currentChanged( int i )
{
	// get child
	pAbstractChild* c = child( i );
	bool hasChild = c;
	bool modified = hasChild ? c->isModified() : false;
	bool print = hasChild ? c->isPrintAvailable() : false;
	bool undo = hasChild ? c->isUndoAvailable() : false;
	bool redo = hasChild ? c->isRedoAvailable() : false;
	bool copy = hasChild ? c->isCopyAvailable() : false;
	bool paste = hasChild ? c->isPasteAvailable() : false;
	bool search = hasChild ? c->isSearchReplaceAvailable() : false;
	bool go = hasChild ? c->isGoToAvailable() : false;

	// update file menu
	pMenuBar::instance()->action( "mFile/mSave/aCurrent" )->setEnabled( modified );
	pMenuBar::instance()->action( "mFile/mSave/aAll" )->setEnabled( hasChild );
	pMenuBar::instance()->action( "mFile/mClose/aCurrent" )->setEnabled( hasChild );
	pMenuBar::instance()->action( "mFile/mClose/aAll" )->setEnabled( hasChild );
	pMenuBar::instance()->action( "mFile/aSaveAsBackup" )->setEnabled( hasChild );
	pMenuBar::instance()->action( "mFile/aQuickPrint" )->setEnabled( print );
	pMenuBar::instance()->action( "mFile/aPrint" )->setEnabled( print );

	// update edit menu
	pMenuBar::instance()->action( "mEdit/aUndo" )->setEnabled( undo );
	pMenuBar::instance()->action( "mEdit/aRedo" )->setEnabled( redo );
	pMenuBar::instance()->action( "mEdit/aCut" )->setEnabled( copy );
	pMenuBar::instance()->action( "mEdit/aCopy" )->setEnabled( copy );
	pMenuBar::instance()->action( "mEdit/aPaste" )->setEnabled( paste );
	pMenuBar::instance()->action( "mEdit/aSearchReplace" )->setEnabled( search );
	pMenuBar::instance()->action( "mEdit/aSearchPrevious" )->setEnabled( search );
	pMenuBar::instance()->action( "mEdit/aSearchNext" )->setEnabled( search );
	pMenuBar::instance()->action( "mEdit/aGoTo" )->setEnabled( go );
	pMenuBar::instance()->action( "mEdit/aExpandAbbreviation" )->setEnabled( hasChild );

	// update view menu
	pMenuBar::instance()->action( "mView/aNext" )->setEnabled( hasChild );
	pMenuBar::instance()->action( "mView/aPrevious" )->setEnabled( hasChild );

	// update status bar
	//pMenuBar::instance()->setCursorPosition( c ? c->cursorPosition() : QPoint( -1, -1 ) );
	//pMenuBar::instance()->setModified( c ? c->isModified() : false );
	//pMenuBar::instance()->setDocumentMode( c ? c->documentMode() : AbstractChild::mNone );
	//pMenuBar::instance()->setLayoutMode( c ? c->layoutMode() : AbstractChild::lNone );
	//pMenuBar::instance()->setFileName( c ? c->currentFile() : QString::null );

	// search dock
	pSearch::instance()->setEditor( hasChild ? c->currentEditor() : 0 );
	
	// update item tooltip
	if ( hasChild )
		listWidget()->setItemToolTip( i, c->currentFile() );
	
	// emit file changed
	emit currentFileChanged( c, hasChild ? c->currentFile() : QString() );
}
/*
void pWorkspace::internal_aboutToCloseTab( int i, QCloseEvent* e )
{
	if ( UISaveFiles::saveDocument( window(), child( i ), false ) == UISaveFiles::bCancelClose )
		e->ignore();
}
*/
void pWorkspace::internal_urlsDropped( const QList<QUrl>& l )
{
	// create menu
	QMenu m( this );
	QAction* aof = m.addAction( tr( "Open As &File" ) );
	QAction* aop = m.addAction( tr( "Open As &Project" ) );
	m.addSeparator();
	m.addAction( tr( "Cancel" ) );
	
	// execute menu
	QAction* a = m.exec( QCursor::pos() );
	
	// check triggered action
	if ( a == aof )
	{
		foreach ( QUrl u, l )
			if ( !u.toLocalFile().trimmed().isEmpty() )
				openFile( u.toLocalFile() );
	}
	else if ( a == aop )
	{
		foreach ( QUrl u, l )
			if ( !u.toLocalFile().trimmed().isEmpty() )
				UIProjectsManager::instance()->openProject( u.toLocalFile() );
	}
}

void pWorkspace::internal_listWidget_customContextMenuRequested( const QPoint& p )
{
	QMenu m;
	m.addAction( pMenuBar::instance()->action( "mFile/mClose/aCurrent" ) );
	m.addAction( pMenuBar::instance()->action( "mFile/mSave/aCurrent" ) );
	m.exec( listWidget()->mapToGlobal( p ) );
}

// file menu
void pWorkspace::fileNew_triggered()
{
	UITemplatesWizard* d = UITemplatesWizard::instance( this );
	d->setType( tr( "Files" ) );
	d->exec();
}

void pWorkspace::fileOpen_triggered()
{
	// get last file open path
	const QString mPath = currentChild() ? currentChild()->currentFile() : pRecentsManager::instance()->recentFileOpenPath();

	// get available filters
	QString mFilters = availableFilesFilters();

	// prepend a all in one filter
	if ( !mFilters.isEmpty() )
	{
		QString s;
		foreach ( QStringList l, availableFilesSuffixes().values() )
			s.append( l.join( " " ).append( " " ) );
		mFilters.prepend( QString( "All Supported Files (%1);;" ).arg( s.trimmed() ) );
	}

	// open open file dialog
	QStringList l = getOpenFileNames( tr( "Choose the file(s) to open" ), mPath, mFilters, window() );

	// for each entry, open file
	foreach ( QString s, l )
	{
		if ( openFile( s ) )
			// append file to recents
			pRecentsManager::instance()->addRecentFile( s );
		else
			// remove it from recents files
			pRecentsManager::instance()->removeRecentFile( s );
	}

	// store file open path
	if ( !l.isEmpty() )
		pRecentsManager::instance()->setRecentFileOpenPath( QFileInfo( l.at( 0 ) ).canonicalPath() );
}

void pWorkspace::fileSessionSave_triggered()
{
	QStringList l;
	// saves opened files
	foreach ( pAbstractChild* c, children() )
		l << c->files();
	pSettings::instance()->setValue( "Session/Files", l );
	// saves opened projects
	l.clear();
	foreach ( ProjectItem* p, UIProjectsManager::instance()->rootProjects() )
		l << p->canonicalFilePath();
	pSettings::instance()->setValue( "Session/Projects", l );
}

void pWorkspace::fileSessionRestore_triggered()
{
	// restore files
	foreach ( QString s, pSettings::instance()->value( "Session/Files", QStringList() ).toStringList() )
		if ( !openFile( s ) ) // remove it from recents files
			pRecentsManager::instance()->removeRecentFile( s );
	// restore projects
	foreach ( QString s, pSettings::instance()->value( "Session/Projects", QStringList() ).toStringList() )
		if ( !UIProjectsManager::instance()->openProject( s ) ) // remove it from recents projects
			pRecentsManager::instance()->removeRecentProject( s );
}

void pWorkspace::fileSaveCurrent_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->saveCurrentFile();
}

void pWorkspace::fileSaveAll_triggered()
{
	foreach ( pAbstractChild* c, children() )
		c->saveFiles();
}

void pWorkspace::fileCloseCurrent_triggered()
{ closeCurrentDocument(); }

void pWorkspace::fileCloseAll_triggered()
{
	closeAllDocuments();
	/*
	// try save documents
	UISaveFiles::Buttons cb = UISaveFiles::saveDocuments( window(), children(), b );
	
	// close all object, disconnecting them
	if ( cb != UISaveFiles::bCancelClose )
		closeAllTabs( b, true );
	*/
}

void pWorkspace::fileSaveAsBackup_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
	{
		const QString s = getSaveFileName( tr( "Choose a filename to backup your file" ), QFileInfo( c->currentFile() ).fileName(), QString::null, this );
	
		if ( !s.isEmpty() )
			c->backupCurrentFile( s );
	}
}

void pWorkspace::fileQuickPrint_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->quickPrintCurrentFile();
}

void pWorkspace::filePrint_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->printCurrentFile();
}

void pWorkspace::fileExit_triggered()
{ window()->close(); }

// edit menu
void pWorkspace::editSettings_triggered()
{ UISettings::instance( this )->exec(); }

void pWorkspace::editTranslations_triggered()
{
	if ( UITranslator::instance( this )->exec() )
		loadTranslations();
}

void pWorkspace::editUndo_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->undo();
}

void pWorkspace::editRedo_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->redo();
}

void pWorkspace::editCut_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->cut();
}

void pWorkspace::editCopy_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->copy();
}

void pWorkspace::editPaste_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->paste();
}

void pWorkspace::editSearchReplace_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->searchReplace();
}

void pWorkspace::editSearchPrevious_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->searchPrevious();
}

void pWorkspace::editSearchNext_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->searchNext();
}

void pWorkspace::editGoTo_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		c->goTo();
}

void pWorkspace::editExpandAbbreviation_triggered()
{
	pAbstractChild* c = currentChild();
	if ( c )
		pAbbreviationsManager::expandAbbreviation( c->currentEditor() );
}

void pWorkspace::editPrepareAPIs_triggered()
{ prepareAPIs(); }

// view menu
void pWorkspace::agStyles_triggered( QAction* a )
{
	qApp->setStyle( a->text() );
	qApp->setPalette( qApp->style()->standardPalette() );
	pSettings::instance()->setValue( "MainWindow/Style", a->text() );
}

// help menu
void pWorkspace::helpAboutApplication_triggered()
{ UIAbout::instance( this )->exec(); }

void pWorkspace::helpAboutQt_triggered()
{ qApp->aboutQt(); }

#ifdef __COVERAGESCANNER__
void pWorkspace::helpTestReport_triggered()
{ UITestReport::instance( this )->exec(); }
#endif

void pWorkspace::closeCurrentDocument()
{
	if ( UISaveFiles::saveDocument( window(), currentChild(), false ) == UISaveFiles::bCancelClose )
		return;
	pExtendedWorkspace::closeCurrentDocument();
}

bool pWorkspace::closeAllDocuments()
{
	// try save documents
	UISaveFiles::Buttons cb = UISaveFiles::saveDocuments( window(), children(), false );
	// close all object, disconnecting them
	if ( cb != UISaveFiles::bCancelClose )
	{
		pExtendedWorkspace::closeAllDocuments();
		return true;
	}
	else
		return false; //not close IDE
}
