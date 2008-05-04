#include "pQueuedStatusBar.h"

pQueuedStatusBar::pQueuedStatusBar( QWidget* parent )
	: QStatusBar( parent )
{
	// create pQueuedMessageWidget
	mQueuedWidget = new pQueuedMessageWidget( this );
	mQueuedWidget->setVisible( false );
	addWidget( mQueuedWidget, 100 );
	// connections
	connect( mQueuedWidget, SIGNAL( messageShown() ), this, SLOT( messageChanged() ) );
	connect( mQueuedWidget, SIGNAL( messageClosed() ), this, SLOT( messageChanged() ) );
}

int pQueuedStatusBar::appendMessage( const pQueuedMessage& message )
{
	return mQueuedWidget->append( message );
}

int pQueuedStatusBar::appendMessage( const QString& message, int milliseconds, const QPixmap pixmap, const QBrush& background, const QBrush& foreground )
{
	return mQueuedWidget->append( message, milliseconds, pixmap, background, foreground );
}

void pQueuedStatusBar::removeMessage( const pQueuedMessage& message )
{
	mQueuedWidget->remove( message );
}

void pQueuedStatusBar::removeMessage( int id )
{
	mQueuedWidget->remove( id );
}

void pQueuedStatusBar::messageChanged()
{
	setPalette( mQueuedWidget->palette() );
	setAutoFillBackground( mQueuedWidget->autoFillBackground() );
}
