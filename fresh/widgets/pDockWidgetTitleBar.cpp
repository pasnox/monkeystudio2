#include "pDockWidgetTitleBar.h"

#include <QAction>
#include <QLayout>
#include <QToolButton>
#include <QFrame>

#include <QDebug>

pDockWidgetTitleBar::pDockWidgetTitleBar( pDockWidget* parent )
	: QWidget( parent )
{
	Q_ASSERT( parent );
	
	mDock = parent;
	
	bOrientation = new pDockWidgetButton( pDockWidgetButton::Orientation, this );
	bFloat = new pDockWidgetButton( pDockWidgetButton::Float, this );
	bClose = new pDockWidgetButton( pDockWidgetButton::Close, this );
	
	mBox1 = new QBoxLayout( QBoxLayout::LeftToRight, this );
	mBox1->setMargin( 3 );
	mBox1->setSpacing( 0 );
	
	mBox2 = new QBoxLayout( QBoxLayout::LeftToRight );
	mBox2->setMargin( 0 );
	mBox2->setSpacing( 0 );
	
	mBox1->addStretch( 100 );
	mBox1->addLayout( mBox2 );
	
	addToolButton( bOrientation );
	addToolButton( bFloat );
	addToolButton( bClose );
	
	featuresChanged( mDock->features() );
	
	connect( mDock, SIGNAL( featuresChanged( QDockWidget::DockWidgetFeatures ) ), this, SLOT( featuresChanged( QDockWidget::DockWidgetFeatures ) ) );
	connect( bOrientation, SIGNAL( clicked() ), this, SLOT( aOrientation_clicked() ) );
	connect( bFloat, SIGNAL( clicked() ), this, SLOT( aFloat_clicked() ) );
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
	//drawFrame( &p );
	
	// init style options
	QStyleOptionDockWidgetV2 titleOpt;
	
	titleOpt.initFrom( mDock );
	titleOpt.rect = rect();
	
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
	QSize size = mDock->widget() ? mDock->widget()->size() : QSize();
	QDockWidget::DockWidgetFeatures features = mDock->features();
	
	if ( features & QDockWidget::DockWidgetVerticalTitleBar )
	{
		size.setWidth( buttonSize().width() +3 );
	}
	else
	{
		size.setHeight( buttonSize().height() +3 );
	}
	
	return size;
}

QSize pDockWidgetTitleBar::buttonSize() const
{
	return QSize( 18, 18 );
}

QSize pDockWidgetTitleBar::iconSize() const
{
	return QSize( 12, 12 );
}

QWidget* pDockWidgetTitleBar::addToolButton( QToolButton* tb, int index )
{
	Q_ASSERT( tb );
	
	if ( index == -1 )
	{
		index = mBox2->count();
	}
	
	tb->setFixedSize( buttonSize() );
	tb->setIconSize( iconSize() );
	tb->setAutoRaise( true );
	
	mBox2->insertWidget( index, tb, 0, Qt::AlignCenter );
	
	return tb;
}

QWidget* pDockWidgetTitleBar::addAction( QAction* action, int index )
{
	Q_ASSERT( action );
	
	if ( index == -1 )
	{
		index = mBox2->count();
	}
	
	if ( action->isSeparator() )
	{
		QFrame::Shape shape = mBox2->direction() == QBoxLayout::LeftToRight ? QFrame::VLine : QFrame::HLine;
		QFrame* f = new QFrame( this );
		
		f->setProperty( "isSeparator", true );
		f->setFrameStyle( shape | QFrame::Sunken );
		f->setFixedSize( 3, buttonSize().height() );
		
		mBox2->insertWidget( index, f, 0, Qt::AlignCenter );
		
		delete action;
		return f;
	}
	else
	{
		QToolButton* tb = new QToolButton( this );
		
		tb->setDefaultAction( action );
		
		return addToolButton( tb, index );
	}
}

QWidget* pDockWidgetTitleBar::addSeparator( int index )
{
	QAction* action = new QAction( this );
	action->setSeparator( true );
	
	return addAction( action, index );
}

void pDockWidgetTitleBar::aOrientation_clicked()
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

void pDockWidgetTitleBar::aFloat_clicked()
{
	mDock->setFloating( !mDock->isFloating() );
}

void pDockWidgetTitleBar::featuresChanged( QDockWidget::DockWidgetFeatures features )
{
	if ( features & QDockWidget::DockWidgetVerticalTitleBar )
	{
		// vertical
		mBox1->setDirection( QBoxLayout::BottomToTop );
		mBox2->setDirection( QBoxLayout::BottomToTop );
		
		bOrientation->setIcon( style()->standardIcon( QStyle::SP_ToolBarHorizontalExtensionButton ) );
		bFloat->setVisible( features & QDockWidget::DockWidgetFloatable );
		bClose->setVisible( features & QDockWidget::DockWidgetClosable );
		
		QFrame::Shape shape = mBox2->direction() == QBoxLayout::BottomToTop ? QFrame::HLine : QFrame::VLine;
		
		for ( int i = 0; i < mBox2->count(); i++ )
		{
			QFrame* f = qobject_cast<QFrame*>( mBox2->itemAt( i )->widget() );
			
			if ( f && f->property( "isSeparator" ).toBool() )
			{
				f->setFrameShape( shape );
				f->setFixedSize( buttonSize().width(), 3 );
			}
		}
	}
	else
	{
		// horizontal
		mBox1->setDirection( QBoxLayout::LeftToRight );
		mBox2->setDirection( QBoxLayout::LeftToRight );
		
		bOrientation->setIcon( style()->standardIcon( QStyle::SP_ToolBarVerticalExtensionButton ) );
		bFloat->setVisible( features & QDockWidget::DockWidgetFloatable );
		bClose->setVisible( features & QDockWidget::DockWidgetClosable );
		
		QFrame::Shape shape = mBox2->direction() == QBoxLayout::LeftToRight ? QFrame::VLine : QFrame::HLine;
		
		for ( int i = 0; i < mBox2->count(); i++ )
		{
			QFrame* f = qobject_cast<QFrame*>( mBox2->itemAt( i )->widget() );
			
			if ( f && f->property( "isSeparator" ).toBool() )
			{
				f->setFrameShape( shape );
				f->setFixedSize( 3, buttonSize().height() );
			}
		}
	}
}
