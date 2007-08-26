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
#include "pTabbedWorkspaceCorner.h"
#include "pFileManager.h"
#include "UISettings.h"
#include "UISaveFiles.h"
#include "UIAbout.h"
#include "UITemplatesWizard.h"
#include "pAbbreviationsManager.h"
#include "pMonkeyStudio.h"

#include "pChild.h"
#include "pEditor.h"
#include "pSearch.h"

#include <QToolButton>
#include <QCloseEvent>

pWorkspace::pWorkspace( QWidget* p )
	: pTabbedWorkspace( p )
{
	// set background
	setBackground( ":/application/icons/application/background.png" );

	// set right corner button pixmap
	cornerWidget()->findChild<QToolButton*>()->defaultAction()->setIcon( QPixmap( ":/application/icons/application/closetab.png" ) );

	// connections
	connect( this, SIGNAL( currentChanged( int ) ), this, SLOT( internal_currentChanged( int ) ) );
	connect( this, SIGNAL( aboutToCloseTab( int, QCloseEvent* ) ), this, SLOT( internal_aboutToCloseTab( int, QCloseEvent* ) ) );
}

pAbstractChild* pWorkspace::currentChild() const
{
	return qobject_cast<pAbstractChild*>( currentDocument() );
}

pAbstractChild* pWorkspace::child( int i ) const
{
	return qobject_cast<pAbstractChild*>( document( i ) );
}

QList<pAbstractChild*> pWorkspace::children() const
{
	QList<pAbstractChild*> l;
	foreach ( QWidget* w, documents() )
		l << qobject_cast<pAbstractChild*>( w );
	return l;
}

void pWorkspace::internal_currentChanged( int i )
{
	// get child
	pAbstractChild* c = child( i );
	bool ic = c;
	bool modified = ic ? c->isModified() : false;
	bool print = ic ? c->isPrintAvailable() : false;
	bool undo = c ? c->isUndoAvailable() : false;
	bool redo = c ? c->isRedoAvailable() : false;
	bool copy = c ? c->isCopyAvailable() : false;
	bool paste = c ? c->isPasteAvailable() : false;
	bool search = c ? c->isSearchReplaceAvailable() : false;
	bool go = c ? c->isGoToAvailable() : false;

	// update file menu
	pMenuBar::instance()->action( "mFile/mSave/aCurrent" )->setEnabled( modified );
	pMenuBar::instance()->action( "mFile/mSave/aAll" )->setEnabled( ic );
	pMenuBar::instance()->action( "mFile/mClose/aCurrent" )->setEnabled( ic );
	pMenuBar::instance()->action( "mFile/mClose/aAll" )->setEnabled( ic );
	//pMenuBar::instance()->action( "mFile/aSaveAsTemplate" )->setEnabled( false );
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
	pMenuBar::instance()->action( "mEdit/aExpandAbbreviation" )->setEnabled( ic );

	// update view menu
	pMenuBar::instance()->action( "mView/aNext" )->setEnabled( ic );
	pMenuBar::instance()->action( "mView/aPrevious" )->setEnabled( ic );

	// update status bar
	//pMenuBar::instance()->setCursorPosition( c ? c->cursorPosition() : QPoint( -1, -1 ) );
	//pMenuBar::instance()->setModified( c ? c->isModified() : false );
	//pMenuBar::instance()->setDocumentMode( c ? c->documentMode() : AbstractChild::mNone );
	//pMenuBar::instance()->setLayoutMode( c ? c->layoutMode() : AbstractChild::lNone );
	//pMenuBar::instance()->setFileName( c ? c->currentFile() : QString::null );

	// left corner widget
	//

	// search dock
	pSearch::instance()->setEditor( ic ? c->currentEditor() : 0 );
	
	// emit file changed
	emit pFileManager::instance()->currentFileChanged( ic ? c->currentFile() : QString() );
}

void pWorkspace::internal_aboutToCloseTab( int i, QCloseEvent* e )
{
	// get child
	pAbstractChild* c = child( i );

	// check if need to save files
	UISaveFiles::execute( c, e );
}

// file menu
void pWorkspace::fileNew_triggered()
{
	UITemplatesWizard::instance( this )->exec();
}

void pWorkspace::fileOpen_triggered()
{
	// get last file open path
	const QString mPath = pRecentsManager::instance()->recentFileOpenPath();

	// get available filters
	QString mFilters = pMonkeyStudio::availableLanguagesFilters();

	// prepend a all in one filter
	if ( !mFilters.isEmpty() )
	{
		QString s;
		foreach ( QStringList l, pMonkeyStudio::availableSuffixes().values() )
			s.append( l.join( " " ).append( " " ) );
		mFilters.prepend( QString( "All Supported Files (%1);;" ).arg( s.trimmed() ) );
	}

	// open open file dialog
	QStringList l = pMonkeyStudio::getOpenFileNames( tr( "Choose the file(s) to open" ), mPath, mFilters, window() );

	// for each entry, open file
	foreach ( QString s, l )
	{
		if ( pFileManager::instance()->openFile( s ) )
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
	foreach ( pAbstractChild* c, children() )
		l << c->files();

	pSettings::instance()->setValue( "Session", l );
}

void pWorkspace::fileSessionRestore_triggered()
{
	foreach ( QString s, pSettings::instance()->value( "Session", QStringList() ).toStringList() )
		if ( !pFileManager::instance()->openFile( s ) ) // remove it from recents files
			pRecentsManager::instance()->removeRecentFile( s );
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
{
	closeCurrentTab();
}

void pWorkspace::fileCloseAll_triggered( bool b )
{
	closeAllTabs( b );
}

void pWorkspace::fileSaveAsTemplate_triggered()
{
	qWarning( "Not yet implemented." );
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
{
	window()->close();
}

// edit menu
void pWorkspace::editSettings_triggered()
{
	UISettings::instance( this )->exec();
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

// view menu
void pWorkspace::agStyles_triggered( QAction* a )
{
	qApp->setStyle( a->text() );
	qApp->setPalette( qApp->style()->standardPalette() );
	pSettings::instance()->setValue( "MainWindow/Style", a->text() );
}

// project menu
void pWorkspace::projectNew_triggered()
{
	//projectsManager()->newProject();
}

void pWorkspace::projectOpen_triggered()
{
	//projectsManager()->openProject();
}

void pWorkspace::projectSaveCurrent_triggered()
{
	//projectsManager()->saveCurrent();
}

void pWorkspace::projectSaveAll_triggered()
{
	//projectsManager()->saveAll();
}

void pWorkspace::projectCloseCurrent_triggered()
{
	//projectsManager()->closeCurrent();
}

void pWorkspace::projectCloseAll_triggered()
{
	//projectsManager()->closeAll();
}

void pWorkspace::projectSettings_triggered()
{
	//projectsManager()->projectSettings();
}

// help menu
void pWorkspace::helpAboutApplication_triggered()
{
	UIAbout::instance( this )->exec();
}

void pWorkspace::helpAboutQt_triggered()
{
	qApp->aboutQt();
}

#ifdef __COVERAGESCANNER__
void pWorkspace::helpTestReport_triggered()
{
	UITestReport::instance( this )->exec();
}
#endif
