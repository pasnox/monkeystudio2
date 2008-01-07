/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pMainWindow.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class herits from QMainWindow and add some lib features
 ********************************************************************************************************/
#include "pMainWindow.h"
#include "pSettings.h"
#include "pActionManager.h"
#include "pMenuBar.h"
#include "pDockToolBarManager.h"
#include "pDockToolBar.h"

pMainWindow::pMainWindow( QWidget* w, Qt::WindowFlags f )
	: QMainWindow( w, f ), mSettings( 0 )
{
	// set menu bar
	setMenuBar( new pMenuBar( this ) );
	// init toolbar
	dockToolBar( Qt::TopToolBarArea );
	dockToolBar( Qt::BottomToolBarArea );
	dockToolBar( Qt::LeftToolBarArea );
	dockToolBar( Qt::RightToolBarArea );
}

void pMainWindow::hideEvent( QHideEvent* )
{ saveState(); }

pMenuBar* pMainWindow::menuBar()
{ return qobject_cast<pMenuBar*>( QMainWindow::menuBar() ); }

pDockToolBarManager* pMainWindow::dockToolBarManager()
{ return pDockToolBarManager::instance( this ); }

pDockToolBar* pMainWindow::dockToolBar( Qt::ToolBarArea a )
{ return dockToolBarManager()->bar( a ); }

void pMainWindow::setSettings( pSettings* s )
{
	if ( mSettings != s )
	{
		mSettings = s;
		restoreState();
	}
}

pSettings* pMainWindow::settings()
{ return mSettings; }

void pMainWindow::saveState()
{
	if ( settings() )
	{
		dockToolBarManager()->saveState();
		settings()->saveState( this );
	}
}

void pMainWindow::restoreState()
{
	if ( settings() )
	{
		dockToolBarManager()->restoreState();
		settings()->restoreState( this );
	}
}
