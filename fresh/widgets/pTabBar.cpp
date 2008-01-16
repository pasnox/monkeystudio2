/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pTabBar.cpp
** Date      : 2008-01-14T00:27:50
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
#include "pTabBar.h"
#include "pAction.h"

#include <QMouseEvent>
#include <QApplication>
#include <QIcon>
#include <QPainter>

#include <QDebug>

pTabBar::pTabBar( QWidget* w )
	: QTabBar( w )
{
	// set default colors
	mTabsColor = Qt::black;
	mCurrentTabColor = Qt::blue;
	
	// close button
	aToggleTabsHaveCloseButton = new pAction( "aTabbedTabsHaveShortcut", tr( "Tabs Have &Close Button" ), QKeySequence(), tr( "Tabbed Workspace" ) );
	aToggleTabsHaveCloseButton->setCheckable( true );
	
	// create actions for right tab
	aToggleTabsHaveShortcut = new pAction( "aTabbedTabsHaveShortcut", tr( "Tabs Have &Shortcut" ), QKeySequence(), tr( "Tabbed Workspace" ) );
	aToggleTabsHaveShortcut->setCheckable( true );
	
	// elid
	aToggleTabsElided = new pAction( "aTabbedTabsElided", tr( "Tabs Are &Elided" ), QKeySequence(), tr( "Tabbed Workspace" ) );
	aToggleTabsElided->setCheckable( true );
	
	// for accepting drop
	setAcceptDrops( true );
	
	// set mouse tracking
	setMouseTracking( true );
	
	// update tab text color on current changed
	connect( this, SIGNAL( currentChanged( int ) ), this, SLOT( resetTabsColor() ) );
	connect( this, SIGNAL( tabsColorChanged( const QColor& ) ), this, SLOT( resetTabsColor() ) );
	connect( this, SIGNAL( currentTabColorChanged( const QColor& ) ), this, SLOT( resetTabsColor() ) );
	connect( this, SIGNAL( tabsHaveCloseButtonChanged( bool ) ), this, SLOT( update() ) );
	connect( aToggleTabsHaveCloseButton, SIGNAL( toggled( bool ) ), this, SLOT( setTabsHaveCloseButton( bool ) ) );
	connect( aToggleTabsHaveShortcut, SIGNAL( toggled( bool ) ), this, SLOT( setTabsHaveShortcut( bool ) ) );
	connect( aToggleTabsElided, SIGNAL( toggled( bool ) ), this, SLOT( setTabsElided( bool ) ) );
}

void pTabBar::paintEvent( QPaintEvent* e )
{
	// draw tabs
	QTabBar::paintEvent( e );
	
	// update button close
	if ( !aToggleTabsHaveCloseButton->isChecked() )
		return;
	
	// get tab
	int i = tabAt( mapFromGlobal( QCursor::pos() ) );
	if ( i != -1 )
	{
		// get close button rect
		QRect ir = iconRectForTab( i );
		
		// if mouse in close button rect
		if ( ir.contains( mapFromGlobal( QCursor::pos() ) ) )
		{
			// draw button
			QPainter p( this );
			p.drawPixmap( ir.topLeft(), QIcon( ":/file/icons/file/closeall.png" ).pixmap( iconSize(), QIcon::Active, isTabEnabled( i ) ? QIcon::On : QIcon::Off ) );
		}
	}
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
		if ( e->button() == Qt::LeftButton )
			emit leftButtonPressed( i, e->globalPos() );
		
		// emit mid button pressed
		if ( e->button() == Qt::MidButton )
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

void pTabBar::mouseReleaseEvent( QMouseEvent* e )
{
	// check button close clicked
	if ( aToggleTabsHaveCloseButton->isChecked() )
	{
		// get tab under cursor
		int i = tabAt( e->pos() );
		
		// if tab and left button and  tab icon pressed
		if ( i != -1 )
			if ( e->button() == Qt::LeftButton && inCloseButtonRect( i, e->pos() ) )
				emit closeButtonClicked( i );
	}
	
	// default event
	QTabBar::mouseReleaseEvent( e );
}

void pTabBar::mouseMoveEvent(QMouseEvent * e )
{
	if ( aToggleTabsHaveCloseButton->isChecked() )
	{
		// update icon state
		update();
	
		// change cursor if over button
		if ( inCloseButtonRect( tabAt( e->pos() ), e->pos() ) )
			setCursor( Qt::PointingHandCursor );
		else
			unsetCursor();
	}
	
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
	// if correct mime and same tabbar
	if ( ( e->mimeData()->hasFormat( "x-tabindex" ) && e->mimeData()->hasFormat( "x-tabbar" ) 
		&& reinterpret_cast<pTabBar*>( QVariant( e->mimeData()->data( "x-tabbar" ) ).value<quintptr>() ) == this 
		&& tabAt( e->pos() ) != -1 ) || e->mimeData()->hasUrls() )
	{
		// accept drag
		e->acceptProposedAction();
	}
	
	// default event
	QTabBar::dragEnterEvent( e );
}

void pTabBar::dropEvent( QDropEvent* e )
{
	if ( !e->mimeData()->hasUrls() )
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
	}
	else
		emit urlsDropped( e->mimeData()->urls () );
	
	// default event
	QTabBar::dropEvent( e );
}

void pTabBar::tabInserted( int i )
{
	// set chortcut if needed
	if ( tabsHaveShortcut() )
		updateTabsNumber( i );
}

