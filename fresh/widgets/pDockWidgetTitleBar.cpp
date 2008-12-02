#include "pDockWidgetTitleBar.h"

#include <QAction>
#include <QLayout>
#include <QToolButton>
#include <QFrame>
#include <QToolBar>

#include <QDebug>

pDockWidgetTitleBar::pDockWidgetTitleBar( pDockWidget* parent )
	: QFrame( parent )
{
	Q_ASSERT( parent );
	
	mDock = parent;
	
	int size = style()->pixelMetric( QStyle::PM_SmallIconSize ) -4;
	
	mBox1 = new QBoxLayout( QBoxLayout::LeftToRight, this );
	mBox1->setMargin( 0 );
	mBox1->setSpacing( 0 );
	
	mToolBar = new QToolBar( this );
	mToolBar->setIconSize( QSize( size, size ) );
	mToolBar->layout()->setMargin( 0 );
	mToolBar->layout()->setSpacing( 0 );
	
	mBox1->addStretch( 100 );
	mBox1->addWidget( mToolBar, 0, Qt::AlignCenter );
	
	bOrientation = new pDockWidgetButton( pDockWidgetButton::Orientation, this );
	bOrientation->setFixedSize( buttonSize() );
	
	bFloat = new pDockWidgetButton( pDockWidgetButton::Float, this );
	bFloat->setFixedSize( buttonSize() );
	
	bClose = new pDockWidgetButton( pDockWidgetButton::Close, this );
	bClose->setFixedSize( buttonSize() );
	
	mToolBar->addWidget( bOrientation );
	mToolBar->addWidget( bFloat );
	mToolBar->addWidget( bClose );
	
	featuresChanged( mDock->features() );
	
	connect( mDock, SIGNAL( featuresChanged( QDockWidget::DockWidgetFeatures ) ), this, SLOT( featuresChanged( QDockWidget::DockWidgetFeatures ) ) );
	connect( bOrientation, SIGNAL( clicked() ), this, SLOT( bOrientation_clicked() ) );
	connect( bFloat, SIGNAL( clicked() ), this, SLOT( bFloat_clicked() ) );
	connect( bClose, SIGNAL( clicked() ), mDock, SLOT( close() ) );
}

pDockWidgetTitleBar::~pDockWidgetTitleBar()
{
}

void pDockWidgetTitleBar::paintEvent( QPaintEvent* event )
{
	Q_UNUSED( event );
	
	QStylePainter p( this );
	
	// paint frame
	if ( mDock->isFloating() )
	{
		drawFrame( &p );
	}
	
	// init style options
	QStyleOptionDockWidgetV2 titleOpt;
	
	titleOpt.initFrom( mDock );
	titleOpt.rect = rect();
	
	if ( titleOpt.title.isEmpty() )
	{
		titleOpt.title = mDock->windowTitle();
	}
	
	// paint dock title
	if ( mDock->features() & QDockWidget::DockWidgetVerticalTitleBar )
	{
		p.rotate( -90 );
		p.translate( QPoint( -rect().height(), 0 ) );
		
		QSize size = titleOpt.rect.size();
		
		size.transpose();
		titleOpt.rect.setSize( size );
	}

	p.drawControl( QStyle::CE_DockWidgetTitle, titleOpt );
}

QSize pDockWidgetTitleBar::sizeHint() const
{
	ensurePolished();
	
	QSize size = QFrame::sizeHint();
	int i = mToolBar->sizeHint().width();
	
	if ( mDock->features() & QDockWidget::DockWidgetVerticalTitleBar )
	{
		i = mToolBar->sizeHint().height();
	}
	
	if ( mDock )
	{
		QFontMetrics fm( font() );
		i += fm.width( mDock->windowTitle() );
	}
	
	i += 2 *style()->pixelMetric( QStyle::PM_DockWidgetTitleMargin );
	
	if ( mDock->features() & QDockWidget::DockWidgetVerticalTitleBar )
	{
		size.setHeight( i );
	}
	else
	{
		size.setWidth( i );
	}
	
	return size;
}

QSize pDockWidgetTitleBar::buttonSize() const
{
	int size = 2 *style()->pixelMetric( QStyle::PM_DockWidgetTitleBarButtonMargin );
	size += style()->pixelMetric( QStyle::PM_SmallIconSize ) -2;
	
	return QSize( size, size );
}

QWidget* pDockWidgetTitleBar::addAction( QAction* action, int index )
{
	Q_ASSERT( action );
	
	if ( index == -1 )
	{
		index = mToolBar->actions().count();
	}
	
	QAction* before = mToolBar->actions().value( index );
	mToolBar->insertAction( before, action );
	QWidget* widget = mToolBar->widgetForAction( action );
	
	QToolButton* tb = qobject_cast<QToolButton*>( widget );
	if ( tb )
	{
		tb->setFixedSize( buttonSize() );
	}
	
	return widget;
}

void pDockWidgetTitleBar::addSeparator( int index )
{
	if ( index == -1 )
	{
		index = mToolBar->actions().count();
	}
	
	QAction* before = mToolBar->actions().value( index );
	mToolBar->insertSeparator( before );
}

void pDockWidgetTitleBar::bOrientation_clicked()
{
	QDockWidget::DockWidgetFeatures features = mDock->features();
	
	if ( features & QDockWidget::DockWidgetVerticalTitleBar )
	{
		mDock->setFeatures( features ^ QDockWidget::DockWidgetVerticalTitleBar );
	}
	else
	{
		mDock->setFeatures( features | QDockWidget::DockWidgetVerticalTitleBar );
	}
}

void pDockWidgetTitleBar::bFloat_clicked()
{
	mDock->setFloating( !mDock->isFloating() );
}

void pDockWidgetTitleBar::featuresChanged( QDockWidget::DockWidgetFeatures features )
{
	if ( features & QDockWidget::DockWidgetVerticalTitleBar )
	{
		// vertical
		mBox1->setDirection( QBoxLayout::BottomToTop );
		mBox1->setContentsMargins( 4, 4, 0, 4 );
		mToolBar->setOrientation( Qt::Vertical );
		
		bOrientation->setIcon( style()->standardIcon( QStyle::SP_ToolBarHorizontalExtensionButton ) );
		bFloat->setVisible( features & QDockWidget::DockWidgetFloatable );
		bClose->setVisible( features & QDockWidget::DockWidgetClosable );
	}
	else
	{
		// horizontal
		mBox1->setDirection( QBoxLayout::LeftToRight );
		mBox1->setContentsMargins( 4, 4, 4, 0 );
		mToolBar->setOrientation( Qt::Horizontal );
		
		bOrientation->setIcon( style()->standardIcon( QStyle::SP_ToolBarVerticalExtensionButton ) );
		bFloat->setVisible( features & QDockWidget::DockWidgetFloatable );
		bClose->setVisible( features & QDockWidget::DockWidgetClosable );
	}
}
