/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pTabbedWorkspace.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class provide a workspace that can at run time be switched in SDI / MDI or Top Level
 ********************************************************************************************************/
#include "pTabbedWorkspace.h"
#include "pTabbedWorkspaceRightCorner.h"

#include <QBoxLayout>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QWorkspace>
#include <QIcon>
#include <QCloseEvent>
#include <QFile>

pTabbedWorkspace::pTabbedWorkspace( QWidget* p, pTabbedWorkspace::TabMode m )
	: QWidget( p ), mTabsHaveShortcut( true )
{
	/*
	QBoxLayout::LeftToRight
	QBoxLayout::RightToLeft
	QBoxLayout::TopToBottom
	QBoxLayout::BottomToTop
	*/
	// tab widget
	mTabLayout = new QBoxLayout( QBoxLayout::LeftToRight );
	mTabLayout->setSpacing( 3 );
	mTabLayout->setMargin( 0 );
	mTabLayout->addWidget( ( mTabBar = new QTabBar ) );
	mTabLayout->addStretch( 100 );

	// document widget
	mStackedLayout = new QStackedLayout;
	mStackedLayout->addWidget( ( mStackedWidget = new QStackedWidget ) );
	mStackedLayout->addWidget( ( mWorkspaceWidget = new QWorkspace ) );
	mWorkspaceWidget->setScrollBarsEnabled( true );

	// main layout
	mLayout = new QBoxLayout( QBoxLayout::TopToBottom, this );
	mLayout->setSpacing( 0 );
	mLayout->setMargin( 0 );
	mLayout->addLayout( mTabLayout );
	mLayout->addLayout( mStackedLayout );

	// connections
	connect( mTabBar, SIGNAL( currentChanged( int ) ), this, SLOT( internal_currentChanged( int ) ) );
	connect( mWorkspaceWidget, SIGNAL( windowActivated( QWidget* ) ), this, SLOT( workspaceWidget_windowActivated( QWidget* ) ) );
	// init view
	setAttribute( Qt::WA_DeleteOnClose );
	mTabBar->setDrawBase( false );
	mTabBar->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum ) );
	setCornerWidget( pTabbedWorkspaceRightCorner::instance( this ) );
	setTabMode( m );
	setDocumentMode( pTabbedWorkspace::dmMaximized );
}

pTabbedWorkspace::~pTabbedWorkspace()
{
	// close all document
	closeAllTabs( true );

	// delete all unclose document, if user click cancel
	qDeleteAll( mDocuments );

	// delete containers
	delete mStackedWidget;
	delete mWorkspaceWidget;
}

bool pTabbedWorkspace::eventFilter( QObject* o, QEvent* e )
{
	// get document
	QWidget* td = qobject_cast<QWidget*>( o );

	// get event type
	QEvent::Type t = e->type();

	// if mode is toplevel and event is activate, activate correct window if needed
	if ( mTabMode == tmTopLevel && t == QEvent::WindowActivate )
	{
		if ( td && td != currentDocument() )
			setCurrentDocument( td );
	}

	// remove document from workspace
	else if ( t == QEvent::Close && td )
	{
		// get closeevent
		QCloseEvent* ce = static_cast<QCloseEvent*>( e );

		// emit that document will be close, giving event so user can cancel it
		emit aboutToCloseTab( indexOf( td ), ce );
		emit aboutToCloseDocument( td, ce );
		
		// close document if accepted
		if ( ce->isAccepted() )
			td->deleteLater();
		else // else cancel
			return true;
	}

	// return default event filter
	return QWidget::eventFilter( o, e );
}

void pTabbedWorkspace::workspaceWidget_windowActivated( QWidget* w )
{
	setCurrentDocument( w );
}

void pTabbedWorkspace::removeDocument( QObject* o )
{
	removeDocument( qobject_cast<QWidget*>( o ) );
}

QTabBar* pTabbedWorkspace::tabBar() const
{
	return mTabBar;
}

QTabBar::Shape pTabbedWorkspace::tabShape() const
{
	return mTabBar->shape();
}

pTabbedWorkspace::TabMode pTabbedWorkspace::tabMode() const
{
	return mTabMode;
}

pTabbedWorkspace::DocumentMode pTabbedWorkspace::documentMode() const
{
	return mDocumentMode;
}

void pTabbedWorkspace::setBackground( const QPixmap& p )
{
	if ( !p.isNull() )
		mWorkspaceWidget->setBackground( QBrush( p ) );
}

