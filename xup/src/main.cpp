#include <QApplication>

#include "UIProjectsManager.h"
#include "QMakeProjectItem.h"

int main( int argc, char** argv )
{
	// create application
	QApplication app( argc, argv );

	// init application
	app.setApplicationName( "XUP Project Manager" );
	
	// register items
	ProjectItem::registerItem();
	QMakeProjectItem::registerItem();
	
	UIProjectsManager d;
	d.show();

	// connection
	QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

	// start application
	return app.exec();
}
