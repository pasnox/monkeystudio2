/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pTabbedMainWindow.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class herits from pMainWindow and add a pTabbedWorkspace as central widget
 ********************************************************************************************************/
#include "pTabbedMainWindow.h"
#include "pTabbedWorkspace.h"

pTabbedMainWindow::pTabbedMainWindow( QWidget* w, Qt::WindowFlags f )
	: pMainWindow( w, f ), mWorkspace( new pTabbedWorkspace )
{
	// init tabbed workspace
	setCentralWidget( mWorkspace );
}

pTabbedWorkspace* pTabbedMainWindow::tabbedWorkspace()
{
	return mWorkspace;
}

void pTabbedMainWindow::saveState()
{
	pMainWindow::saveState();
}

void pTabbedMainWindow::restoreState()
{
	pMainWindow::restoreState();
}
