#include <QApplication>

#include "qCtagsSenseBrowser.h"

int main( int argc, char** argv )
{
	// create application
	QApplication app( argc, argv );

	// init application
	app.setApplicationName( "qCtagsSense" );
	
	qCtagsSenseBrowser* browser = new qCtagsSenseBrowser;
	browser->setWindowTitle( "qCtagsSense" );
	browser->show();

	// connection
	QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

	// start application
	return app.exec();
}
