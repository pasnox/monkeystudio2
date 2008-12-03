#include "pDockWidgetTitleBar.h"

#include <QAction>
#include <QLayout>
#include <QToolBar>

#include <QDebug>

pDockWidgetTitleBar::pDockWidgetTitleBar( pDockWidget* parent )
	: QFrame( parent )
{
	Q_ASSERT( parent );
	
	mDock = parent;
	
	mBox1 = new QBoxLayout( QBoxLayout::LeftToRight, this );
	mBox1->setMargin( 0 );
	mBox1->setSpacing( 0 );
	
	mBox2 = new QBoxLayout( QBoxLayout::LeftToRight );
	mBox2->setMargin( 0 );
	mBox2->setSpacing( 0 );
	
	mBox1->addStretch( 100 );
	mBox1->addLayout( mBox2 );
	
	bOrientation = new pDockWidgetButton( this, pDockWidgetButton::Orientation );
	bOrientation->setFixedSize( buttonSize() );
	
	bFloat = new pDockWidgetButton( this, pDockWidgetButton::Float );
	bFloat->setFixedSize( buttonSize() );
	
	bClose = new pDockWidgetButton( this, pDockWidgetButton::Close );
	bClose->setFixedSize( buttonSize() );
	
	mBox2->addWidget( bOrientation, 0, Qt::AlignCenter );
	mBox2->addWidget( bFloat, 0, Qt::AlignCenter );
	mBox2->addWidget( bClose, 0, Qt::AlignCenter );
	
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
	int i = mBox2->sizeHint().width();
	
	if ( mDock->features() & QDockWidget::DockWidgetVerticalTitleBar )
	{
		i = mBox2->sizeHint().height();
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

QSize pDockWidgetTitleBar::iconSize() const
{
	int size = style()->pixelMetric( QStyle::PM_SmallIconSize ) -4;
	
	return QSize( size, size );
}

QWidget* pDockWidgetTitleBar::addAction( QAction* action, int index )
{
	Q_ASSERT( action );
	
	if ( index == -1 )
	{
		index = mBox2->count();
	}
	
	QToolButton* tb = new pDockWidgetButton( this );
	tb->setDefaultAction( action );
	tb->setFixedSize( buttonSize() );
	tb->setIconSize( iconSize() );
	
	mBox2->insertWidget( index, tb, 0, Qt::AlignCenter );
	
	return tb;
}

void pDockWidgetTitleBar::addSeparator( int index )
{
	if ( index == -1 )
	{
		index = mBox2->count();
	}
	
	QFrame* f = new QFrame( this );
	f->setFixedSize( iconSize() );
	
	if ( mDock->features() & QDockWidget::DockWidgetVerticalTitleBar )
	{
		f->setFrameStyle( QFrame::HLine | QFrame::Sunken );
	}
	else
	{
		f->setFrameStyle( QFrame::VLine | QFrame::Sunken );
	}
	
	mBox2->insertWidget( index, f, 0, Qt::AlignCenter );
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
		mBox1->setContentsMargins( 3, 3, 0, 3 );
		mBox2->setDirection( QBoxLayout::BottomToTop );
		
		foreach ( QFrame* f, findChildren<QFrame*>() )
		{
			f->setFrameStyle( QFrame::HLine | QFrame::Sunken );
		}
		
		bOrientation->setIcon( style()->standardIcon( QStyle::SP_ToolBarHorizontalExtensionButton ) );
		bFloat->setVisible( features & QDockWidget::DockWidgetFloatable );
		bClose->setVisible( features & QDockWidget::DockWidgetClosable );
	}
	else
	{
		// horizontal
		mBox1->setDirection( QBoxLayout::LeftToRight );
		mBox1->setContentsMargins( 3, 3, 3, 0 );
		mBox2->setDirection( QBoxLayout::LeftToRight );
		
		foreach ( QFrame* f, findChildren<QFrame*>() )
		{
			f->setFrameStyle( QFrame::VLine | QFrame::Sunken );
		}
		
		bOrientation->setIcon( style()->standardIcon( QStyle::SP_ToolBarVerticalExtensionButton ) );
		bFloat->setVisible( features & QDockWidget::DockWidgetFloatable );
		bClose->setVisible( features & QDockWidget::DockWidgetClosable );
	}
}
