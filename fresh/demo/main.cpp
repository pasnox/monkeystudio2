/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : main.cpp
** Date      : 2008-01-14T00:27:35
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
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
