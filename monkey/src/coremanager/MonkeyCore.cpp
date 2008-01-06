#include "MonkeyCore.h"
#include "pMonkeyStudio.h"
#include "pSettings.h"
#include "PluginsManager.h"
#include "UIMain.h"
#include "pMenuBar.h"
#include "pRecentsManager.h"
#include "pToolsManager.h"
#include "UIProjectsManager.h"
#include "pFileManager.h"
#include "pWorkspace.h"
#include "pConsoleManager.h"
#include "pSearch.h"
#include <QStatusBar>

#include "pDockToolBar.h"
#include "UISettings.h"
#include "UITranslator.h"

#include <QSplashScreen>

void showMessage( QSplashScreen* s, const QString& m )
{ s->showMessage( m, Qt::AlignRight | Qt::AlignBottom, Qt::white ); }

QHash<const QMetaObject*, QObject*> MonkeyCore::mInstances;

void MonkeyCore::init()
{
	// create splashscreen
	QSplashScreen splash( QPixmap( ":/application/icons/application/splashscreen.png" ) );
	QFont ft( splash.font() );
#ifndef Q_OS_WIN
	ft.setPointSize( ft.pointSize() -2 );
#endif
	ft.setBold( true );
	splash.setFont( ft );
	splash.show();
	
	// init pSettings
	pSettings::setIniInformations();
	
	// restore application style
	showMessage( &splash, tr( "Initializing Style..." ) );
	qApp->setStyle( settings()->value( "MainWindow/Style", "plastique" ).toString() );
	
	// init translation
	showMessage( &splash, tr( "Initializing Translation..." ) );
	if ( !settings()->value( "Translations/Accepted" ).toBool() )
		UITranslator::instance()->exec();
	pMonkeyStudio::loadTranslations();
	
	// start console manager
	showMessage( &splash, tr( "Initializing Console..." ) );
	consoleManager();

	// init main window
	showMessage( &splash, tr( "Initializing Main Window..." ) );
	mainWindow()->initGui();
	
	// init pluginsmanager
	showMessage( &splash, tr( "Initializing Plugins..." ) );
	pluginsManager()->loadsPlugins();
	
	// restore window state
	showMessage( &splash, tr( "Restoring Workspace..." ) );
	mainWindow()->restoreState();
	
	// show main window
	mainWindow()->show();
	
	// restore session
	showMessage( &splash, tr( "Restoring Session..." ) );
	if ( pMonkeyStudio::restoreSessionOnStartup() )
		workspace()->fileSessionRestore_triggered();
	
	// ready
	showMessage( &splash, tr( "%1 v%2 Ready !" ).arg( PROGRAM_NAME, PROGRAM_VERSION ) );

	// finish splashscreen
	splash.finish( mainWindow() );
	
	// show settings dialog the first time user start program
	if ( settings()->value( "FirstTimeRunning", true ).toBool() )
		if ( UISettings::instance()->exec() )
			settings()->setValue( "FirstTimeRunning", false );
}

pSettings* MonkeyCore::settings()
{
	if ( !mInstances.contains( &pSettings::staticMetaObject ) )
		mInstances[&pSettings::staticMetaObject] = new pSettings();
	return qobject_cast<pSettings*>( mInstances[&pSettings::staticMetaObject] );
}

PluginsManager* MonkeyCore::pluginsManager()
{
	if ( !mInstances.contains( &PluginsManager::staticMetaObject ) )
		mInstances[&PluginsManager::staticMetaObject] = new PluginsManager();
	return qobject_cast<PluginsManager*>( mInstances[&PluginsManager::staticMetaObject] );
}

UIMain* MonkeyCore::mainWindow()
{
	if ( !mInstances.contains( &UIMain::staticMetaObject ) )
		mInstances[&UIMain::staticMetaObject] = new UIMain();
	return qobject_cast<UIMain*>( mInstances[&UIMain::staticMetaObject] );
}

pMenuBar* MonkeyCore::menuBar()
{
	if ( !mInstances.contains( &pMenuBar::staticMetaObject ) )
		mInstances[&pMenuBar::staticMetaObject] = pMenuBar::instance(); //new pMenuBar();
	return qobject_cast<pMenuBar*>( mInstances[&pMenuBar::staticMetaObject] );
}

pRecentsManager* MonkeyCore::recentsManager()
{
	if ( !mInstances.contains( &pRecentsManager::staticMetaObject ) )
		mInstances[&pRecentsManager::staticMetaObject] = pRecentsManager::instance( mainWindow() ); //new pRecentsManager();
	return qobject_cast<pRecentsManager*>( mInstances[&pRecentsManager::staticMetaObject] );
}

pToolsManager* MonkeyCore::toolsManager()
{
	if ( !mInstances.contains( &pToolsManager::staticMetaObject ) )
		mInstances[&pToolsManager::staticMetaObject] = pToolsManager::instance( mainWindow() ); //new pToolsManager();
	return qobject_cast<pToolsManager*>( mInstances[&pToolsManager::staticMetaObject] );
}

UIProjectsManager* MonkeyCore::projectsManager()
{
	if ( !mInstances.contains( &UIProjectsManager::staticMetaObject ) )
		mInstances[&UIProjectsManager::staticMetaObject] = UIProjectsManager::instance( mainWindow() ); //new UIProjectsManager();
	return qobject_cast<UIProjectsManager*>( mInstances[&UIProjectsManager::staticMetaObject] );
}

pFileManager* MonkeyCore::fileManager()
{
	if ( !mInstances.contains( &pFileManager::staticMetaObject ) )
		mInstances[&pFileManager::staticMetaObject] = pFileManager::instance( qApp ); //new pFileManager();
	return qobject_cast<pFileManager*>( mInstances[&pFileManager::staticMetaObject] );
}

pWorkspace* MonkeyCore::workspace()
{
	if ( !mInstances.contains( &pWorkspace::staticMetaObject ) )
		mInstances[&pWorkspace::staticMetaObject] = pWorkspace::instance( mainWindow() ); //new pWorkspace();
	return qobject_cast<pWorkspace*>( mInstances[&pWorkspace::staticMetaObject] );
}

pConsoleManager* MonkeyCore::consoleManager()
{
	if ( !mInstances.contains( &pConsoleManager::staticMetaObject ) )
		mInstances[&pConsoleManager::staticMetaObject] = pConsoleManager::instance( qApp ); //new pConsoleManager();
	return qobject_cast<pConsoleManager*>( mInstances[&pConsoleManager::staticMetaObject] );
}

pSearch* MonkeyCore::searchDock()
{
	if ( !mInstances.contains( &pSearch::staticMetaObject ) )
		mInstances[&pSearch::staticMetaObject] = pSearch::instance(); //new pSearch();
	return qobject_cast<pSearch*>( mInstances[&pSearch::staticMetaObject] );
}

QStatusBar* MonkeyCore::statusBar()
{
	if ( !mInstances.contains( &QStatusBar::staticMetaObject ) )
		mInstances[&QStatusBar::staticMetaObject] = new QStatusBar();
	return qobject_cast<QStatusBar*>( mInstances[&QStatusBar::staticMetaObject] );
	//StatusBar::self()->setText( StatusBar::tStatusTip, tr( "%1 v%2 Ready !" ).arg( PROGRAM_NAME, PROGRAM_VERSION ), 15000 );
}
