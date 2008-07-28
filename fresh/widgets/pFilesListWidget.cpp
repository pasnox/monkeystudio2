/****************************************************************************
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
****************************************************************************/
#include "pFilesListWidget.h"
#include "pExtendedWorkspace.h"

#include <QListWidget>
#include <QMainWindow>
#include <QDropEvent>

/*!
	\details Create a new pFilesListWidget instance
	\param title The dock title
	\param workspace The workspace on where operate the dock
*/
pFilesListWidget::pFilesListWidget( const QString& title, pExtendedWorkspace* workspace )
	: pDockWidget( title ), mWorkspace( workspace )
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
	// init icons
	mModifiedIcon = QIcon( QPixmap( ":/file/icons/file/save.png" ) );
	mNonModifiedIcon = QIcon( QPixmap( ":/file/icons/file/transparent.png" ) );
	// connection
	connect( mList, SIGNAL( currentRowChanged( int ) ), mWorkspace, SLOT( setCurrentIndex( int ) ) );
	connect( mWorkspace, SIGNAL( currentChanged( int ) ), this, SLOT( setCurrentRow( int ) ) );
	connect( mWorkspace, SIGNAL( modifiedChanged( int, bool ) ), this, SLOT( modifiedChanged( int, bool ) ) );
	connect( mWorkspace, SIGNAL( docTitleChanged( int, const QString& ) ), this, SLOT( docTitleChanged( int, const QString& ) ) );
	connect( mWorkspace, SIGNAL( documentInserted( int, const QString&, const QIcon& ) ), this, SLOT( documentInserted( int, const QString&, const QIcon& ) ) );
	connect( mWorkspace, SIGNAL( documentAboutToClose( int ) ), this, SLOT( documentAboutToClose( int ) ) );
}

bool pFilesListWidget::eventFilter( QObject* object, QEvent* event )
{
	static int i = -1;
	static QListWidgetItem* it = 0;
	if ( event->type() == QEvent::ChildAdded )
	{
		i = mList->currentRow();
		it = mList->item( i );
	}
	else if ( event->type() == QEvent::ChildRemoved )
	{
		int j = mList->row( it );
		if ( i != j )
		{
			mWorkspace->moveDocument( i, j );
			mList->setCurrentRow( j );
		}
	}
	return QDockWidget::eventFilter( object, event );
}

void pFilesListWidget::dragEnterEvent( QDragEnterEvent* event )
{
	// if correct mime and same tabbar
	if ( event->mimeData()->hasUrls() )
	{
		// accept drag
		event->acceptProposedAction();
	}
	// default event
	QDockWidget::dragEnterEvent( event );
}

void pFilesListWidget::dropEvent( QDropEvent* event )
{
	if ( event->mimeData()->hasUrls() )
		emit urlsDropped( event->mimeData()->urls () );
	// default event
	QDockWidget::dropEvent( event );
}

/*!
	\details Set the toolTip for item id
	\param id The item id
	\param toolTip The toolTip to set
*/
void pFilesListWidget::setItemToolTip( int id, const QString& toolTip )
{
	if ( QListWidgetItem* it = mList->item( id ) )
		it->setToolTip( toolTip );
}

void pFilesListWidget::modifiedChanged( int id, bool modified )
{
	if ( QListWidgetItem* it = mList->item( id ) )
		it->setIcon( modified ? mModifiedIcon : mNonModifiedIcon );
}

void pFilesListWidget::docTitleChanged( int id, const QString& title )
{
	if ( QListWidgetItem* it = mList->item( id ) )
		it->setText( title );
}

void pFilesListWidget::documentInserted( int id, const QString& title, const QIcon& /*icon*/ )
{ mList->insertItem( id, title ); }

void pFilesListWidget::documentAboutToClose( int id )
{ delete mList->takeItem( id ); }

void pFilesListWidget::setCurrentRow( int id )
{ mList->setCurrentRow( id ); }
