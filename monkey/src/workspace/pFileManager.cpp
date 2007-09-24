#include "pFileManager.h"
#include "pWorkspace.h"
#include "pRecentsManager.h"
#include "pAbstractChild.h"
#include "pMenuBar.h"

#include "pChild.h"

#include <QFileInfo>

pFileManager::pFileManager( QObject* o )
	: QObject( o )
{}

pAbstractChild* pFileManager::openFile( const QString& s, const QPoint& p )
{
	// if it not exists
	if ( !QFile::exists( s ) )
		return 0;
	
	// check if file is already opened
	QFileInfo f( s );
	foreach ( pAbstractChild* c, pWorkspace::instance()->children() )
	{
		foreach ( QFileInfo fi, c->files() )
		{
			if ( fi.canonicalFilePath() == f.canonicalFilePath() )
			{
				pWorkspace::instance()->setCurrentDocument( c );
				c->goTo( s, p );
				return c;
			}
		}
	}

	// open file
/*
	if ( pluginsManager()->childPluginOpenFile( s, p ) )
	{}
	else
*/
	{
		// create child
		pAbstractChild* c = new pChild;

		// opened/closed file
		connect( c, SIGNAL( fileOpened( const QString& ) ), this, SIGNAL( fileOpened( const QString& ) ) );
		connect( c, SIGNAL( fileClosed( const QString& ) ), this, SIGNAL( fileClosed( const QString& ) ) );
		// update file menu
		connect( c, SIGNAL( modifiedChanged( bool ) ), pMenuBar::instance()->action( "mFile/mSave/aCurrent" ), SLOT( setEnabled( bool ) ) );
		//update tab icon
		connect( c, SIGNAL( modifiedChanged( bool ) ), pWorkspace::instance(), SLOT( setCurrentTabModified( bool ) ) );
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

		// open file
		c->openFile( s, p );

		// add child to workspace
		pWorkspace::instance()->addTab( c, c->currentFileName() );
		
		// set modification state because file is open before put in worksapce so workspace can't know it
		c->setWindowModified( c->isModified() );

		return c;
	}

	return 0;
}

void pFileManager::closeFile( const QString& s )
{
	// search child for this file
	foreach ( pAbstractChild* c, pWorkspace::instance()->children() )
	{
		if ( c->files().contains( s ) )
		{
			c->closeFile( s );
			return;
		}
	}
}

void pFileManager::goToLine( const QString& s, const QPoint& p, bool b )
{
	// search child for this file
	foreach ( pAbstractChild* c, pWorkspace::instance()->children() )
	{
		if ( c->files().contains( s ) )
		{
			c->goTo( s, p, b );
			return;
		}
	}

	// open file
	pAbstractChild* c = openFile( s, p );

	// goto line
	if ( c )
		c->goTo( s, p, b );
}

/*
void pFileManager::setBreakPoint( const QString& s, int i, bool b, pAbstractProjectProxy* p )
{
	emit breakPointChanged( s, i, b, 0 );
}

void pFileManager::openProject( const QString& s )
{
	emit projectOpened( s, 0 );
}

void pFileManager::closeProject( const QString& s )
{
	emit projectClosed( s, 0 );
	emit aboutToCloseProject( s, 0 );
}
*/
