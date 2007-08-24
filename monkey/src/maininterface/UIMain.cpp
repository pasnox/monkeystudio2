/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : UIMain.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "UIMain.h"
#include "pFileManager.h"
#include "pWorkspace.h"
#include "pMenuBar.h"
#include "pRecentsManager.h"
#include "pActionManager.h"
#include "pToolsManager.h"
#include "pSettings.h"
#include "pDockToolBar.h"
#include "pSearch.h"
#include "pMonkeyStudio.h"

#include "UIMessageBox.h" // remvoe me at end, this not core and only there for testing ConsoleManager

#include <QActionGroup>
#include <QStyleFactory>
#include <QStatusBar>

UIMain::UIMain( QWidget* p )
	: pMainWindow( p )
{
	// create menubar
	initMenuBar();

	// init toolbar
	initToolBar();

	// init gui
	initGui();

	// init connections
	initConnections();

	// restore session if needed
	if ( pMonkeyStudio::restoreSessionOnStartup() )
		workspace()->fileSessionRestore_triggered();
}

void UIMain::closeEvent( QCloseEvent* )
{
	// save session if needed
	if ( pMonkeyStudio::saveSessionOnClose() )
		workspace()->fileSessionSave_triggered();

	// close all documents
	workspace()->fileCloseAll_triggered();
}

pFileManager* UIMain::fileManager()
{
	return pFileManager::instance( this );
}

