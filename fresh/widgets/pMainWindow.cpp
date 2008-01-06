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
	: QMainWindow( w, f )
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

void pMainWindow::saveState()
{
	dockToolBarManager()->saveState();
	pSettings().saveState( this );
}

void pMainWindow::restoreState()
{
	dockToolBarManager()->restoreState();
	pSettings().restoreState( this );
}
