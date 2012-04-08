#include <QApplication>
#include <QWidget>

#include <qCtagsSense.h>

int main( int argc, char** argv )
{
    // create application
    QApplication app( argc, argv );

    // init application
    app.setApplicationName( "qctagssense" );
    
    QWidget w;
    w.show();
    
    qCtagsSense::Instance sense( &w );

    // connection
    QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

    // start application
    return app.exec();
}
