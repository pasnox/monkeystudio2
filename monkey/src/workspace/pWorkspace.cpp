/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pWorkspace.cpp
** Date      : 2008-01-14T00:37:21
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
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
#include "MonkeyCore.h"

#include "pChild.h"
#include "pEditor.h"
#include "pSearch.h"

#include <QToolButton>
#include <QCloseEvent>
#include <QMainWindow>
#include <QDebug>

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
	pAbstractChild* c = MonkeyCore::pluginsManager()->openChildFile( s ); // TODO: repalce by a childForFileName member witch will return a child that will open file
	
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
		connect( c, SIGNAL( modifiedChanged( bool ) ), MonkeyCore::menuBar()->action( "mFile/mSave/aCurrent" ), SLOT( setEnabled( bool ) ) );
		// update edit menu
		connect( c, SIGNAL( undoAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aUndo" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( redoAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aRedo" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( copyAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aCut" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( copyAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aCopy" ), SLOT( setEnabled( bool ) ) );
		connect( c, SIGNAL( pasteAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aPaste" ), SLOT( setEnabled( bool ) ) );
		//connect( c, SIGNAL( searchReplaceAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aSearchReplace" ), SLOT( setEnabled( bool ) ) );
		//connect( c, SIGNAL( goToAvailableChanged( bool ) ), MonkeyCore::menuBar()->action( "mEdit/aGoTo" ), SLOT( setEnabled( bool ) ) );
		// update status bar
		//connect( c, SIGNAL( cursorPositionChanged( const QPoint& ) ), statusBar(), SLOT( setCursorPosition( const QPoint& ) ) );
		//connect( c, SIGNAL( modifiedChanged( bool ) ), statusBar(), SLOT( setModified( bool ) ) );
		//connect( c, SIGNAL( documentModeChanged( AbstractChild::DocumentMode ) ), statusBar(), SLOT( setDocumentMode( AbstractChild::DocumentMode ) ) );
		//connect( c, SIGNAL( layoutModeChanged( AbstractChild::LayoutMode ) ), statusBar(), SLOT( setLayoutMode( AbstractChild::LayoutMode ) ) );
		//connect( c, SIGNAL( currentFileChanged( const QString& ) ), statusBar(), SLOT( setFileName( const QString& ) ) );
	}
	
	// add child to workspace if needed
	if ( !children().contains( c ) )
		MonkeyCore::workspace()->addDocument( c, QString() );
		//MonkeyCore::workspace()->addDocument( c, c->currentFileName() );
	
	// open file
	c->openFile( s );
	
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
	qWarning () << "File not finded "<< s;
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
	bool moreThanOneChild = count() > 1;

	// update file menu
	MonkeyCore::menuBar()->action( "mFile/mSave/aCurrent" )->setEnabled( modified );
	MonkeyCore::menuBar()->action( "mFile/mSave/aAll" )->setEnabled( hasChild );
	MonkeyCore::menuBar()->action( "mFile/mClose/aCurrent" )->setEnabled( hasChild );
	MonkeyCore::menuBar()->action( "mFile/mClose/aAll" )->setEnabled( hasChild );
	MonkeyCore::menuBar()->action( "mFile/aSaveAsBackup" )->setEnabled( hasChild );
	MonkeyCore::menuBar()->action( "mFile/aQuickPrint" )->setEnabled( print );
	MonkeyCore::menuBar()->action( "mFile/aPrint" )->setEnabled( print );

	// update edit menu
	MonkeyCore::menuBar()->action( "mEdit/aUndo" )->setEnabled( undo );
	MonkeyCore::menuBar()->action( "mEdit/aRedo" )->setEnabled( redo );
	MonkeyCore::menuBar()->action( "mEdit/aCut" )->setEnabled( copy );
	MonkeyCore::menuBar()->action( "mEdit/aCopy" )->setEnabled( copy );
	MonkeyCore::menuBar()->action( "mEdit/aPaste" )->setEnabled( paste );
	MonkeyCore::menuBar()->action( "mEdit/aSearchReplace" )->setEnabled( search );
	MonkeyCore::menuBar()->action( "mEdit/aSearchPrevious" )->setEnabled( search );
	MonkeyCore::menuBar()->action( "mEdit/aSearchNext" )->setEnabled( search );
	MonkeyCore::menuBar()->action( "mEdit/aGoTo" )->setEnabled( go );
	MonkeyCore::menuBar()->action( "mEdit/aExpandAbbreviation" )->setEnabled( hasChild );

	// update view menu
	MonkeyCore::menuBar()->action( "mView/aNext" )->setEnabled( moreThanOneChild );
	MonkeyCore::menuBar()->action( "mView/aPrevious" )->setEnabled( moreThanOneChild );

	// update status bar
	//MonkeyCore::menuBar()->setCursorPosition( c ? c->cursorPosition() : QPoint( -1, -1 ) );
	//MonkeyCore::menuBar()->setModified( c ? c->isModified() : false );
	//MonkeyCore::menuBar()->setDocumentMode( c ? c->documentMode() : AbstractChild::mNone );
	//MonkeyCore::menuBar()->setLayoutMode( c ? c->layoutMode() : AbstractChild::lNone );
	//MonkeyCore::menuBar()->setFileName( c ? c->currentFile() : QString::null );

	// search dock
	//MonkeyCore::searchDock()->setEditor( hasChild ? c->currentEditor() : 0 );
	
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
				MonkeyCore::projectsManager()->openProject( u.toLocalFile() );
	}
}

