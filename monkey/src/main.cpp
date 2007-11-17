#include <QApplication>
#include <QSplashScreen>
#include <QFile>
#include <QErrorMessage>

#include "UIMain.h"
#include "UISettings.h"
#include "UITranslator.h"
#include "pWorkspace.h"
#include "pSettings.h"
#include "PluginsManager.h"
#include "pConsoleManager.h"
#include "pMonkeyStudio.h"

using namespace pMonkeyStudio;

void showMessage( QSplashScreen* s, const QString& m )
{ s->showMessage( m, Qt::AlignRight | Qt::AlignBottom, Qt::white ); }

int main( int argc, char** argv )
{
	// create application
	QApplication a( argc, argv );
	
	// create splashscreen
	QSplashScreen splash( QPixmap( ":/application/icons/application/splashscreen.png" ) );

	// change splashscreen font
	QFont ft( splash.font() );
	ft.setPointSize( 8 );
	ft.setBold( true );
	splash.setFont( ft );

	// show splash screen
	splash.show();
	
	// init translation
	showMessage( &splash, QObject::tr( "Initializing Translation..." ) );
	if ( !pSettings::instance()->value( "Translations/Accepted" ).toBool() )
		UITranslator::instance()->exec();
	loadTranslations();

	// init application
	showMessage( &splash, QObject::tr( "Initializing Application..." ) );
	a.setApplicationName( PROGRAM_NAME );
	a.setOrganizationName( PROGRAM_NAME );
	a.setOrganizationDomain( PROGRAM_DOMAIN );
	
	// start console manager
	showMessage( &splash, QObject::tr( "Starting Console Manager..." ) );
	pConsoleManager::instance();

	// restore application style
	showMessage( &splash, QObject::tr( "Setting Style..." ) );
	a.setStyle( pSettings::instance()->value( "MainWindow/Style", "plastique" ).toString() );
	
	/*
	// assign style sheet
	showMessage( &splash, QObject::tr( "Loading Style Sheet..." ) );
	QFile fs( ":/stylesheets/stylesheets/default.css" );
	fs.open( QFile::ReadOnly );
	a.setStyleSheet( fs.readAll() );
	fs.close();
	*/
	// show settings dialog the first time user start program
	if ( pSettings::instance()->value( "FirstTimeRunning", true ).toBool() )
		if ( UISettings::instance()->exec() )
			pSettings::instance()->setValue( "FirstTimeRunning", false );

	// init main window
	showMessage( &splash, QObject::tr( "Initializing Main Window..." ) );
	UIMain::instance()->setWindowTitle( QObject::tr( "%1 v%2 - %3, The Monkey Studio Team" ).arg( PROGRAM_NAME, PROGRAM_VERSION, PROGRAM_COPYRIGHTS ) );
	
	// init pluginsmanager
	showMessage( &splash, QObject::tr( "Initializing Plugins Manager..." ) );
	a.addLibraryPath( "plugins" );
	PluginsManager::instance()->loadsPlugins();
	
	// restore window state
	showMessage( &splash, QObject::tr( "Restoring Workspace..." ) );
	UIMain::instance()->restoreState();
	
	// show main window
	UIMain::instance()->show();
	
	// restore session
	showMessage( &splash, QObject::tr( "Restoring Session..." ) );
	if ( restoreSessionOnStartup() )
		UIMain::instance()->workspace()->fileSessionRestore_triggered();

	// ready
	showMessage( &splash, QObject::tr( "%1 v%2 Ready !" ).arg( PROGRAM_NAME, PROGRAM_VERSION ) );
	//StatusBar::self()->setText( StatusBar::tStatusTip, tr( "%1 v%2 Ready !" ).arg( PROGRAM_NAME, PROGRAM_VERSION ), 15000 );

	// finish splashscreen
	splash.finish( UIMain::instance() );

	// connection
	QObject::connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	
	// start application
	return a.exec();
}
