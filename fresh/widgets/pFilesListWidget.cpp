/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/12/29
 * AUTHOR       : Kopats Andrei aka hlamer( hlamer@tut.by )
 * FILENAME     : pFilesListWidget.cpp
 * LICENSE      : GPL
 * COMMENTARY   : Widget with list of files, opened on workspace, for activating needed file
 ********************************************************************************************************/
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
	// connection
	connect( mList, SIGNAL( currentRowChanged( int ) ), mWorkspace, SLOT( setCurrentIndex( int ) ) );
	connect( mWorkspace, SIGNAL( currentChanged( int ) ), this, SLOT( setCurrentRow( int ) ) );
	connect( mWorkspace, SIGNAL( modifiedChanged( int, bool ) ), this, SLOT( modifiedChanged( int, bool ) ) );
    connect( mWorkspace, SIGNAL( docTitleChanged( int, const QString& ) ), this, SLOT( docTitleChanged( int, const QString& ) ) );
    connect( mWorkspace, SIGNAL( documentInserted( int, const QString&, const QIcon& ) ), this, SLOT( documentInserted( int, const QString&, const QIcon& ) ) );
	connect( mWorkspace, SIGNAL( documentAboutToClose( int ) ), this, SLOT( documentAboutToClose( int ) ) );
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
