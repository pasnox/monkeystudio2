#include <QApplication>
#include <QErrorMessage>
#include <QDockWidget>
#include <QTextEdit>

#include "pTabbedMainWindow.h"
#include "pMenuBar.h"
#include "pDockToolBar.h"
#include "pTabbedWorkspace.h"
#include "pActionManager.h"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	pTabbedMainWindow p;

	// set tabbed sample settings
	p.tabbedWorkspace()->setTabShape( QTabBar::RoundedNorth );
	p.tabbedWorkspace()->setTabMode( pTabbedWorkspace::tmMDI );
	p.tabbedWorkspace()->setDocumentMode( pTabbedWorkspace::dmMaximized );

	// set sample menu
	p.menuBar()->menu( "mTools" )->setTitle( "Tools" );
	QAction* a = p.menuBar()->action( "mTools/aShortcutsEditor", "Shortcuts Editor" );
	QObject::connect( a, SIGNAL( triggered() ), pActionManager::instance(), SLOT( showSettings() ) );

	// add sample dock widget
	for ( int i = 0; i < 5; i++ )
		p.dockToolBar( Qt::TopToolBarArea )->addDock( new QDockWidget, QString( "Qt Assistant %1" ).arg( i ), QPixmap( "icon.png" ) );

	// add sample document
	for ( int i = 0; i < 10; i++ )
	{
		QTextEdit* e = new QTextEdit;
		e->setWindowTitle( QString( "Tab: %1" ).arg( i ) );
		e->setPlainText( e->windowTitle() );
		p.tabbedWorkspace()->addTab( e, e->windowTitle() );
	}

	// restore window state
	p.restoreState();

	// show mainwindow
	p.show();

	// execute application
	return app.exec();
}
