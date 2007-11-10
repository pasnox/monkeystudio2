/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : pTabBar
** FileName  : pTabBar.cpp
** Date      : 2007-11-09T23:58:01
** License   : GPL
** Comment   : Your comment here
** Home Page : 
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pTabBar.h"

#include <QMouseEvent>
#include <QApplication>
#include <QIcon>

pTabBar::pTabBar( QWidget* w )
	: QTabBar( w )
{
	// for accepting drop
	setAcceptDrops( true );
	
	// update tab text color on current changed
	connect( this, SIGNAL( currentChanged( int ) ), this, SLOT( clearTabsColor() ) );
}

void pTabBar::mousePressEvent( QMouseEvent* e )
{
	// reset drag position
	dragStartPosition = QPoint();
	
	// get tab under cursor
	int i = tabAt( e->pos() );
	
	// if tab
	if ( i != -1 )
	{
		// emit left button pressed
		if ( e->buttons() == Qt::LeftButton )
			emit leftButtonPressed( i, e->globalPos() );
		
		// emit mid button pressed
		if ( e->buttons() == Qt::MidButton )
			emit midButtonPressed( i, e->globalPos() );
		
		// emit right button pressed and drag position
		if ( e->button() == Qt::RightButton )
		{
			emit rightButtonPressed( i, e->globalPos() );
			dragStartPosition = e->pos();
		}
	}
	
	// default event
	QTabBar::mousePressEvent( e );
}

void pTabBar::mouseMoveEvent(QMouseEvent * e )
{
	// need left button
	if ( e->buttons() != Qt::LeftButton )
		return;
	
	// need target tab and minimum drag distance
	if ( tabAt( e->pos() ) == -1 || ( e->pos() -dragStartPosition ).manhattanLength() < QApplication::startDragDistance() )
		return;

	// create mime
	QMimeData* m = new QMimeData;
	m->setData( "x-tabindex", QByteArray::number( tabAt( e->pos() ) ) );
	m->setData( "x-tabbar", QByteArray::number( reinterpret_cast<quintptr>( this ) ) );
	
	// create drag and set mime
	QDrag* d = new QDrag( this );
	d->setMimeData( m );
	
	// execute drag
	d->exec( Qt::MoveAction );
	
	// default event
	QTabBar::mouseMoveEvent( e );
}

void pTabBar::dragEnterEvent( QDragEnterEvent* e )
{
	// if corect mime and same tabbar
	if ( e->mimeData()->hasFormat( "x-tabindex" ) && e->mimeData()->hasFormat( "x-tabbar" ) 
		&& reinterpret_cast<pTabBar*>( QVariant( e->mimeData()->data( "x-tabbar" ) ).value<quintptr>() ) == this 
		&& tabAt( e->pos() ) != -1 )
	{
		// accept drag
		e->acceptProposedAction();
	}
	
	// default event
	QTabBar::dragEnterEvent( e );
}

void pTabBar::dropEvent( QDropEvent* e )
{
	// get drop tab
	int ni = tabAt( e->pos() );
	
	// if get it
	if ( ni != -1 )
	{
		// get original tab infos
		int oi = e->mimeData()->data( "x-tabindex" ).toInt();
		QVariant otd = tabData( oi );
		QIcon oti = tabIcon( oi );
		QString ott = tabText( oi );
		QColor ottc = tabTextColor( oi );
		QString ottt = tabToolTip( oi );
		QString otwt = tabWhatsThis( oi );
		
		// remove original tab
		removeTab( oi );
		
		// insert new one with correct infos
		int i = insertTab( ni, oti, ott );
		setTabData( i, otd );
		setTabTextColor( i, ottc );
		setTabToolTip( i, ottt );
		setTabWhatsThis( i, otwt );
		
		//accept
		e->acceptProposedAction();
		
		// emit signal
		emit tabDropped( oi, i );
		
		// set new current index
		setCurrentIndex( i );
	}
	
	// default event
	QTabBar::dropEvent( e );
}

void pTabBar::clearTabsColor()
{
	for ( int i = 0; i < count(); i++ )
		setTabTextColor( i, i == currentIndex() ? Qt::blue : Qt::black );
}
