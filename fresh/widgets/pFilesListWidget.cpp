/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pFilesListWidget.cpp
** Date      : 2008-01-14T00:27:44
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
#include "pFilesListWidget.h"
#include "pExtendedWorkspace.h"

#include <QListWidget>
#include <QMainWindow>
#include <QDropEvent>

pFilesListWidget::pFilesListWidget( const QString& s, pExtendedWorkspace* p )
	: QDockWidget( s ), mWorkspace( p )
{
	Q_ASSERT ( mWorkspace );
	setParent( mWorkspace );
	setObjectName( "FilesListWidget" );
	setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	setAcceptDrops( true );
	setContextMenuPolicy( Qt::CustomContextMenu );
	setWidget( mList = new QListWidget() );
	mList->setDragDropMode( QAbstractItemView::InternalMove );
	mList->installEventFilter( this );
	// connection
	connect( mList, SIGNAL( currentRowChanged( int ) ), mWorkspace, SLOT( setCurrentIndex( int ) ) );
	connect( mWorkspace, SIGNAL( currentChanged( int ) ), this, SLOT( setCurrentRow( int ) ) );
	connect( mWorkspace, SIGNAL( modifiedChanged( int, bool ) ), this, SLOT( modifiedChanged( int, bool ) ) );
    connect( mWorkspace, SIGNAL( docTitleChanged( int, const QString& ) ), this, SLOT( docTitleChanged( int, const QString& ) ) );
    connect( mWorkspace, SIGNAL( documentInserted( int, const QString&, const QIcon& ) ), this, SLOT( documentInserted( int, const QString&, const QIcon& ) ) );
	connect( mWorkspace, SIGNAL( documentAboutToClose( int ) ), this, SLOT( documentAboutToClose( int ) ) );
}

bool pFilesListWidget::eventFilter( QObject* o, QEvent* e )
{
	static int i = -1;
	static QListWidgetItem* it = 0;
	if ( e->type() == QEvent::ChildAdded )
	{
		i = mList->currentRow();
		it = mList->item( i );
	}
	else if ( e->type() == QEvent::ChildRemoved )
	{
		int j = mList->row( it );
		if ( i != j )
		{
			mWorkspace->moveDocument( i, j );
			mList->setCurrentRow( j );
		}
	}
	return QDockWidget::eventFilter( o, e );
}

void pFilesListWidget::dragEnterEvent( QDragEnterEvent* e )
{
	// if correct mime and same tabbar
	if ( e->mimeData()->hasUrls() )
	{
		// accept drag
		e->acceptProposedAction();
	}
	// default event
	QDockWidget::dragEnterEvent( e );
}

void pFilesListWidget::dropEvent( QDropEvent* e )
{
	if ( e->mimeData()->hasUrls() )
		emit urlsDropped( e->mimeData()->urls () );
	// default event
	QDockWidget::dropEvent( e );
}

void pFilesListWidget::setItemToolTip( int i, const QString& s )
{
	if ( QListWidgetItem* it = mList->item( i ) )
		it->setToolTip( s );
}

void pFilesListWidget::modifiedChanged( int i, bool b )
{ mList->item( i )->setIcon( b ? QIcon( ":/file/icons/file/save.png" ) : QIcon( ":/file/icons/file/transparent.png" ) ); }

void pFilesListWidget::docTitleChanged( int i, const QString& s )
{ mList->item( i )->setText( s ); }

void pFilesListWidget::documentInserted( int i, const QString& s, const QIcon& )
{ mList->insertItem( i, s ); }

void pFilesListWidget::documentAboutToClose( int i )
{ delete mList->takeItem( i ); }

void pFilesListWidget::setCurrentRow( int i )
{ mList->setCurrentRow( i ); }
