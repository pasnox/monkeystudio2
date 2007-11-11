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
#include "pAction.h"

#include <QMouseEvent>
#include <QApplication>
#include <QIcon>
#include <QPainter>

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
	
	// draw buttons
	for ( int i = 0; i < count(); i++ )
	{
		// paint on this
		QPainter p( this );
		
		// icon rect
		QRect ir = iconRectForTab( i );
		
		// get icon mode
		QIcon::Mode m = ir.contains( mapFromGlobal( QCursor::pos() ) ) ? QIcon::Active : QIcon::Normal;
		
		// get icon file
		QString s = m == QIcon::Active ? ":/file/icons/file/closeall.png" : ":/file/icons/file/close.png";
		
		// draw pixmap
		p.drawPixmap( ir.topLeft(), QIcon( s ).pixmap( iconSize(), m, isTabEnabled( i ) ? QIcon::On : QIcon::Off ) );
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

QSize pTabBar::tabSizeHint( int i ) const
{
	/*
	QFontMetrics fm = fontMetrics();
	QSize ts;
	bool ai = false;
	ts.rheight() = fm.height();
	
	switch ( shape() )
	{
		case QTabBar::RoundedNorth:
		case QTabBar::RoundedSouth:
		case QTabBar::TriangularNorth:
		case QTabBar::TriangularSouth:
			ts.rwidth() = fm.width( tabText( i ) );
			if ( aToggleTabsHaveCloseButton->isChecked() )
				ts.rwidth() += iconSize().width();
			ai = tabIcon( i ).isNull();
			break;
		case QTabBar::RoundedWest:
		case QTabBar::RoundedEast:
		case QTabBar::TriangularWest:
		case QTabBar::TriangularEast:
			// calcul max width
			for ( int j = 0; j < count(); j++ )
			{
				if ( ts.width() < fm.width( tabText( j ) ) )
					ts.rwidth() = fm.width( tabText( j ) );
				if ( !ai && !tabIcon( j ).isNull() )
					ai = true;
			}
			break;
	}
	
	// add icon width if needed
	if ( ai )
		ts.rwidth() += iconSize().width();
	
	// check icon size
	if ( ai && ts.height() < iconSize().height() )
		ts.rheight() = iconSize().height();
	
	// add 4.4 pixel more
	ts += QSize( 3, 3 );
	
	// return
	return ts;
	
	*/
	// good
	
	// get original sizehint
	QSize s = QTabBar::tabSizeHint( i );
	
	// add close button size if needed
	if ( aToggleTabsHaveCloseButton->isChecked() )
	{
		// compute with icon size
		switch ( shape() )
		{
			case QTabBar::RoundedNorth:
			case QTabBar::RoundedSouth:
			case QTabBar::TriangularNorth:
			case QTabBar::TriangularSouth:
				s.rwidth() += iconSize().width();
				if ( iconSize().height() > s.height() )
					s.rheight() = iconSize().height();
				break;
			case QTabBar::RoundedWest:
			case QTabBar::RoundedEast:
			case QTabBar::TriangularWest:
			case QTabBar::TriangularEast:
				s.rheight() += iconSize().width();
				if ( iconSize().height() > s.width() )
					s.rwidth() = iconSize().height();
				break;
		}
	}
	
	// return size
	return s;
}

QRect pTabBar::iconRectForTab( int i )
{
	// get tab infos
	int x = 0, y = 0;
	QSize sh = tabSizeHint( i );
	QRect tr = tabRect( i );
	
	// calcul positions
	/*
	x = 2;
	y = ( sh.height() -iconSize().height() ) / 2;
	*/
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