void pTabbedWorkspace::setBackground( const QString& s )
{
	if ( QFile::exists( s ) )
		mWorkspaceWidget->setBackground( QBrush( QPixmap( s ) ) );
}

void pTabbedWorkspace::setTabShape( QTabBar::Shape s )
{
	if ( tabShape() != s )
	{
		// apply tab shape
		mTabBar->setShape( s );
	
		// update view layout
		switch ( s )
		{
		case QTabBar::RoundedNorth:
		case QTabBar::TriangularNorth:
			mTabLayout->setDirection( QBoxLayout::LeftToRight );
			mLayout->setDirection( QBoxLayout::TopToBottom );
			break;
		case QTabBar::RoundedSouth:
		case QTabBar::TriangularSouth:
			mTabLayout->setDirection( QBoxLayout::LeftToRight );
			mLayout->setDirection( QBoxLayout::BottomToTop );
			break;
		case QTabBar::RoundedWest:
		case QTabBar::TriangularWest:
			mTabLayout->setDirection( QBoxLayout::BottomToTop );
			mLayout->setDirection( QBoxLayout::LeftToRight );
			break;
		case QTabBar::RoundedEast:
		case QTabBar::TriangularEast:
			mTabLayout->setDirection( QBoxLayout::TopToBottom );
			mLayout->setDirection( QBoxLayout::RightToLeft );
			break;
		}
	
		// update corners
		updateCorners();
	
		// emit shape changed
		emit tabShapeChanged( s );
	}
}

void pTabbedWorkspace::setTabMode( pTabbedWorkspace::TabMode t )
{
	if ( mTabMode != t )
	{
		// retain tab mode
		mTabMode = t;

		// retain current index
		int i = currentIndex();

		// show correct workspace
		switch ( mTabMode )
		{
		case tmSDI:
			if ( mStackedLayout->currentWidget() != mStackedWidget )
				mStackedLayout->setCurrentWidget( mStackedWidget );
			break;
		case tmMDI:
			if ( mStackedLayout->currentWidget() != mWorkspaceWidget )
				mStackedLayout->setCurrentWidget( mWorkspaceWidget );
			break;
		case tmTopLevel:
			if ( mStackedLayout->currentIndex() != -1 )
				mStackedLayout->setCurrentIndex( -1 );
			break;
		}

		// update view
		updateView();

		// restore current index
		setCurrentIndex( i );

		// emit tab mode changed
		emit tabModeChanged( mTabMode );
	}
}

void pTabbedWorkspace::setDocumentMode( pTabbedWorkspace::DocumentMode d )
{
	if ( mTabMode == pTabbedWorkspace::tmMDI /*&& mDocumentMode != d*/ )
	{
		// retain document mode
		mDocumentMode = d;

		// apply document layout
		switch( mDocumentMode )
		{
		case dmMaximized:
			foreach ( QWidget* td, documents() )
				if ( !td->isMaximized() )
					td->showMaximized();
			break;
		case dmCascade:
			mWorkspaceWidget->cascade();
			break;
		case dmTile:
			mWorkspaceWidget->tile();
			break;
		case dmIcons:
			mWorkspaceWidget->arrangeIcons();
			break;
		case dmMinimizeAll:
			foreach ( QWidget* td, documents() )
				if ( !td->isMinimized() )
					td->showMinimized();
			break;
		case dmRestoreAll:
			foreach ( QWidget* td, documents() )
				if ( td->isMaximized() || td->isMinimized() )
					td->showNormal();
			break;
		}

		// emit document mode changed
		emit documentModeChanged( mDocumentMode );
	}
}

int pTabbedWorkspace::currentIndex() const
{
	return mTabBar->currentIndex();
}

void pTabbedWorkspace::setCurrentIndex( int i )
{
	if ( i != currentIndex() )
		mTabBar->setCurrentIndex( i );
	else if ( currentDocument() != document( i ) )
		internal_currentChanged( i );
}

void pTabbedWorkspace::internal_currentChanged( int i )
{
	// get document
	QWidget* d = document( i );
	// set correct document visible
	if ( currentDocument() != d )
	{
		switch ( mTabMode )
		{
			case tmSDI:
				if ( mStackedWidget->currentIndex() != i )
					mStackedWidget->setCurrentIndex( i );
				break;
			case tmMDI:
				if ( d && mWorkspaceWidget->activeWindow() != d )
					mWorkspaceWidget->setActiveWindow( d );
			break;
			case tmTopLevel:
				if ( d )
				{
					d->raise();
					d->activateWindow();
				}
			break;
		}
	}
	// emit document change
	emit currentChanged( i );
}