void pTabBar::tabRemoved( int i )
{
	// set chortcut if needed
	if ( tabsHaveShortcut() )
		updateTabsNumber( i );
}

QRect pTabBar::iconRectForTab( int i )
{
	// get tab infos
	int x = 0, y = 0;
	QRect tr = tabRect( i );
	QSize sh = tr.size();
	
	switch ( shape() )
	{
		case QTabBar::RoundedNorth:
		case QTabBar::TriangularNorth:
			// calcul positions
			x = sh.width() -iconSize().width();
			y = ( sh.height() -iconSize().height() ) / 2;
			if ( currentIndex() == i )
			{
				y++;
				x -= 2;
			}
			else if ( currentIndex() != i )
			{
				y += 2;
				x--;
			}
			break;
		case QTabBar::RoundedSouth:
		case QTabBar::TriangularSouth:
			// calcul positions
			x = sh.width() -iconSize().width();
			y = ( sh.height() -iconSize().height() ) / 2;
			if ( currentIndex() == i )
				x -= 2;
			else if ( currentIndex() != i )
				x--;
			break;
		case QTabBar::RoundedWest:
		case QTabBar::TriangularWest:
			// calcul positions
			x = ( sh.width() -iconSize().height() ) / 2;
			if ( currentIndex() == i )
			{
				x++;
				y += 2;
			}
			else if ( currentIndex() != i )
			{
				x += 2;
				y += 2;
			}
			break;
		case QTabBar::RoundedEast:
		case QTabBar::TriangularEast:
			// calcul positions
			x = ( sh.width() -iconSize().height() ) / 2;
			y = sh.height() -iconSize().width();
			if ( currentIndex() == i )
			{
				x++;
				y -= 2;
			}
			else if ( currentIndex() != i )
			{
				x--;
				y -= 2;
			}
			break;
	}
	
	// return icon rect
	return QRect( tr.topLeft() +QPoint( x, y ), iconSize() );
}

bool pTabBar::inCloseButtonRect( int i, const QPoint& p )
{ return iconRectForTab( i ).contains( p ); }

void pTabBar::resetTabsColor()
{
	for ( int i = 0; i < count(); i++ )
		setTabTextColor( i, i == currentIndex() ? currentTabColor() : tabsColor() );
}

QColor pTabBar::tabsColor() const
{ return mTabsColor; }

void pTabBar::setTabsColor( const QColor& c )
{
	if ( mTabsColor == c )
		return;
	mTabsColor = c;
	emit tabsColorChanged( mTabsColor );
}

QColor pTabBar::currentTabColor() const
{ return mCurrentTabColor; }

void pTabBar::setCurrentTabColor( const QColor& c )
{
	if ( mCurrentTabColor == c )
		return;
	mCurrentTabColor = c;
	emit currentTabColorChanged( mCurrentTabColor );
}

bool pTabBar::tabsHaveCloseButton() const
{ return aToggleTabsHaveCloseButton->isChecked(); }

void pTabBar::setTabsHaveCloseButton( bool b )
{
	if ( aToggleTabsHaveCloseButton->isChecked() == b && sender() != aToggleTabsHaveCloseButton )
		return;
	aToggleTabsHaveCloseButton->setChecked( b );
	setTabText( 0, tabText( 0 ) ); // workaround for tabs update
	emit tabsHaveCloseButtonChanged( aToggleTabsHaveCloseButton->isChecked() );
}

bool pTabBar::tabsHaveShortcut() const
{ return aToggleTabsHaveShortcut->isChecked(); }

void pTabBar::setTabsHaveShortcut( bool b )
{
	if ( aToggleTabsHaveShortcut->isChecked() == b && sender() != aToggleTabsHaveShortcut )
		return;
	aToggleTabsHaveShortcut->setChecked( b );
	updateTabsNumber();
	emit tabsHaveShortcutChanged( aToggleTabsHaveShortcut->isChecked() );
}

bool pTabBar::tabsElided() const
{ return  aToggleTabsElided->isChecked(); }

void pTabBar::setTabsElided( bool b )
{
	if ( aToggleTabsElided->isChecked() == b && sender() != aToggleTabsElided )
		return;
	aToggleTabsElided->setChecked( b );
	setElideMode( b ? Qt::ElideMiddle : Qt::ElideNone );
	setTabText( 0, tabText( 0 ) ); // workaround for tabs update
	emit tabsElidedChanged( aToggleTabsElided->isChecked() );
}

void pTabBar::updateTabsNumber( int i )
{
	// fill i if i = -1 for complete update
	if ( i == -1 )
		i = 0;

	// loop documents starting at id i
	for ( int j = i; j < count(); j++ )
	{
		// only 10 tabs can have shortcut
		if ( j > 9 )
			return;

		// got tab text
		QString s = tabText( j );

		// look index of cut part
		int k = s.indexOf( ":" );

		// set new tab caption
		if ( tabsHaveShortcut() )
			setTabText( j, QString( "&%1: %2" ).arg( j ).arg( s.mid( k != -1 ? k +2 : 0 ) ) );
		else
			setTabText( j, s.mid( k != -1 ? k +2 : 0 ) );
	}
}

QAction* pTabBar::toggleTabsHaveCloseButtonAction() const
{ return aToggleTabsHaveCloseButton; }

QAction* pTabBar::toggleTabsHaveShortcutAction() const
{ return aToggleTabsHaveShortcut; }

QAction* pTabBar::toggleTabsElidedAction() const
{ return aToggleTabsElided; }