pWorkspace* UIMain::workspace()
{
	return pWorkspace::instance( this );
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
		mb->action( "mSave/aCurrent", tr( "&Current" ), QIcon( ":/file/icons/file/save.png" ), tr( "Ctrl+S" ), tr( "Save the current file" ) )->setEnabled( false );
		mb->action( "mSave/aAll", tr( "&All" ), QIcon( ":/file/icons/file/saveall.png" ), QString::null, tr( "Save all files" ) )->setEnabled( false );
		mb->menu( "mClose", tr( "&Close" ), QIcon( ":/file/icons/file/close.png" ) );
		mb->action( "mClose/aCurrent", tr( "&Current" ), QIcon( ":/file/icons/file/close.png" ), tr( "Ctrl+W" ), tr( "Close the current file" ) )->setEnabled( false );
		mb->action( "mClose/aAll", tr( "&All" ), QIcon( ":/file/icons/file/closeall.png" ), QString::null, tr( "Close all files" ) )->setEnabled( false );
		mb->action( "aSeparator3" );
		//mb->action( "aSaveAsTemplate", tr( "Save As &Template" ), QIcon( ":/Icons/Icons/filesaveastemplate.png" ), tr( "Ctrl+T" ), tr( "Save the current file as template" ) )->setEnabled( false );
		mb->action( "aSeparator4" );
		mb->action( "aQuickPrint", tr( "Quic&k Print" ), QIcon( ":/file/icons/file/quickprint.png" ), QString::null, tr( "Quick print the current file" ) )->setEnabled( false );
		mb->action( "aPrint", tr( "&Print..." ), QIcon( ":/file/icons/file/print.png" ), tr( "Ctrl+P" ), tr( "Print the current file" ) )->setEnabled( false );
		mb->action( "aSeparator5" );
		mb->action( "aQuit", tr( "&Quit" ), QIcon( ":/file/icons/file/quit.png" ), tr( "Ctrl+Q" ), tr( "Quit the application" ) );
	mb->endGroup();
	mb->menu( "mEdit", tr( "&Edit" ) );
	mb->beginGroup( "mEdit" );
		mb->action( "aSettings", tr( "Settings..." ), QIcon( ":/edit/icons/edit/settings.png" ), "", tr( "Edit the application settings" ) );
		mb->action( "aShortcutsEditor", tr( "Shortcuts Editor..." ), QIcon( ":/edit/icons/edit/shortcuts.png" ), tr( "Ctrl+E" ), tr( "Edit the application shortcuts" ) );
		mb->action( "aSeparator1" );
		mb->action( "aUndo", tr( "&Undo" ), QIcon( ":/edit/icons/edit/undo.png" ), tr( "Ctrl+Z" ), tr( "Undo" ) )->setEnabled( false );
		mb->action( "aRedo", tr( "&Redo" ), QIcon( ":/edit/icons/edit/redo.png" ), tr( "Ctrl+Y" ), tr( "Redo" ) )->setEnabled( false );
		mb->action( "aSeparator2" );
		mb->action( "aCopy", tr( "&Copy" ), QIcon( ":/edit/icons/edit/copy.png" ), tr( "Ctrl+C" ), tr( "Copy" ) )->setEnabled( false );
		mb->action( "aCut", tr( "Cu&t" ), QIcon( ":/edit/icons/edit/cut.png" ), tr( "Ctrl+X" ), tr( "Cut" ) )->setEnabled( false );
		mb->action( "aPaste", tr( "&Paste" ), QIcon( ":/edit/icons/edit/paste.png" ), tr( "Ctrl+V" ), tr( "Paste" ) )->setEnabled( false );
		mb->action( "aSeparator3" );
		mb->action( "aSearchReplace", tr( "&Search - Replace..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "Ctrl+F" ), tr( "Search - Replace..." ) )->setEnabled( false );
		mb->action( "aSearchPrevious", tr( "Search Previous" ), QIcon( ":/edit/icons/edit/previous.png" ), tr( "Shift+F3" ), tr( "Search Previous" ) )->setEnabled( false );
		mb->action( "aSearchNext", tr( "Search Next" ), QIcon( ":/edit/icons/edit/next.png" ), tr( "F3" ), tr( "Search Next" ) )->setEnabled( false );
		mb->action( "aSeparator4" );
		mb->action( "aGoTo", tr( "&Go To..." ), QIcon( ":/edit/icons/edit/goto.png" ), tr( "Ctrl+G" ), tr( "Go To..." ) )->setEnabled( false );
		mb->action( "aSeparator5" );
		mb->action( "aExpandAbbreviation", tr( "Expand Abbreviation" ), QIcon( ":/edit/icons/edit/abbreviation.png" ), tr( "Ctrl+L" ), tr( "Expand Abbreviation" ) )->setEnabled( false );
	mb->endGroup();
	mb->menu( "mView", tr( "&View" ) );
	mb->beginGroup( "mView" );
		mb->menu( "mStyle", tr( "&Style" ), QIcon( ":/view/icons/view/style.png" ) );
		mb->action( "aNext", tr( "&Next Tab" ), QIcon( ":/view/icons/view/next.png" ), tr( "Alt+Right" ), tr( "Active the next tab" ) )->setEnabled( false );
		mb->action( "aPrevious", tr( "&Previous Tab" ), QIcon( ":/view/icons/view/previous.png" ), tr( "Alt+Left" ), tr( "Active the previous tab" ) )->setEnabled( false );
		//mb->action( "aFilteredView", tr( "&Filtered view" ), QIcon( "" ), tr( "" ), tr( "Filtered project view" ) )->setEnabled( false );	
	mb->endGroup();
	mb->menu( "mProject", tr( "&Project" ) );
	mb->beginGroup( "mProject" );
		mb->action( "aNew", tr( "&New" ), QIcon( ":/Icons/Icons/projectnew.png" ), tr( "Ctrl+Shift+N" ), tr( "New project..." ) );
		mb->action( "aOpen", tr( "&Open" ), QIcon( ":/Icons/Icons/projectopen.png" ), tr( "Ctrl+Shift+O" ), tr( "Open a project..." ) );
		mb->menu( "mSave", tr( "&Save" ), QIcon( ":/Icons/Icons/projectsave.png" ) );
		mb->action( "mSave/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/projectsave.png" ), QString::null, tr( "Save the current project" ) );
		mb->action( "mSave/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/projectsaveall.png" ), QString::null, tr( "Save all projects" ) );
		mb->menu( "mClose", tr( "&Close" ), QIcon( ":/Icons/Icons/projectclose.png" ) );
		mb->action( "mClose/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/projectclose.png" ), QString::null, tr( "Close the current project" ) );
		mb->action( "mClose/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/projectcloseall.png" ), QString::null, tr( "Close all projects" ) );
		mb->action( "aSeparator2" );
		mb->action( "aSettings", tr( "Set&tings..." ), QIcon( ":/Icons/Icons/projectsettings.png" ), QString::null, tr( "Project settings" ) );
		mb->action( "aSeparator3" );
		mb->menu( "mRecents", tr( "&Recents" ), QIcon( ":/Icons/Icons/recentsprojects.png" ) );
		mb->action( "mRecents/aClear", tr( "&Clear" ), QIcon( ":/Icons/Icons/filecloseall.png" ), QString::null, tr( "Clear the recents projects list" ) );
		mb->action( "mRecents/aSeparator1" );
	mb->endGroup();
	mb->menu( "mBuild", tr( "&Build" ) );