QWidget* pTabbedWorkspace::currentDocument() const
{
	switch ( mTabMode )
	{
		case tmSDI:
			return mStackedWidget->currentWidget();
		case tmMDI:
			return  mWorkspaceWidget->activeWindow();
		case tmTopLevel:
			return QApplication::activeWindow();
		default:
			Q_ASSERT_X( 0, __func__, "not right tab mode" );
	}
	return 0;
}

void pTabbedWorkspace::setCurrentDocument( QWidget* d )
{
	setCurrentIndex( indexOf( d ) );
}

int pTabbedWorkspace::indexOf( QWidget* d ) const
{
	return mDocuments.indexOf( d );
}

QWidget* pTabbedWorkspace::document( int i ) const
{
	return mDocuments.value( i );
}

int pTabbedWorkspace::count() const
{
	return mDocuments.count();
}

QList<QWidget*> pTabbedWorkspace::documents() const
{
	return mDocuments;
}
//
pTabbedWorkspaceCorner* pTabbedWorkspace::cornerWidget( Qt::Corner c ) const
{
	// if only one it s tabbar, no need to check
	if ( mTabLayout->count() == 1 )
		return 0;

	// get corner
	switch ( c )
	{
	case Qt::TopLeftCorner:
	case Qt::BottomLeftCorner:
		if ( mTabLayout->indexOf( mTabBar ) == 0 )
			return 0;
		return qobject_cast<pTabbedWorkspaceCorner*>( mTabLayout->itemAt( 0 )->widget() );
		break;
	case Qt::TopRightCorner:
	case Qt::BottomRightCorner:
		if ( mTabLayout->indexOf( mTabBar ) == mTabLayout->count() -1 )
			return 0;
		return qobject_cast<pTabbedWorkspaceCorner*>( mTabLayout->itemAt( mTabLayout->count() -1 )->widget() );
		break;
	}

	// shut up gcc warning
	return 0;
}

void pTabbedWorkspace::setCornerWidget( pTabbedWorkspaceCorner* w, Qt::Corner c )
{
	switch ( c )
	{
	case Qt::TopLeftCorner:
	case Qt::BottomLeftCorner:
		if ( mTabLayout->indexOf( mTabBar ) == 1 )
			delete mTabLayout->itemAt( 0 )->widget();
		mTabLayout->insertWidget( 0, w );
		break;
	case Qt::TopRightCorner:
	case Qt::BottomRightCorner:
		if ( mTabLayout->indexOf( mTabBar ) != mTabLayout->count() -1 )
			delete mTabLayout->itemAt( mTabLayout->count() -1 )->widget();
		mTabLayout->addWidget( w );
		break;
	}
}

void pTabbedWorkspace::updateCorners()
{
	// temp corner
	pTabbedWorkspaceCorner* c;

	// check left corner
	if ( ( c = cornerWidget( Qt::TopLeftCorner ) ) )
	{
		c->setDirection( mTabLayout->direction() );
		c->setEnabled( count() );
	}

	// check right corner
	if ( ( c = cornerWidget( Qt::TopRightCorner ) ) )
	{
		c->setDirection( mTabLayout->direction() );
		c->setEnabled( count() );
	}
}

void pTabbedWorkspace::updateView( QWidget* nd )
{
	// tmp list
	QList<QWidget*> l;
	if ( nd )
		l << nd;
	else
		l << mDocuments;

	// add document to correct workspace
	foreach ( QWidget* td, l )
	{
		// add to correct container
		switch ( mTabMode )
		{
		case tmSDI:
			mStackedWidget->addWidget( td );
			break;
		case tmMDI:
			mWorkspaceWidget->addWindow( td );
			mWorkspaceWidget->setActiveWindow( td );
			switch ( mDocumentMode )
			{
			case dmMaximized:
				if ( !td->isMaximized() )
					td->showMaximized();
				break;
			case dmCascade:
				if ( !td->isVisible() )
					td->show();
				if ( !l.isEmpty() && td == l.last() )
					mWorkspaceWidget->cascade();
				break;
			case dmTile:
				if ( !td->isVisible() )
					td->show();
				if ( !l.isEmpty() && td == l.last() )
					mWorkspaceWidget->tile();
				break;
			case dmIcons:
				if ( !td->isMinimized() )
					td->showMinimized();
				if ( !l.isEmpty() && td == l.last() )
					mWorkspaceWidget->arrangeIcons();
				break;
			case dmMinimizeAll:
				if ( !td->isMinimized() )
					td->showMinimized();
				break;
			case dmRestoreAll:
				if ( td->isMaximized() || td->isMinimized() )
					nd->showNormal();
				break;
			}
			break;
		case tmTopLevel:
			// remove from container
			if ( td->parent() )
				td->setParent( 0 );
			if ( !td->isVisible() )
				td->show();
			break;
		}	
	}

	// update corners
	updateCorners();
}

