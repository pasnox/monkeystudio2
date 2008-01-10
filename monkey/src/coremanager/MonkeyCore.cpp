#include "MonkeyCore.h"
#include "pMonkeyStudio.h"
#include "Settings.h"
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
#include "pActionManager.h"

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
	qApp->setStyle( settings()->value( "MainWindow/Style", "system" ).toString() );
	
	// set default settings if first time running
	if ( settings()->value( "FirstTimeRunning", true ).toBool() )
		settings()->setDefaultSettings();
	
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
	
	// init shortcuts editor
	showMessage( &splash, tr( "Initializing Shortcuts..." ) );
	MonkeyCore::actionManager()->setSettings( settings() );
	
	// init pluginsmanager
	showMessage( &splash, tr( "Initializing Plugins..." ) );
	pluginsManager()->loadsPlugins();
	
	// restore window state
	showMessage( &splash, tr( "Restoring Workspace..." ) );
	mainWindow()->setSettings( settings() );
	
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
	{
		// execute settings dialog
		if ( UISettings::instance()->exec() )
			settings()->setValue( "FirstTimeRunning", false );
	}
}

pSettings* MonkeyCore::settings()
{
	if ( !mInstances.contains( &Settings::staticMetaObject ) )
		mInstances[&Settings::staticMetaObject] = new Settings( qApp );
	return qobject_cast<Settings*>( mInstances[&Settings::staticMetaObject] );
}

PluginsManager* MonkeyCore::pluginsManager()
{
	if ( !mInstances.contains( &PluginsManager::staticMetaObject ) )
		mInstances[&PluginsManager::staticMetaObject] = new PluginsManager( qApp );
	return qobject_cast<PluginsManager*>( mInstances[&PluginsManager::staticMetaObject] );
}

UIMain* MonkeyCore::mainWindow()
{
	if ( !mInstances.contains( &UIMain::staticMetaObject ) )
		mInstances[&UIMain::staticMetaObject] = new UIMain();
	return qobject_cast<UIMain*>( mInstances[&UIMain::staticMetaObject] );
}

pMenuBar* MonkeyCore::menuBar()
{ return mainWindow()->menuBar(); }

pActionManager* MonkeyCore::actionManager()
{ return pActionManager::instance(); }

pRecentsManager* MonkeyCore::recentsManager()
{
	if ( !mInstances.contains( &pRecentsManager::staticMetaObject ) )
		mInstances[&pRecentsManager::staticMetaObject] = new pRecentsManager( qApp );
	return qobject_cast<pRecentsManager*>( mInstances[&pRecentsManager::staticMetaObject] );
}

pToolsManager* MonkeyCore::toolsManager()
{
	if ( !mInstances.contains( &pToolsManager::staticMetaObject ) )
		mInstances[&pToolsManager::staticMetaObject] = new pToolsManager( qApp );
	return qobject_cast<pToolsManager*>( mInstances[&pToolsManager::staticMetaObject] );
}

UIProjectsManager* MonkeyCore::projectsManager()
{
	if ( !mInstances.contains( &UIProjectsManager::staticMetaObject ) )
		mInstances[&UIProjectsManager::staticMetaObject] = new UIProjectsManager( mainWindow() );
	return qobject_cast<UIProjectsManager*>( mInstances[&UIProjectsManager::staticMetaObject] );
}

pFileManager* MonkeyCore::fileManager()
{
	if ( !mInstances.contains( &pFileManager::staticMetaObject ) )
		mInstances[&pFileManager::staticMetaObject] = new pFileManager( qApp );
	return qobject_cast<pFileManager*>( mInstances[&pFileManager::staticMetaObject] );
}

pWorkspace* MonkeyCore::workspace()
{
	if ( !mInstances.contains( &pWorkspace::staticMetaObject ) )
		mInstances[&pWorkspace::staticMetaObject] = new pWorkspace( mainWindow() );
	return qobject_cast<pWorkspace*>( mInstances[&pWorkspace::staticMetaObject] );
}

pConsoleManager* MonkeyCore::consoleManager()
{
	if ( !mInstances.contains( &pConsoleManager::staticMetaObject ) )
		mInstances[&pConsoleManager::staticMetaObject] = new pConsoleManager( qApp );
	return qobject_cast<pConsoleManager*>( mInstances[&pConsoleManager::staticMetaObject] );
}

pSearch* MonkeyCore::searchDock()
{
	if ( !mInstances.contains( &pSearch::staticMetaObject ) )
		mInstances[&pSearch::staticMetaObject] = new pSearch();
	return qobject_cast<pSearch*>( mInstances[&pSearch::staticMetaObject] );
}

QStatusBar* MonkeyCore::statusBar()
{
	if ( !mInstances.contains( &QStatusBar::staticMetaObject ) )
		mInstances[&QStatusBar::staticMetaObject] = new QStatusBar( mainWindow() );
	return qobject_cast<QStatusBar*>( mInstances[&QStatusBar::staticMetaObject] );
	//StatusBar::self()->setText( StatusBar::tStatusTip, tr( "%1 v%2 Ready !" ).arg( PROGRAM_NAME, PROGRAM_VERSION ), 15000 );
}
