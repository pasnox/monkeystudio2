/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pMainWindow.cpp
** Date      : 2008-01-14T00:27:46
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
#include "pMainWindow.h"
#include "../objects/pSettings.h"
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
		dockToolBarManager()->setSettings( s );
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
