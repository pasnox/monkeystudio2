#include "pQueuedMessageWidget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include <QShowEvent>
#include <QPushButton>
#include <QStyle>
#include <QMetaMethod>

int pQueuedMessageWidget::pQueuedMessageUniqueId = 0;

pQueuedMessageWidget::pQueuedMessageWidget( QWidget* parent )
	: QWidget( parent )
{
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
	dbbButtons->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	// layout
	QHBoxLayout* hbl = new QHBoxLayout( this );
	hbl->setMargin( 0 );
	hbl->addWidget( lPixmap, 0, Qt::AlignTop | Qt::AlignHCenter );
	hbl->addWidget( lMessage );
	hbl->addWidget( dbbButtons, 0, Qt::AlignTop | Qt::AlignHCenter );
	// connections
	connect( dbbButtons, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( clicked( QAbstractButton* ) ) );
}

int pQueuedMessageWidget::messagesCount() const
{ return mMessages.count(); }

int pQueuedMessageWidget::append( const pQueuedMessage& message )
{
	if ( !mMessages.values().contains( message ) )
	{
		mMessages[pQueuedMessageUniqueId] = message;
		if ( mMessages.count() == 1 )
			QTimer::singleShot( 0, this, SLOT( showMessage() ) );
		return pQueuedMessageUniqueId++;
	}
	return mMessages.key( message );
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
{ mMessages.remove( mMessages.key( message ) ); }

void pQueuedMessageWidget::remove( int id )
{ mMessages.remove( id ); }

void pQueuedMessageWidget::clear()
{
	lPixmap->clear();
	lMessage->clear();
	dbbButtons->clear();
	emit cleared();
}

void pQueuedMessageWidget::clicked( QAbstractButton* button )
{
	const pQueuedMessage msg = mMessages.begin().value();
	if ( msg.Object && msg.Slot )
		QMetaObject::invokeMethod( msg.Object, msg.Slot, Q_ARG( QDialogButtonBox::StandardButton, dbbButtons->standardButton( button ) ), Q_ARG( pQueuedMessage, msg ) );
	closeMessage();
}

void pQueuedMessageWidget::showMessage()
{
	// get message
	pQueuedMessage msg = mMessages.begin().value();
	// set palette
	QPalette pal = style()->standardPalette();
	if ( msg.Foreground != QBrush() )
		pal.setBrush( lMessage->foregroundRole(), msg.Foreground );
	lMessage->setPalette( pal );
	// format widget
	lPixmap->setPixmap( msg.Pixmap );
	lMessage->setText( msg.Message );
	lMessage->setToolTip( msg.Message );
	lMessage->setWhatsThis( msg.Message );
	// set buttons
	if ( msg.Buttons.isEmpty() )
		msg.Buttons[ QDialogButtonBox::Ok ] = QString();
	dbbButtons->clear();
	foreach( QDialogButtonBox::StandardButton button, msg.Buttons.keys() )
	{
		QPushButton* pb = dbbButtons->addButton( button );
		if ( !msg.Buttons[ button ].isEmpty() )
			pb->setText( msg.Buttons[ button ] );
	}
	// auto close if needed
	if ( msg.MilliSeconds > 0 )
		QTimer::singleShot( msg.MilliSeconds, this, SLOT( closeMessage() ) );
	// emit signal
	emit messageShown( msg );
}

void pQueuedMessageWidget::closeMessage()
{
	// emit message
	emit messageClosed( mMessages.begin().value() );
	// remove remove current message from hash
	mMessages.erase( mMessages.begin() );
	// process next if possible, else clear gui
	QTimer::singleShot( 0, this, mMessages.count() > 0 ? SLOT( showMessage() ) : SLOT( clear() ) );
	// emit finished message if needed
	if ( mMessages.count() == 0 )
		emit finished();
}