void pWorkspace::internal_listWidget_customContextMenuRequested( const QPoint& p )
{
	QMenu m;
	m.addAction( MonkeyCore::menuBar()->action( "mFile/mClose/aCurrent" ) );
	m.addAction( MonkeyCore::menuBar()->action( "mFile/mSave/aCurrent" ) );
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
	const QString mPath = currentChild() ? currentChild()->currentFile() : MonkeyCore::recentsManager()->recentFileOpenPath();

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
			MonkeyCore::recentsManager()->addRecentFile( s );
		else
			// remove it from recents files
			MonkeyCore::recentsManager()->removeRecentFile( s );
	}

	// store file open path
	if ( !l.isEmpty() )
		MonkeyCore::recentsManager()->setRecentFileOpenPath( QFileInfo( l.at( 0 ) ).canonicalPath() );
}

void pWorkspace::fileSessionSave_triggered()
{
	QStringList l;
	// saves opened files
	foreach ( pAbstractChild* c, children() )
		l << c->files();
	MonkeyCore::settings()->setValue( "Session/Files", l );
	// saves opened projects
	l.clear();
	foreach ( ProjectItem* p, MonkeyCore::projectsManager()->rootProjects() )
		l << p->canonicalFilePath();
	MonkeyCore::settings()->setValue( "Session/Projects", l );
}

void pWorkspace::fileSessionRestore_triggered()
{
	// restore files
	foreach ( QString s, MonkeyCore::settings()->value( "Session/Files", QStringList() ).toStringList() )
		if ( !openFile( s ) ) // remove it from recents files
			MonkeyCore::recentsManager()->removeRecentFile( s );
	// restore projects
	foreach ( QString s, MonkeyCore::settings()->value( "Session/Projects", QStringList() ).toStringList() )
		if ( !MonkeyCore::projectsManager()->openProject( s ) ) // remove it from recents projects
			MonkeyCore::recentsManager()->removeRecentProject( s );
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
	MonkeyCore::settings()->setValue( "MainWindow/Style", a->text() );
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
{ closeDocument( currentDocument() ); }

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

void pWorkspace::closeDocument( QWidget* document )
{
	if ( UISaveFiles::saveDocument( window(), qobject_cast<pAbstractChild*>( document ), false ) == UISaveFiles::bCancelClose )
		return;
	pExtendedWorkspace::closeDocument( document );
}
