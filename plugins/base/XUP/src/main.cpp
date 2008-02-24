#include <QApplication>
#include <QMainWindow>

#include "UIProjectsManager.h"
#include "QMakeProjectItem.h"

int main( int argc, char** argv )
{
	// create application
	QApplication app( argc, argv );

	// init application
	app.setApplicationName( "XUP Project Manager" );
	
	QMainWindow w;
	UIProjectsManager d;
	w.addDockWidget( Qt::RightDockWidgetArea, &d );
	w.show();

	// connection
	QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

	// start application
	return app.exec();
}
