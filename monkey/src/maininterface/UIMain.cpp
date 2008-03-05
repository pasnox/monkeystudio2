/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UIMain.cpp
** Date      : 2008-01-14T00:36:56
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
#include "UIMain.h"
#include "MonkeyCore.h"
#include "pMonkeyStudio.h"
#include "UIProjectsManager.h"
#include "pDockToolBar.h"
#include "pMenuBar.h"
#include "pRecentsManager.h"
#include "pToolsManager.h"
#include "pSettings.h"
#include "pConsoleManager.h"
#include "pRecentsManager.h"
#include "pFileManager.h"
#include "pActionManager.h"
#include "ProjectsProxy.h"
#include "PluginsManager.h"
#include "pSearch.h"

#include <QCloseEvent>
#include <QMenu>
#include <QStyleFactory>

UIMain::UIMain( QWidget* p )
	: pMainWindow( p )
{
	setCorner (Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
	setCorner (Qt::TopRightCorner, Qt::RightDockWidgetArea);
}

void UIMain::initGui()
{
	// init menubar
	initMenuBar();
	// init recents manager
	MonkeyCore::recentsManager();
	// init tools manager
	MonkeyCore::toolsManager();
	// init toolbar
	initToolBar();
	// init projects manager
	dockToolBar( Qt::LeftToolBarArea )->addDock( MonkeyCore::projectsManager(), MonkeyCore::projectsManager()->windowTitle(), QIcon( ":/project/icons/project/project.png" ) );
	// init workspace
	setCentralWidget( MonkeyCore::workspace() );
	// init search dock
//	addDockWidget( Qt::RightDockWidgetArea, MonkeyCore::searchDock() );
//	MonkeyCore::searchDock()->setVisible( false );
//	MonkeyCore::searchDock()->setFloating( true );
	// init staus bar
	setStatusBar( MonkeyCore::statusBar() );
	// init connection
	initConnections();
	// init final gui
	setWindowTitle( QObject::tr( "%1 v%2 - %3 & The Monkey Studio Team" ).arg( PROGRAM_NAME, PROGRAM_VERSION, PROGRAM_COPYRIGHTS ) );
	setUnifiedTitleAndToolBarOnMac( true );
	setWindowIcon( menuBar()->action( "mHelp/aAbout" )->icon() );
	setIconSize( QSize( 16, 16 ) );
}

void UIMain::closeEvent( QCloseEvent* e )
{
	// save session if needed
	if ( pMonkeyStudio::saveSessionOnClose() )
		MonkeyCore::workspace()->fileSessionSave_triggered();
	// request close all documents
	if ( !MonkeyCore::workspace()->closeAllDocuments() )
	{
		e->ignore();
		return;
	}
	// force to close all projects
	MonkeyCore::projectsManager()->projectCloseAll_triggered();
	// inform that we close mainwindow
	emit aboutToClose();
}

QMenu* UIMain::createPopupMenu()
{
	// create default menu
	QMenu* m = QMainWindow::createPopupMenu();
	// add exclusive action of pDockToolBar
	QList<pDockToolBar*> l;
	if ( ( l = findChildren<pDockToolBar*>() ).count() )
	{
		m->addSeparator();
		foreach ( pDockToolBar* tb, l )
			m->addAction( tb->toggleExclusiveAction() );
	}
	// return menu
	return m;
}

void UIMain::initMenuBar()
{
	// create menubar menus and actions
	pMenuBar* mb = menuBar();
	mb->menu( "mFile", tr( "&File" ) );
	mb->beginGroup( "mFile" );
		mb->action( "aNew", tr( "&New..." ), QIcon( ":/file/icons/file/new.png" ), tr( "Ctrl+N" ), tr( "Create a new file" ) );
		mb->action( "aOpen", tr( "&Open..." ), QIcon( ":/file/icons/file/open.png" ), tr( "Ctrl+O" ), tr( "Open a file" ) );
		mb->menu( "mRecents", tr( "&Recents" ), QIcon( ":/file/icons/file/recents.png" ) );
		mb->action( "mRecents/aClear", tr( "&Clear" ), QIcon( ":/file/icons/file/clear.png" ), QString::null, tr( "Clear the recents files list" ) );
		mb->action( "mRecents/aSeparator1" );
		mb->action( "aSeparator1" );
		mb->menu( "mSession", tr( "Session" ), QIcon( ":/file/icons/file/session.png" ) );
		mb->action( "mSession/aSave", tr( "Save" ), QIcon( ":/file/icons/file/save.png" ), QString::null, tr( "Save the current session files list" ) );
		mb->action( "mSession/aRestore", tr( "Restore" ), QIcon( ":/file/icons/file/restore.png" ), QString::null, tr( "Restore the current session files list" ) );
		mb->action( "aSeparator2" );
		mb->menu( "mSave", tr( "&Save" ), QIcon( ":/file/icons/file/save.png" ) );
		mb->action( "mSave/aCurrent", tr( "&Save" ), QIcon( ":/file/icons/file/save.png" ), tr( "Ctrl+S" ), tr( "Save the current file" ) )->setEnabled( false );
		mb->action( "mSave/aAll", tr( "Save &All" ), QIcon( ":/file/icons/file/saveall.png" ), QString::null, tr( "Save all files" ) )->setEnabled( false );
		mb->menu( "mClose", tr( "&Close" ), QIcon( ":/file/icons/file/close.png" ) );
		mb->action( "mClose/aCurrent", tr( "&Close" ), QIcon( ":/file/icons/file/close.png" ), tr( "Ctrl+W" ), tr( "Close the current file" ) )->setEnabled( false );
		mb->action( "mClose/aAll", tr( "Close &All" ), QIcon( ":/file/icons/file/closeall.png" ), QString::null, tr( "Close all files" ) )->setEnabled( false );
		mb->action( "aSeparator3" );
		mb->action( "aSaveAsBackup", tr( "Save As &Backup" ), QIcon( ":/file/icons/file/backup.png" ), tr( "Ctrl+B" ), tr( "Save a backup of the current file" ) )->setEnabled( false );
		mb->action( "aSeparator4" );
		mb->action( "aQuickPrint", tr( "Quic&k Print" ), QIcon( ":/file/icons/file/quickprint.png" ), QString::null, tr( "Quick print the current file" ) )->setEnabled( false );
		mb->action( "aPrint", tr( "&Print..." ), QIcon( ":/file/icons/file/print.png" ), tr( "Ctrl+P" ), tr( "Print the current file" ) )->setEnabled( false );
		mb->action( "aSeparator5" );
		mb->action( "aQuit", tr( "&Quit" ), QIcon( ":/file/icons/file/quit.png" ), tr( "Ctrl+Q" ), tr( "Quit the application" ) );
	mb->endGroup();
	mb->menu( "mEdit", tr( "&Edit" ) );
	mb->beginGroup( "mEdit" );
		mb->action( "aSettings", tr( "Settings..." ), QIcon( ":/edit/icons/edit/settings.png" ), "", tr( "Edit the application settings" ) );
		mb->action( "aShortcutsEditor", tr( "Shortcuts Editor..." ), QIcon( ":/edit/icons/edit/shortcuts.png" ), tr( "Ctrl+Shift+E" ), tr( "Edit the application shortcuts" ) );
		mb->action( "aTranslations", tr( "Translations..." ), QIcon( ":/edit/icons/edit/translations.png" ), tr( "Ctrl+T" ), tr( "Change the application translations files" ) );
		mb->action( "aSeparator1" );
		mb->action( "aUndo", tr( "&Undo" ), QIcon( ":/edit/icons/edit/undo.png" ), tr( "Ctrl+Z" ), tr( "Undo" ) )->setEnabled( false );
		mb->action( "aRedo", tr( "&Redo" ), QIcon( ":/edit/icons/edit/redo.png" ), tr( "Ctrl+Y" ), tr( "Redo" ) )->setEnabled( false );
		mb->action( "aSeparator2" );
		mb->action( "aCopy", tr( "&Copy" ), QIcon( ":/edit/icons/edit/copy.png" ), tr( "Ctrl+C" ), tr( "Copy" ) )->setEnabled( false );
		mb->action( "aCut", tr( "Cu&t" ), QIcon( ":/edit/icons/edit/cut.png" ), tr( "Ctrl+X" ), tr( "Cut" ) )->setEnabled( false );
		mb->action( "aPaste", tr( "&Paste" ), QIcon( ":/edit/icons/edit/paste.png" ), tr( "Ctrl+V" ), tr( "Paste" ) )->setEnabled( false );
		mb->action( "aSeparator3" );
		mb->menu( "mSearchReplace", tr( "&Search and replace" ) );
			mb->action( "mSearchReplace/aSearchFile", tr( "&Search in the file..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "Ctrl+F" ), tr( "Search in the file..." ) )->setEnabled( true );
			mb->action( "mSearchReplace/aReplaceFile", tr( "&Replace in the file..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "Ctrl+R" ), tr( "Replace in the file..." ) )->setEnabled( true );
			//mb->action( "mSearchReplace/aSearchProject", tr( "&Search in the project..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "" ), tr( "Search in the project..." ) )->setEnabled( true );
			//mb->action( "mSearchReplace/aReplaceProject", tr( "&Replace in the project..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "" ), tr( "Replace in the project..." ) )->setEnabled( true );
			mb->action( "mSearchReplace/aSearchFolder", tr( "&Search in the folder..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "Ctrl+Alt+F" ), tr( "Search in the folder..." ) )->setEnabled( true );
			mb->action( "mSearchReplace/aReplaceFolder", tr( "&Replace in the folder..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "Ctrl+Alt+R" ), tr( "Replace in the folder..." ) )->setEnabled( true );
			mb->action( "mSearchReplace/aSearchPrevious", tr( "Search Previous" ), QIcon( ":/edit/icons/edit/previous.png" ), tr( "Shift+F3" ), tr( "Search Previous" ) )->setEnabled( true );
			mb->action( "mSearchReplace/aSearchNext", tr( "Search Next" ), QIcon( ":/edit/icons/edit/next.png" ), tr( "F3" ), tr( "Search Next" ) )->setEnabled( true );
		mb->action( "aGoTo", tr( "&Go To..." ), QIcon( ":/edit/icons/edit/goto.png" ), tr( "Ctrl+G" ), tr( "Go To..." ) )->setEnabled( false );
		mb->menu( "mAllCommands", tr( "&All Commands" ), QIcon( ":/edit/icons/edit/commands.png" ) );
		mb->action( "aSeparator5" );
		mb->action( "aExpandAbbreviation", tr( "Expand Abbreviation" ), QIcon( ":/edit/icons/edit/abbreviation.png" ), tr( "Ctrl+E" ), tr( "Expand Abbreviation" ) )->setEnabled( false );
		mb->action( "aPrepareAPIs", tr( "Prepare APIs" ), QIcon( ":/edit/icons/edit/prepareapis.png" ), tr( "Ctrl+Alt+P" ), tr( "Prepare the APIs files for auto completion / calltips" ) );
	mb->endGroup();
	mb->menu( "mView", tr( "&View" ) );
	mb->beginGroup( "mView" );
		mb->menu( "mStyle", tr( "&Style" ), QIcon( ":/view/icons/view/style.png" ) );
		mb->action( "aNext", tr( "&Next Tab" ), QIcon( ":/view/icons/view/next.png" ), tr( "Alt+Right" ), tr( "Active the next tab" ) )->setEnabled( false );
		mb->action( "aPrevious", tr( "&Previous Tab" ), QIcon( ":/view/icons/view/previous.png" ), tr( "Alt+Left" ), tr( "Active the previous tab" ) )->setEnabled( false );
		mb->action( "aFiltered", tr( "&Filtered Projects" ), QIcon( ":/view/icons/view/filtered.png" ), tr( "" ), tr( "Filtered project view" ) )->setCheckable( true );
	mb->endGroup();
	mb->menu( "mProject", tr( "&Project" ) );
	mb->beginGroup( "mProject" );
		mb->action( "aNew", tr( "&New" ), QIcon( ":/project/icons/project/new.png" ), tr( "Ctrl+Shift+N" ), tr( "New project..." ) );
		mb->action( "aOpen", tr( "&Open" ), QIcon( ":/project/icons/project/open.png" ), tr( "Ctrl+Shift+O" ), tr( "Open a project..." ) );
		mb->menu( "mSave", tr( "&Save" ), QIcon( ":/project/icons/project/save.png" ) );
		mb->action( "mSave/aCurrent", tr( "&Save" ), QIcon( ":/project/icons/project/save.png" ), QString::null, tr( "Save the current project" ) )->setEnabled( false );
		mb->action( "mSave/aAll", tr( "Save &All" ), QIcon( ":/project/icons/project/saveall.png" ), QString::null, tr( "Save all projects" ) )->setEnabled( false );
		mb->menu( "mClose", tr( "&Close" ), QIcon( ":/project/icons/project/close.png" ) );
		mb->action( "mClose/aCurrent", tr( "&Close" ), QIcon( ":/project/icons/project/close.png" ), QString::null, tr( "Close the current project" ) )->setEnabled( false );
		mb->action( "mClose/aAll", tr( "Close &All" ), QIcon( ":/project/icons/project/closeall.png" ), QString::null, tr( "Close all projects" ) )->setEnabled( false );
		mb->action( "aSeparator2" );
		mb->action( "aSettings", tr( "Set&tings..." ), QIcon( ":/project/icons/project/settings.png" ), QString::null, tr( "Project settings" ) )->setEnabled( false );
		mb->action( "aSeparator3" );
		mb->action( "aAddExistingFiles", tr( "&Add Existing Files/Projects..." ), QIcon( ":/project/icons/project/add.png" ), QString::null, tr( "Add existing files/projects to the current project" ) )->setEnabled( false );
		mb->action( "aRemove", tr( "&Remove Variable/Value..." ), QIcon( ":/project/icons/project/remove.png" ), QString::null, tr( "Remove the current variable/value" ) )->setEnabled( false );
		mb->action( "aSeparator4" );
		mb->menu( "mRecents", tr( "&Recents" ), QIcon( ":/project/icons/project/recents.png" ) );
		mb->action( "mRecents/aClear", tr( "&Clear" ), QIcon( ":/project/icons/project/clear.png" ), QString::null, tr( "Clear the recents projects list" ) );
		mb->action( "mRecents/aSeparator1" );
	mb->endGroup();
	mb->menu( "mBuilder", tr( "&Build" ) )->setEnabled( false );
	mb->beginGroup( "mBuilder" );
		mb->menu( "mBuild", tr( "&Build" ), QIcon( ":/build/icons/build/build.png" ) );
		mb->menu( "mRebuild", tr( "&Rebuild" ), QIcon( ":/build/icons/build/rebuild.png" ) );
		mb->menu( "mClean", tr( "&Clean" ), QIcon( ":/build/icons/build/clean.png" ) );
		mb->menu( "mExecute", tr( "&Execute" ), QIcon( ":/build/icons/build/execute.png" ) );
		mb->menu( "mUserCommands", tr( "&User Commands" ), QIcon( ":/build/icons/build/misc.png" ) );
		mb->action( "aSeparator1" );
	mb->endGroup();
	mb->menu( "mDebugger", tr( "&Debugger" ) )->setEnabled( false );
	mb->menu( "mInterpreter", tr( "&Interpreter" ) )->setEnabled( false );
	mb->menu( "mTools", tr( "&Tools" ) );
	mb->beginGroup( "mTools" );
		mb->action( "aEditUser", tr( "&Edit User Tools..." ), QIcon( ":/tools/icons/tools/edit.png" ), QString::null, tr( "Edit tools..." ) );
		mb->action( "aEditDesktop", tr( "Edit &Desktop Tools..." ), QIcon( ":/tools/icons/tools/desktop.png" ), QString::null, tr( "Edit desktop tools..." ) );
		mb->action( "aSeparator1" );
		mb->menu( "mUserTools", tr( "&User Tools" ), QIcon( ":/tools/icons/tools/user.png" ) );
		mb->menu( "mDesktopTools", tr( "Desktop &Tools" ), QIcon( ":/tools/icons/tools/desktop.png" ) );
	mb->endGroup();
	mb->menu( "mPlugins", tr( "Plu&gins" ) );
	mb->beginGroup( "mPlugins" );
		mb->action( "aManage", tr( "&Manage..." ), QIcon( ":/Icons/Icons/toolsedit.png" ), QString::null, tr( "Manage plugins..." ) );
		mb->action( "aSeparator1" );
	mb->endGroup();
	mb->menu( "mWindow", tr( "&Window" ) );
	mb->beginGroup( "mWindow" );
		mb->action( "aSDI", tr( "&Single Document Interface" ), QIcon( "" ), QString::null, tr( "Single Document Interface" ) );
		mb->action( "aMDI", tr( "&Multiple Document Interface" ), QIcon( "" ), QString::null, tr( "Multiple Document Interface" ) );
		mb->action( "aTopLevel", tr( "&Top Level Windows Interface" ), QIcon( "" ), QString::null, tr( "Top Level Windows Interface" ) );
		mb->action( "aSeparator1" );
		mb->action( "aCascase", tr( "&Cascade" ), QIcon( "" ), QString::null, tr( "Cascade" ) );
		mb->action( "aTile", tr( "&Tile" ), QIcon( "" ), QString::null, tr( "Tile" ) );
		mb->action( "aMinimize", tr( "&Minimize" ), QIcon( "" ), QString::null, tr( "Minimize" ) );
		mb->action( "aRestore", tr( "&Restore" ), QIcon( "" ), QString::null, tr( "Restore normal size" ) );
	mb->endGroup();
	mb->menu( "mHelp", tr( "&Help" ) );
	mb->beginGroup( "mHelp" );
		mb->action( "aManual", tr( "&Manual" ), QIcon( ":/help/icons/help/assistant.png" ), QString::null, tr( "Manual" ) );
		mb->action( "aSeparator1" );
		mb->action( "aAbout", tr( "&About..." ), QIcon( ":/application/icons/application/monkey2.png" ), QString::null, tr( "About application..." ) );
		mb->action( "aAboutQt", tr( "About &Qt..." ), QIcon( ":/help/icons/help/qt.png" ), QString::null, tr( "About Qt..." ) );
#ifdef __COVERAGESCANNER__
		mb->action( "aTestReport", tr( "&Test Report" ), QIcon( ) , tr( "Pause" ), tr( "Coverage Meter Test Report..." ) );
#endif
	mb->endGroup();
	// create action for styles
	agStyles = new QActionGroup( mb->menu( "mView/mStyle" ) );
	foreach ( QString s, QStyleFactory::keys() )
	{
		QAction* a = agStyles->addAction( s );
		a->setCheckable( true );
		if ( MonkeyCore::settings()->value( "MainWindow/Style" ).toString() == s )
			a->setChecked( true );
	}
	// add styles action to menu
	mb->menu( "mView/mStyle" )->addActions( agStyles->actions() );
}

void UIMain::initToolBar()
{
	// recents
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->menu( "mFile/mRecents" )->menuAction() );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->menu( "mFile/mSession" )->menuAction() );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->menu( "mProject/mRecents" )->menuAction() );
	dockToolBar( Qt::TopToolBarArea )->addAction();
	// settings, tools
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mEdit/aSettings" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mEdit/aShortcutsEditor" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mTools/aEditUser" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mTools/aEditDesktop" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction();
	// file action
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mFile/aNew" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mFile/aOpen" ) );
	dockToolBar( Qt::TopToolBarArea )->addActions( menuBar()->menu( "mFile/mSave" )->actions() );
	dockToolBar( Qt::TopToolBarArea )->addActions( menuBar()->menu( "mFile/mClose" )->actions() );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mFile/aQuickPrint" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction();
	// edit action
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mEdit/aUndo" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mEdit/aRedo" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction();
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mEdit/aCut" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mEdit/aCopy" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mEdit/aPaste" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction();
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mEdit/aSearchReplace" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mEdit/aGoTo" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction();
	// help action
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mHelp/aAbout" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction();
	// console action
	dockToolBar( Qt::TopToolBarArea )->addAction( MonkeyCore::consoleManager()->stopAction() );
}

void UIMain::initConnections()
{
	// file connection
	connect( menuBar()->action( "mFile/aNew" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileNew_triggered() ) );
	connect( menuBar()->action( "mFile/aOpen" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileOpen_triggered() ) );
	connect( MonkeyCore::recentsManager(), SIGNAL( openFileRequested( const QString& ) ), MonkeyCore::fileManager(), SLOT( openFile( const QString& ) ) );
	connect( menuBar()->action( "mFile/mSession/aSave" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileSessionSave_triggered() ) );
	connect( menuBar()->action( "mFile/mSession/aRestore" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileSessionRestore_triggered() ) );
	connect( menuBar()->action( "mFile/mSave/aCurrent" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileSaveCurrent_triggered() ) );
	connect( menuBar()->action( "mFile/mSave/aAll" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileSaveAll_triggered() ) );
	connect( menuBar()->action( "mFile/mClose/aCurrent" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileCloseCurrent_triggered() ) );
	connect( menuBar()->action( "mFile/mClose/aAll" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileCloseAll_triggered() ) );
	connect( menuBar()->action( "mFile/aSaveAsBackup" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileSaveAsBackup_triggered() ) );
	connect( menuBar()->action( "mFile/aQuickPrint" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileQuickPrint_triggered() ) );
	connect( menuBar()->action( "mFile/aPrint" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( filePrint_triggered() ) );
	connect( menuBar()->action( "mFile/aQuit" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( fileExit_triggered() ) );
	// edit connection
	connect( menuBar()->action( "mEdit/aSettings" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editSettings_triggered() ) );
	connect( menuBar()->action( "mEdit/aShortcutsEditor" ), SIGNAL( triggered() ), MonkeyCore::actionManager(), SLOT( showSettings() ) );
	connect( menuBar()->action( "mEdit/aTranslations" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editTranslations_triggered() ) );
	connect( menuBar()->action( "mEdit/aUndo" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editUndo_triggered() ) );
	connect( menuBar()->action( "mEdit/aRedo" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editRedo_triggered() ) );
	connect( menuBar()->action( "mEdit/aCut" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editCut_triggered() ) );
	connect( menuBar()->action( "mEdit/aCopy" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editCopy_triggered() ) );
	connect( menuBar()->action( "mEdit/aPaste" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editPaste_triggered() ) );
	//connect( menuBar()->action( "mEdit/aSearchFile" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editSearchReplace_triggered() ) );
	//connect( menuBar()->action( "mEdit/aSearchPrevious" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editSearchPrevious_triggered() ) );
	//connect( menuBar()->action( "mEdit/aSearchNext" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editSearchNext_triggered() ) );
	connect( menuBar()->action( "mEdit/aGoTo" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editGoTo_triggered() ) );
	connect( menuBar()->action( "mEdit/aExpandAbbreviation" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editExpandAbbreviation_triggered() ) );
	connect( menuBar()->action( "mEdit/aPrepareAPIs" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( editPrepareAPIs_triggered() ) );
	// view connection
	connect( agStyles, SIGNAL( triggered( QAction* ) ), MonkeyCore::workspace(), SLOT( agStyles_triggered( QAction* ) ) );
	connect( menuBar()->action( "mView/aNext" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( activateNextDocument() ) );
	connect( menuBar()->action( "mView/aPrevious" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( activatePreviousDocument() ) );
	connect( menuBar()->action( "mView/aFiltered" ), SIGNAL( triggered( bool ) ), MonkeyCore::projectsManager()->proxy(), SLOT( setFiltering( bool ) ) );
	// project connection
	connect( menuBar()->action( "mProject/aNew" ), SIGNAL( triggered() ), MonkeyCore::projectsManager(), SLOT( projectNew_triggered() ) );
	connect( menuBar()->action( "mProject/aOpen" ), SIGNAL( triggered() ), MonkeyCore::projectsManager(), SLOT( projectOpen_triggered() ) );
	connect( menuBar()->action( "mProject/mSave/aCurrent" ), SIGNAL( triggered() ), MonkeyCore::projectsManager(), SLOT( projectSaveCurrent_triggered() ) );
	connect( menuBar()->action( "mProject/mSave/aAll" ), SIGNAL( triggered() ), MonkeyCore::projectsManager(), SLOT( projectSaveAll_triggered() ) );
	connect( menuBar()->action( "mProject/mClose/aCurrent" ), SIGNAL( triggered() ), MonkeyCore::projectsManager(), SLOT( projectCloseCurrent_triggered() ) );
	connect( menuBar()->action( "mProject/mClose/aAll" ), SIGNAL( triggered() ), MonkeyCore::projectsManager(), SLOT( projectCloseAll_triggered() ) );
	connect( menuBar()->action( "mProject/aSettings" ), SIGNAL( triggered() ), MonkeyCore::projectsManager(), SLOT( projectSettings_triggered() ) );
	connect( menuBar()->action( "mProject/aAddExistingFiles" ), SIGNAL( triggered() ), MonkeyCore::projectsManager(), SLOT( projectAddExistingFiles_triggered() ) );
	connect( menuBar()->action( "mProject/aRemove" ), SIGNAL( triggered() ), MonkeyCore::projectsManager(), SLOT( projectRemove_triggered() ) );
	connect( MonkeyCore::recentsManager(), SIGNAL( openProjectRequested( const QString& ) ), MonkeyCore::projectsManager(), SLOT( openProject( const QString& ) ) );
	// builder debugger interpreter menu
	connect( menuBar()->menu( "mBuilder" ), SIGNAL( aboutToShow() ), this, SLOT( menu_aboutToShow() ) );
	connect( menuBar()->menu( "mDebugger" ), SIGNAL( aboutToShow() ), this, SLOT( menu_aboutToShow() ) );
	connect( menuBar()->menu( "mInterpreter" ), SIGNAL( aboutToShow() ), this, SLOT( menu_aboutToShow() ) );
	// plugins menu
	connect( menuBar()->action( "mPlugins/aManage" ), SIGNAL( triggered() ), MonkeyCore::pluginsManager(), SLOT( manageRequested() ) );
	// window menu
	connect( menuBar()->action( "mWindow/aSDI" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( setSDI() ) );
	connect( menuBar()->action( "mWindow/aMDI" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( setMDI() ) );
	connect( menuBar()->action( "mWindow/aTopLevel" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( setTopLevel() ) );
	connect( menuBar()->action( "mWindow/aTile" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( tile() ) );
	connect( menuBar()->action( "mWindow/aCascase" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( cascade() ) );
	connect( menuBar()->action( "mWindow/aMinimize" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( minimize() ) );
	connect( menuBar()->action( "mWindow/aRestore" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( restore() ) );
	// help menu
	connect( menuBar()->action( "mHelp/aAbout" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( helpAboutApplication_triggered() ) );
	connect( menuBar()->action( "mHelp/aAboutQt" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( helpAboutQt_triggered() ) );
#ifdef __COVERAGESCANNER__
	connect( menuBar()->action( "mHelp/aTestReport" ), SIGNAL( triggered() ), MonkeyCore::workspace(), SLOT( helpTestReport_triggered() ) );
#endif
}

void UIMain::menu_aboutToShow()
{
	if ( QMenu* m = qobject_cast<QMenu*>( sender() ) )
		foreach ( QAction* a, m->actions() )
			if ( a->menu() )
				a->menu()->menuAction()->setVisible( a->menu()->actions().count() );
}
