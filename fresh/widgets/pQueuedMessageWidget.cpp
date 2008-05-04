#include "pQueuedMessageWidget.h"

#include <QLabel>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QTimer>

int pQueuedMessageWidget::pQueuedMessageUniqueId = 0;

pQueuedMessageWidget::pQueuedMessageWidget( QWidget* parent )
	: QWidget( parent )
{
	mDefaultPalette = palette();
	// pixmap
	lPixmap = new QLabel( this );
	lPixmap->setAlignment( Qt::AlignCenter );
	lPixmap->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	// message
	lMessage = new QLabel( this );
	lMessage->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
	lMessage->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
	lMessage->setWordWrap( true );
	// button
	dbbButtons = new QDialogButtonBox( this );
	dbbButtons->addButton( QDialogButtonBox::Ok );
	dbbButtons->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	// layout
	QHBoxLayout* hbl = new QHBoxLayout( this );
	hbl->setMargin( 0 );
	hbl->addWidget( lPixmap, 0, Qt::AlignTop | Qt::AlignHCenter );
	hbl->addWidget( lMessage );
	hbl->addWidget( dbbButtons, 0, Qt::AlignTop | Qt::AlignHCenter );
	// connections
	connect( dbbButtons, SIGNAL( accepted() ), this, SLOT( stopMessage() ) );
}

QSize pQueuedMessageWidget::sizeHint() const
{
	QSize size = QWidget::sizeHint();
	size.setHeight( 15 );
	return size;
}

int pQueuedMessageWidget::append( const pQueuedMessage& message )
{
	mMessages[pQueuedMessageUniqueId] = message;
	if ( mMessages.count() == 1 )
		QTimer::singleShot( 0, this, SLOT( startMessage() ) );
	return pQueuedMessageUniqueId++;
}

int pQueuedMessageWidget::append( const QString& message, int milliseconds, const QPixmap pixmap, const QBrush& background, const QBrush& foreground )
{
	pQueuedMessage m;
	m.Message = message;
	m.MilliSeconds = milliseconds;
	m.Pixmap = pixmap;
	m.Background = background;
	m.Foreground = foreground;
	return append( m );
}

void pQueuedMessageWidget::remove( const pQueuedMessage& message )
{
	mMessages.remove( mMessages.key( message ) );
}

void pQueuedMessageWidget::remove( int id )
{
	mMessages.remove( id );
}

void pQueuedMessageWidget::startMessage()
{
	// get message
	const pQueuedMessage msg = mMessages.begin().value();
	// remove it from hash
	mMessages.erase( mMessages.begin() );
	// format widget
	lPixmap->setPixmap( msg.Pixmap );
	lMessage->setText( msg.Message );
	setAutoFillBackground( false );
	QPalette pal = mDefaultPalette;
	if ( msg.Background != QBrush() )
	{
		pal.setBrush( backgroundRole(), msg.Background );
		setAutoFillBackground( true );
	}
	if ( msg.Foreground != QBrush() )
		pal.setBrush( foregroundRole(), msg.Foreground );
	setPalette( pal );
	// show message
	if ( !isVisible() )
		show();
	// emit signal
	emit messageShown();
}

void pQueuedMessageWidget::stopMessage()
{
	if ( mMessages.count() > 0 )
		startMessage();
	else
	{
		setAutoFillBackground( false );
		setPalette( mDefaultPalette );
		hide();
	}
	// emit message
	emit messageClosed();
}
