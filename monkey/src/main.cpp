#include <QApplication>
#include <QMessageBox>

#include "MonkeyCore.h"

int main( int argc, char** argv )
{
	// check qt version
	QT_REQUIRE_VERSION( argc, argv, "4.3.2" );
	// create application
	QApplication a( argc, argv );
	a.setApplicationName( PROGRAM_NAME );
	a.setOrganizationName( PROGRAM_NAME );
	a.setOrganizationDomain( PROGRAM_DOMAIN );
	QObject::connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	// init monkey studio core
	MonkeyCore::init();
	// start application
	return a.exec();
}
