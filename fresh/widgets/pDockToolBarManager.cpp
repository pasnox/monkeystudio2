/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pDockToolBarManager.cpp
** Date      : 2008-01-14T00:27:41
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
#include "pDockToolBarManager.h"
#include "pDockToolBar.h"
#include "pSettings.h"

#include <QMainWindow>
#include <QDockWidget>
#include <QAbstractButton>
#include <QAction>

pDockToolBarManager::pDockToolBarManager( QMainWindow* w )
	: QObject( w ), mMain( w ), mSettings( 0 )
{ Q_ASSERT( w != 0 ); }

QMainWindow* pDockToolBarManager::mainWindow() const
{ return mMain; }

void pDockToolBarManager::setSettings( pSettings* s )
{
	if ( mSettings != s )
		mSettings = s;
}

pSettings* pDockToolBarManager::settings()
{ return mSettings; }

pDockToolBar* pDockToolBarManager::bar( Qt::ToolBarArea a )
{
	// if toolbar not exists, create it
	if ( !mBars.contains( a ) )
	{
		switch ( a )
		{
		case Qt::TopToolBarArea:
			mBars[a] = new pDockToolBar( this, Qt::Horizontal, mMain );
			mBars[a]->setObjectName( "pDockToolBarTop" );
			mBars[a]->setWindowTitle( tr( "Top DockToolBar" ) );
			break;
		case Qt::BottomToolBarArea:
			mBars[a] = new pDockToolBar( this, Qt::Horizontal, mMain );
			mBars[a]->setObjectName( "pDockToolBarBottom" );
			mBars[a]->setWindowTitle( tr( "Bottom DockToolBar" ) );
			break;
		case Qt::LeftToolBarArea:
			mBars[a] = new pDockToolBar( this, Qt::Vertical, mMain );
			mBars[a]->setObjectName( "pDockToolBarLeft" );
			mBars[a]->setWindowTitle( tr( "Left DockToolBar" ) );
			break;
		case Qt::RightToolBarArea:
			mBars[a] = new pDockToolBar( this, Qt::Vertical, mMain );
			mBars[a]->setObjectName( "pDockToolBarRight" );
			mBars[a]->setWindowTitle( tr( "Right DockToolBar" ) );
			break;
		default:
			return 0;
			break;
		}
		// add toolbar to mainwindow
		mMain->addToolBar( a, mBars[a] );
		// hide
		mBars[a]->hide();
		// track dock bar changed
		connect( mBars[a], SIGNAL( dockWidgetAreaChanged( QDockWidget*, pDockToolBar* ) ), this, SLOT( dockWidgetAreaChanged( QDockWidget*, pDockToolBar* ) ) );
	}
	// return existings toolbar
	return mBars[a];
}

Qt::ToolBarArea pDockToolBarManager::dockWidgetAreaToToolBarArea( Qt::DockWidgetArea a )
{
	// convert dock area to toolbar area
	switch ( a )
	{
	case Qt::LeftDockWidgetArea:
		return Qt::LeftToolBarArea;
		break;
	case Qt::RightDockWidgetArea:
		return Qt::RightToolBarArea;
		break;
	case Qt::TopDockWidgetArea:
		return Qt::TopToolBarArea;
		break;
	case Qt::BottomDockWidgetArea:
		return Qt::BottomToolBarArea;
		break;
	default:
		return Qt::BottomToolBarArea;
		break;
	}
}

Qt::DockWidgetArea pDockToolBarManager::toolBarAreaToDockWidgetArea( Qt::ToolBarArea a )
{
	// convert toolbar area to dock widget area
	switch ( a )
	{
	case Qt::LeftToolBarArea:
		return Qt::LeftDockWidgetArea;
		break;
	case Qt::RightToolBarArea:
		return Qt::RightDockWidgetArea;
		break;
	case Qt::TopToolBarArea:
		return Qt::TopDockWidgetArea;
		break;
	case Qt::BottomToolBarArea:
		return Qt::BottomDockWidgetArea;
		break;
	default:
		return Qt::BottomDockWidgetArea;
		break;
	}
}

QBoxLayout::Direction pDockToolBarManager::toolBarAreaToBoxLayoutDirection( Qt::ToolBarArea a )
{
	// convert toolbar area to dock widget area
	switch ( a )
	{
	case Qt::LeftToolBarArea:
		return QBoxLayout::BottomToTop;
		break;
	case Qt::RightToolBarArea:
		return QBoxLayout::TopToBottom;
		break;
	case Qt::TopToolBarArea:
	case Qt::BottomToolBarArea:
		return QBoxLayout::LeftToRight;
		break;
	default:
		return QBoxLayout::LeftToRight;
		break;
	}
}

void pDockToolBarManager::dockWidgetAreaChanged( QDockWidget* d, pDockToolBar* f )
{
	// remove dock from old toolbar
	f->removeDock( d );
	// add dock to new toolbar
	bar( dockWidgetAreaToToolBarArea( mMain->dockWidgetArea( d ) ) )->addDock( d, d->windowTitle(), d->windowIcon().pixmap( QSize( 24, 24 ), QIcon::Normal, QIcon::On ) );
}

void pDockToolBarManager::restoreState( pDockToolBar* p )
{
	// need settings
	if ( !settings() )
		return;
	// get the bar to restore
	QStringList l;
	if ( p )
		l << QString::number( mMain->toolBarArea( p ) );
	else
	{
		settings()->beginGroup( "MainWindow/Docks" );
		l = settings()->childGroups();
		settings()->endGroup();
	}
	// for docktoolbar
	foreach ( QString i, l )
	{
		// get bar
		p = bar( (Qt::ToolBarArea)i.toInt() );
		// if got bar
		if ( p )
		{
			// restore exclusive state
			p->setExclusive( settings()->value( QString( "MainWindow/Docks/%1/Exclusive" ).arg( i ), true ).toBool() );
			// bar datas
			QStringList mList = settings()->value( QString( "MainWindow/Docks/%1/Widgets" ).arg( i ), QStringList() ).toStringList();
			// for each entry
			foreach ( QString e, mList )
			{
				// get dock
				QDockWidget* d = mMain->findChild<QDockWidget*>( e );
				// restore dock area
				if ( d )
					p->addDock( d, d->windowTitle(), d->windowIcon() );
			}
		}
	}
}

void pDockToolBarManager::saveState( pDockToolBar* p )
{
	// need settings
	if ( !settings() )
		return;
	// get the bar to save
	QList<pDockToolBar*> l;
	if ( p )
		l << p;
	else
		l << mBars.values();
	// for each docktoolbar
	foreach ( pDockToolBar* tb, l )
	{
		// list to stock checked button
		QStringList mList;
		// for each dock in docktoolbar
		foreach ( QDockWidget* d, tb->docks() )
			mList << d->objectName();
		// write datas
		settings()->setValue( QString( "MainWindow/Docks/%1/Exclusive" ).arg( mMain->toolBarArea( tb ) ), tb->exclusive() );
		settings()->setValue( QString( "MainWindow/Docks/%1/Widgets" ).arg( mMain->toolBarArea( tb ) ), mList );
	}
}