/* Need to be plugin that instore its actions
	mb->beginGroup( "mBuild" );
		mb->menu( "mBuild", tr( "&Build" ), QIcon( ":/Icons/Icons/buildbuild.png" ) );
		mb->action( "mBuild/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildbuild.png" ), QString::null, tr( "Build current" ) );
		mb->action( "mBuild/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildbuild.png" ), QString::null, tr( "Build all" ) );
		mb->menu( "mRebuild", tr( "&Rebuild" ), QIcon( ":/Icons/Icons/buildrebuild.png" ) );
		mb->action( "mRebuild/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildrebuild.png" ), QString::null, tr( "Rebuild current" ) );
		mb->action( "mRebuild/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildrebuild.png" ), QString::null, tr( "Rebuild all" ) );
		mb->action( "aSeparator1" );
		mb->action( "aStop", tr( "&Stop" ), QIcon( ":/Icons/Icons/buildstop.png" ), QString::null, tr( "Stop current" ) );
		mb->action( "aSeparator2" );
		mb->menu( "mClean", tr( "&Clean" ), QIcon( ":/Icons/Icons/buildclean.png" ) );
		mb->action( "mClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/buildclean.png" ), QString::null, tr( "Clean current" ) );
		mb->action( "mClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/buildclean.png" ), QString::null, tr( "Clean all" ) );
		mb->menu( "mDistClean", tr( "&Dist Clean" ), QIcon( ":/Icons/Icons/builddistclean.png" ) );
		mb->action( "mDistClean/aCurrent", tr( "&Current" ), QIcon( ":/Icons/Icons/builddistclean.png" ), QString::null, tr( "Dist clean current" ) );
		mb->action( "mDistClean/aAll", tr( "&All" ), QIcon( ":/Icons/Icons/builddistclean.png" ), QString::null, tr( "Dist clean all" ) );
		mb->action( "aSeparator3" );
		mb->action( "aExecute", tr( "&Execute" ), QIcon( ":/Icons/Icons/buildexecute.png" ), QString::null, tr( "Execute current" ) );
		mb->action( "aExecuteWithParameters", tr( "Execute with &parameters..." ), QIcon( ":/Icons/Icons/buildexecute.png" ), QString::null, tr( "Execute current with parameters..." ) );
		mb->action( "aSeparator4" );
		mb->action( "aBuildExecute", tr( "Build, Execu&te" ), QIcon( ":/Icons/Icons/buildmisc.png" ), QString::null, tr( "Build and execute current" ) );
		mb->action( "aDistCleanBuildExecute", tr( "Dist Clean, Build, E&xecute" ), QIcon( ":/Icons/Icons/buildmisc.png" ), QString::null, tr( "Dist clean, build and execute current" ) );
	mb->endGroup();
*/
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
	agStyles = new QActionGroup( menuBar()->menu( "mView/mStyle" ) );
	foreach ( QString s, QStyleFactory::keys() )
	{
		QAction* a = agStyles->addAction( s );
		a->setCheckable( true );
		if ( settings()->value( "MainWindow/Style" ).toString() == s )
			a->setChecked( true );
	}

	// add styles action to menu
	menuBar()->menu( "mView/mStyle" )->addActions( agStyles->actions() );

	// init recents manager
	pRecentsManager::instance( this );

	// init tools manager
	pToolsManager::instance( this );
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

	// build action
