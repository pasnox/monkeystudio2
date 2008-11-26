#include "pDockFileBrowserTitleBar.h"

#include <pIconManager.h>

#include <QAction>
#include <QLayout>
#include <QToolButton>

#include <QDebug>

pDockFileBrowserTitleBar::pDockFileBrowserTitleBar( QDockWidget* parent )
	: QLabel( parent )
{
	Q_ASSERT( parent );
	
	mDock = parent;
	
	setText( tr( "File Browser" ) );
	aOrientation = new QAction( this );
	aFloat = new QAction( this );
	aClose = new QAction( this );
	
	setIndent( 5 );
	aOrientation->setIcon( pIconManager::icon( "bookmark.png" ) );
	aFloat->setIcon( pIconManager::icon( "close.png" ) );
	aClose->setIcon( pIconManager::icon( "closeall.png" ) );
	
	mBox1 = new QBoxLayout( QBoxLayout::LeftToRight, this );
	mBox1->setMargin( 3 );
	mBox1->setSpacing( 0 );
	
	mBox2 = new QBoxLayout( QBoxLayout::LeftToRight );
	mBox2->setMargin( 0 );
	mBox2->setSpacing( 0 );
	
	mBox1->addStretch( 100 );
	mBox1->addLayout( mBox2 );
	
	addAction( aOrientation );
	addAction( aFloat );
	addAction( aClose );
	
	featuresChanged( mDock->features() );
	
	connect( mDock, SIGNAL( featuresChanged( QDockWidget::DockWidgetFeatures ) ), this, SLOT( featuresChanged( QDockWidget::DockWidgetFeatures ) ) );
	connect( aOrientation, SIGNAL( triggered() ), this, SLOT( aOrientation_triggered() ) );
	connect( aFloat, SIGNAL( triggered() ), this, SLOT( aFloat_triggered() ) );
	connect( aClose, SIGNAL( triggered() ), mDock, SLOT( close() ) );
}

pDockFileBrowserTitleBar::~pDockFileBrowserTitleBar()
{
}

QSize pDockFileBrowserTitleBar::sizeHint() const
{
	QSize size = mDock->widget() ? mDock->widget()->size() : QSize();
	QDockWidget::DockWidgetFeatures features = mDock->features();
	
	if ( features & QDockWidget::DockWidgetVerticalTitleBar )
	{
		size.setWidth( 26 );
	}
	else
	{
		size.setHeight( 26 );
	}
	
	return size;
}

QSize pDockFileBrowserTitleBar::iconSize() const
{
	return QSize( 12, 12 );
}

QWidget* pDockFileBrowserTitleBar::addAction( QAction* action, int index )
{
	Q_ASSERT( action );
	
	if ( index == -1 )
	{
		index = mBox2->count();
	}
	
	if ( action->isSeparator() )
	{
		QFrame* f = new QFrame( this );
		QFrame::Shape shape = mBox2->direction() == QBoxLayout::LeftToRight ? QFrame::VLine : QFrame::HLine;
		f->setFrameStyle( shape | QFrame::Sunken );
		
		mBox2->insertWidget( index, f );
		
		return f;
	}
	else
	{
		QToolButton* tb = new QToolButton( this );
		tb->setIconSize( iconSize() );
		tb->setDefaultAction( action );
		tb->setAutoRaise( true );
		
		mBox2->insertWidget( index, tb );
		
		return tb;
	}
}

QWidget* pDockFileBrowserTitleBar::addSeparator( int index )
{
	QAction* action = new QAction( this );
	action->setSeparator( true );
	
	return addAction( action, index );
}

void pDockFileBrowserTitleBar::aOrientation_triggered()
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

void pDockFileBrowserTitleBar::aFloat_triggered()
{
	mDock->setFloating( !mDock->isFloating() );
}

void pDockFileBrowserTitleBar::featuresChanged( QDockWidget::DockWidgetFeatures features )
{
	if ( features & QDockWidget::DockWidgetVerticalTitleBar )
	{
		// vertical
		mBox1->setDirection( QBoxLayout::BottomToTop );
		mBox2->setDirection( QBoxLayout::BottomToTop );
	/*
		setOrientation( Qt::Vertical );
		setLayoutDirection( Qt::LeftToRight );
		//lTitle->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding );
		*/
	}
	else
	{
		// horizontal
		mBox1->setDirection( QBoxLayout::LeftToRight );
		mBox2->setDirection( QBoxLayout::LeftToRight );
	/*
		setOrientation( Qt::Horizontal );
		//lTitle->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
		setLayoutDirection( Qt::LeftToRight );
		layout()->setAlignment( Qt::AlignRight );
		*/
	}
}
