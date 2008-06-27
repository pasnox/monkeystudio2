#include "pQueuedStatusBar.h"

pQueuedStatusBar::pQueuedStatusBar( QWidget* parent )
	: QStatusBar( parent )
{
	mDefaultPalette = palette();
	// create pQueuedMessageWidget
	mQueuedWidget = new pQueuedMessageWidget( this );
	addWidget( mQueuedWidget, 100 );
	// connections
	connect( mQueuedWidget, SIGNAL( messageShown( const pQueuedMessage& ) ), this, SLOT( messageShown( const pQueuedMessage& ) ) );
	connect( mQueuedWidget, SIGNAL( cleared() ), this, SLOT( messageCleared() ) );
}

int pQueuedStatusBar::appendMessage( const pQueuedMessage& message )
{ return mQueuedWidget->append( message ); }

int pQueuedStatusBar::appendMessage( const QString& message, int milliseconds, const QPixmap pixmap, const QBrush& background, const QBrush& foreground )
{ return mQueuedWidget->append( message, milliseconds, pixmap, background, foreground ); }

void pQueuedStatusBar::removeMessage( const pQueuedMessage& message )
{ mQueuedWidget->remove( message ); }

void pQueuedStatusBar::removeMessage( int id )
{ mQueuedWidget->remove( id ); }

void pQueuedStatusBar::messageShown( const pQueuedMessage& message )
{
	setAutoFillBackground( true );
	QPalette pal = mDefaultPalette;
	pal.setBrush( backgroundRole(), message.Background );
	setPalette( pal );
}

void pQueuedStatusBar::messageCleared()
{
	setAutoFillBackground( false );
	setPalette( mDefaultPalette );
}
