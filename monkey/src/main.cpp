#include <QApplication>
#include <QSplashScreen>
#include <QLibraryInfo>
#include <QTranslator>
#include <QLocale>
#include <QErrorMessage>

#include "UITranslator.h"
#include "UISettings.h"
#include "UIMain.h"
#include "pSettings.h"
#include "ConsoleManager.h"

void showMessage( QSplashScreen* s, const QString& m )
{
	s->showMessage( m, Qt::AlignRight | Qt::AlignTop, Qt::red );
}

int main( int argc, char** argv )
{
	// create application
	QApplication a( argc, argv );

	// create splashscreen
	QSplashScreen splash( QPixmap( ":/application/application/splashscreen.png" ) );

	// change splashscreen font
	QFont f( splash.font() );
	f.setPointSize( 12 );
	f.setBold( true );
	splash.setFont( f );

	// show splash screen
	splash.show();

	// init application
	showMessage( &splash, QObject::tr( "Initializing Application..." ) );
	a.setApplicationName( PROGRAM_NAME );
	a.setOrganizationName( PROGRAM_NAME );
	a.setOrganizationDomain( PROGRAM_DOMAIN );
	
	// start console manager
	showMessage( &splash, QObject::tr( "Starting Console Manager..." ) );
	ConsoleManager::instance()->start();

	// restore application style
	a.setStyle( pSettings::instance()->value( "MainWindow/Style", "plastique" ).toString() );
	
	// init translation
	showMessage( &splash, QObject::tr( "Initializing Translation..." ) );
	// qt translation
	QString resourceDir = QLibraryInfo::location( QLibraryInfo::TranslationsPath );
	// setting qt translation
	QTranslator qtTranslator;
	qtTranslator.load( QString( "qt_" ) + QLocale::system().name(), resourceDir );
	a.installTranslator( &qtTranslator );
	// setting assistant translation
	QTranslator assistantTranslator;
	assistantTranslator.load( QString( "assistant_" ) + QLocale::system().name(), resourceDir );
	a.installTranslator( &assistantTranslator );
	// setting designer translation
	QTranslator designerTranslator;
	designerTranslator.load( QString( "designer_" ) + QLocale::system().name(), resourceDir );
	a.installTranslator( &designerTranslator );

	QString mLanguage = pSettings::instance()->value( "Language" ).toString();

	if ( mLanguage.isEmpty() )
		if ( !UITranslator::instance( &splash )->exec() )
			mLanguage = "english";

	if ( mLanguage != "english" )
	{
		QTranslator t;
		t.load( QString( "%1/monkey2_%3" ).arg( pSettings::instance()->value( "Paths/Translations" ).toString(), mLanguage ) );
		a.installTranslator( &t );
	}

	// show settings dialog the first time user start program
	if ( pSettings::instance()->value( "FirstTimeRunning", true ).toBool() )
		if ( UISettings::instance()->exec() )
			pSettings::instance()->setValue( "FirstTimeRunning", false );

	// init main window
	showMessage( &splash, QObject::tr( "Initializing Main Window..." ) );
	UIMain::instance()->setWindowTitle( QObject::tr( "%1 v%2 - %3, The Monkey Studio Team" ).arg( PROGRAM_NAME, PROGRAM_VERSION, PROGRAM_COPYRIGHTS ) );

/*
	// menu tools
	showMessage( &splash, tr( "Initializing Tools Manager..." ) );
	ToolsManager::self( UIMain::self() );

	// init recents
	showMessage( &splash, tr( "Initializing Recents Manager..." ) );
	RecentsManager::self( UIMain::self() );

	// init pluginsmanager
	showMessage( &splash, tr( "Initializing Plugins Manager..." ) );
	addLibraryPath( "plugins" );
	PluginsManager::self( UIMain::self() )->loadsPlugins();
*/
	// ready
	showMessage( &splash, QObject::tr( "%1 v%2 Ready !" ).arg( PROGRAM_NAME, PROGRAM_VERSION ) );
	//StatusBar::self()->setText( StatusBar::tStatusTip, tr( "%1 v%2 Ready !" ).arg( PROGRAM_NAME, PROGRAM_VERSION ), 15000 );

	// restore window state
	UIMain::instance()->restoreState();

	// show main window
	UIMain::instance()->show();

	// finish splashscreen
	splash.finish( UIMain::instance() );

	// connection
	QObject::connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	
	QCommandList l;
	
	pCommand* c;
	
	c = new pCommand;
	c->setText( "Executing kcalc..." );
	c->setCommand( "kcal" );
	c->setSkipOnError( true );
	l << c;
	
	c = new pCommand;
	c->setText( "Executing assistant..." );
	c->setCommand( "assistant" );
	c->setSkipOnError( true );
	l << c;
	
	c = new pCommand;
	c->setText( "Executing konqueror..." );
	c->setCommand( "konquero" );
	c->setSkipOnError( true );
	l << c;
	
	ConsoleManager::instance()->addCommands( l );
	
	// start application
	int i = a.exec();
	
	// stop console manager
	ConsoleManager::instance()->terminate();
	
	// return application result
	return i;
}