void pTabbedWorkspace::addDocument( QWidget* td, int i )
{
	if ( i == -1 )
		i = count();

	// set auto delete false
	td->setAttribute( Qt::WA_DeleteOnClose, false );

	// auto remove itself on delete
	connect( td, SIGNAL( destroyed( QObject* ) ), this, SLOT( removeDocument( QObject* ) ) );

	// filter the document
	td->installEventFilter( this );

	// append to document list
	mDocuments.insert( i, td );

	// update view
	updateView( td );

	// emit tab inserted
	emit tabInserted( i );

	// emit tab current changed
	emit currentChanged( i );
}

int pTabbedWorkspace::addTab( QWidget* td, const QString& l )
{
	
	return insertTab( count(), td, l );
}

int pTabbedWorkspace::addTab( QWidget* td, const QIcon& i, const QString& l )
{
	return insertTab( count(), td, i, l );
}

int pTabbedWorkspace::insertTab( int i, QWidget* td, const QString& l )
{
	return insertTab( i, td, QIcon(), l );
}

int pTabbedWorkspace::insertTab( int j, QWidget* td, const QIcon& i, const QString& l )
{
	// if already in or not existing d, cancel
	if ( !td || mDocuments.contains( td ) )
		return -1;

	// insert document
	j = mTabBar->insertTab( j, l );
	addDocument( td, j );

	// set icon if available
	if ( !i.isNull() )
		mTabBar->setTabIcon( j, i );

	// set chortcut if needed
	if ( mTabsHaveShortcut )
		updateTabsNumber( j );

	// return true index of the new document
	return j;
}

void pTabbedWorkspace::removeTab( int i )
{
	removeDocument( document( i ) );
}

void pTabbedWorkspace::removeDocument( QWidget* td )
{
	if ( !td )
		return;

	// get document index
	int i = indexOf( td );

	// remove document from list
	mDocuments.removeAll( td );

	// remove from stacked to avoid crash
	if ( mTabMode == tmSDI )
		mStackedWidget->removeWidget( td );

	// remove tab and position to new index
	if ( i != -1 )
	{
		mTabBar->removeTab( i );
		updateCorners();

		// set chortcut if needed
		if ( mTabsHaveShortcut )
			updateTabsNumber( i );

		emit tabRemoved( i );
		setCurrentIndex( currentIndex() );

		// emit current changed
		emit currentChanged( currentIndex() );
	}
}

void pTabbedWorkspace::closeCurrentTab()
{
	QWidget* td = currentDocument();
	if ( td )
		td->close();
}

void pTabbedWorkspace::closeAllTabs( bool b )
{
	foreach ( QWidget* td, mDocuments )
	{
		td->close();
		if ( b )
			td->deleteLater();
	}
}

void pTabbedWorkspace::activateNextDocument()
{
	int currIndex = currentIndex();
	if ( currIndex +1 == count() )
		setCurrentIndex( 0 );
	else
		setCurrentIndex( currIndex +1 );
}

void pTabbedWorkspace::activatePreviousDocument()
{
	int currIndex = currentIndex();
	if ( currIndex -1 == -1 )
		setCurrentIndex( count() -1 );
	else
		setCurrentIndex( currIndex -1 );
}

bool pTabbedWorkspace::tabsHaveShortcut() const
{
	return mTabsHaveShortcut;
}

void pTabbedWorkspace::setTabsHaveShortcut( bool b )
{
	// if same state, cancel
	if ( mTabsHaveShortcut == b )
		return;

	// store tabs have shortcut
	mTabsHaveShortcut = b;

	// update tabs text
	updateTabsNumber();
}

void pTabbedWorkspace::updateTabsNumber( int i )
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
		QString s = mTabBar->tabText( j );

		// look index of cut part
		int k = s.indexOf( ":" );

		// set new tab caption
		if ( mTabsHaveShortcut )
			mTabBar->setTabText( j, QString( "&%1: %2" ).arg( j ).arg( s.mid( k != -1 ? k +2 : 0 ) ) );
		else
			mTabBar->setTabText( j, s.mid( k != -1 ? k +2 : 0 ) );
	}
}