/*
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mBuild/aDistCleanBuildExecute" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mBuild/aBuildExecute" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mBuild/mBuild/aCurrent" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mBuild/aExecute" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mBuild/aStop" ) );
	dockToolBar( Qt::TopToolBarArea )->addAction();
*/

	// help action
	dockToolBar( Qt::TopToolBarArea )->addAction( menuBar()->action( "mHelp/aAbout" ) );
}

void UIMain::initConnections()
{
	// file connection
	connect( menuBar()->action( "mFile/aNew" ), SIGNAL( triggered() ), workspace(), SLOT( fileNew_triggered() ) );
	connect( menuBar()->action( "mFile/aOpen" ), SIGNAL( triggered() ), workspace(), SLOT( fileOpen_triggered() ) );
	connect( pRecentsManager::instance(), SIGNAL( openFileRequested( const QString& ) ), fileManager(), SLOT( openFile( const QString& ) ) );
	connect( menuBar()->action( "mFile/mSession/aSave" ), SIGNAL( triggered() ), workspace(), SLOT( fileSessionSave_triggered() ) );
	connect( menuBar()->action( "mFile/mSession/aRestore" ), SIGNAL( triggered() ), workspace(), SLOT( fileSessionRestore_triggered() ) );
	connect( menuBar()->action( "mFile/mSave/aCurrent" ), SIGNAL( triggered() ), workspace(), SLOT( fileSaveCurrent_triggered() ) );
	connect( menuBar()->action( "mFile/mSave/aAll" ), SIGNAL( triggered() ), workspace(), SLOT( fileSaveAll_triggered() ) );
	connect( menuBar()->action( "mFile/mClose/aCurrent" ), SIGNAL( triggered() ), workspace(), SLOT( fileCloseCurrent_triggered() ) );
	connect( menuBar()->action( "mFile/mClose/aAll" ), SIGNAL( triggered() ), workspace(), SLOT( fileCloseAll_triggered() ) );
	//connect( menuBar()->action( "mFile/aSaveAsTemplate" ), SIGNAL( triggered() ), workspace(), SLOT( fileSaveAsTemplate_triggered() ) );
	connect( menuBar()->action( "mFile/aQuickPrint" ), SIGNAL( triggered() ), workspace(), SLOT( fileQuickPrint_triggered() ) );
	connect( menuBar()->action( "mFile/aPrint" ), SIGNAL( triggered() ), workspace(), SLOT( filePrint_triggered() ) );
	connect( menuBar()->action( "mFile/aQuit" ), SIGNAL( triggered() ), workspace(), SLOT( fileExit_triggered() ) );
	// edit connection
	connect( menuBar()->action( "mEdit/aSettings" ), SIGNAL( triggered() ), workspace(), SLOT( editSettings_triggered() ) );
	connect( menuBar()->action( "mEdit/aShortcutsEditor" ), SIGNAL( triggered() ), pActionManager::instance(), SLOT( showSettings() ) );
	connect( menuBar()->action( "mEdit/aUndo" ), SIGNAL( triggered() ), workspace(), SLOT( editUndo_triggered() ) );
	connect( menuBar()->action( "mEdit/aRedo" ), SIGNAL( triggered() ), workspace(), SLOT( editRedo_triggered() ) );
	connect( menuBar()->action( "mEdit/aCut" ), SIGNAL( triggered() ), workspace(), SLOT( editCut_triggered() ) );
	connect( menuBar()->action( "mEdit/aCopy" ), SIGNAL( triggered() ), workspace(), SLOT( editCopy_triggered() ) );
	connect( menuBar()->action( "mEdit/aPaste" ), SIGNAL( triggered() ), workspace(), SLOT( editPaste_triggered() ) );
	connect( menuBar()->action( "mEdit/aSearchReplace" ), SIGNAL( triggered() ), workspace(), SLOT( editSearchReplace_triggered() ) );
	connect( menuBar()->action( "mEdit/aSearchPrevious" ), SIGNAL( triggered() ), workspace(), SLOT( editSearchPrevious_triggered() ) );
	connect( menuBar()->action( "mEdit/aSearchNext" ), SIGNAL( triggered() ), workspace(), SLOT( editSearchNext_triggered() ) );
	connect( menuBar()->action( "mEdit/aGoTo" ), SIGNAL( triggered() ), workspace(), SLOT( editGoTo_triggered() ) );
	connect( menuBar()->action( "mEdit/aExpandAbbreviation" ), SIGNAL( triggered() ), workspace(), SLOT( editExpandAbbreviation_triggered() ) );
	// view connection
	connect( agStyles, SIGNAL( triggered( QAction* ) ), workspace(), SLOT( agStyles_triggered( QAction* ) ) );
	connect( menuBar()->action( "mView/aNext" ), SIGNAL( triggered() ), workspace(), SLOT( activateNextDocument() ) );
	connect( menuBar()->action( "mView/aPrevious" ), SIGNAL( triggered() ), workspace(), SLOT( activatePreviousDocument() ) );
	// project connection
	connect( menuBar()->action( "mProject/aNew" ), SIGNAL( triggered() ), workspace(), SLOT( projectNew_triggered() ) );
	connect( menuBar()->action( "mProject/aOpen" ), SIGNAL( triggered() ), workspace(), SLOT( projectOpen_triggered() ) );
	connect( menuBar()->action( "mProject/mSave/aCurrent" ), SIGNAL( triggered() ), workspace(), SLOT( projectSaveCurrent_triggered() ) );
	connect( menuBar()->action( "mProject/mSave/aAll" ), SIGNAL( triggered() ), workspace(), SLOT( projectSaveAll_triggered() ) );
	connect( menuBar()->action( "mProject/mClose/aCurrent" ), SIGNAL( triggered() ), workspace(), SLOT( projectCloseCurrent_triggered() ) );
	connect( menuBar()->action( "mProject/mClose/aAll" ), SIGNAL( triggered() ), workspace(), SLOT( projectCloseAll_triggered() ) );
	connect( menuBar()->action( "mProject/aSettings" ), SIGNAL( triggered() ), workspace(), SLOT( projectSettings_triggered() ) );
	connect( pRecentsManager::instance(), SIGNAL( openProjectRequested( const QString& ) ), fileManager(), SLOT( openProject( const QString& ) ) );
	// plugins menu
	//connect( menuBar()->action( "mPlugins/aManage" ), SIGNAL( triggered() ), pluginsManager(), SLOT( manageRequested() ) );
	// help menu
	connect( menuBar()->action( "mHelp/aAbout" ), SIGNAL( triggered() ), workspace(), SLOT( helpAboutApplication_triggered() ) );
	connect( menuBar()->action( "mHelp/aAboutQt" ), SIGNAL( triggered() ), workspace(), SLOT( helpAboutQt_triggered() ) );
#ifdef __COVERAGESCANNER__
	connect( menuBar()->action( "mHelp/aTestReport" ), SIGNAL( triggered() ), workspace(), SLOT( helpTestReport_triggered() ) );
#endif
}

void UIMain::initGui()
{
	// window icon
	setWindowIcon( menuBar()->action( "mHelp/aAbout" )->icon() );

	// set icon size for toolbar
	setIconSize( QSize( 16, 16 ) );

	// set central widget
	setCentralWidget( workspace() );
	
	// add qscintilla search dock
	dockToolBar( Qt::BottomToolBarArea )->addDock( pSearch::instance(), tr( "Search - Replace" ), QIcon( ":/edit/icons/edit/search.png" ) );
	
	// add messagebox dock
	dockToolBar( Qt::BottomToolBarArea )->addDock( UIMessageBox::instance(), tr( "Console Manager" ), QIcon( ":/templates/icons/templates/console.png" ) );

	// create statusbar
	statusBar()->show();
}
