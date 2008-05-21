#include "QueuedStatusBar.h"

#include <QLabel>
#include <QStackedWidget>

QueuedStatusBar::QueuedStatusBar( QWidget* parent )
	: pQueuedStatusBar( parent )
{
	layout()->setMargin( 0 );
	mStacked = new QStackedWidget;
	//mStacked->setFrameStyle( QFrame::Sunken | QFrame::StyledPanel );
	// create labels
	QLabel* l;
	l = ( mLabels[ltCursorPosition] = new QLabel( this ) );
	l->setMargin( 2 );
	l->setToolTip( tr( "Cursor position" ) );
	l->setFrameStyle( QFrame::StyledPanel );
	l = ( mLabels[ltMessage] = new QLabel( this ) );
	l->setMargin( 2 );
	l->setFrameStyle( QFrame::StyledPanel );
	l = ( mLabels[ltSaveState] = new QLabel( this ) );
	l->setMargin( 2 );
	l->setToolTip( tr( "Modification state of file" ) );
	l->setFrameStyle( QFrame::StyledPanel );
	l = ( mLabels[ltEOLMode] = new QLabel( this ) );
	l->setMargin( 2 );
	l->setToolTip( tr( "EOL mode" ) );
	l->setFrameStyle( QFrame::StyledPanel );
	l = ( mLabels[ltIndentMode] = new QLabel( this ) );
	l->setMargin( 2 );
	l->setToolTip( tr( "Indentation mode" ) );
	l->setFrameStyle( QFrame::StyledPanel );
	//
	mWidgetLabels = new QWidget;
	QHBoxLayout* hlayout = new QHBoxLayout( mWidgetLabels );
	hlayout->setMargin( 0 );
	// add labels to layout
	hlayout->addWidget( mLabels[ltMessage] );
	hlayout->addWidget( mLabels[ltCursorPosition] );
	hlayout->addWidget( mLabels[ltSaveState] );
	hlayout->addWidget( mLabels[ltEOLMode] );
	hlayout->addWidget( mLabels[ltIndentMode] );
	hlayout->setStretchFactor( mLabels[ltMessage], 255 );
	// add widgets to stacked
	mStacked->addWidget( mWidgetLabels );
	mStacked->addWidget( mQueuedWidget );
	// add labels to status bar as permanent widgets
	addPermanentWidget( mStacked, 255 );
	// connections
	connect( mQueuedWidget, SIGNAL( messageShown( const pQueuedMessage& ) ), this, SLOT( messageShown( const pQueuedMessage& ) ) );
	connect( mQueuedWidget, SIGNAL( finished() ), this, SLOT( messageFinished() ) );
	connect( this, SIGNAL( messageChanged( const QString& ) ), this, SLOT( setMessage( const QString& ) ) );
	// remember default sizeHint height
	mHeight = sizeHint().height();
}

QLabel* QueuedStatusBar::label( QueuedStatusBar::LabelType type )
{ return mLabels[type]; }

void QueuedStatusBar::resizeEvent( QResizeEvent* event )
{
	pQueuedStatusBar::resizeEvent( event );
	QLabel* label = mLabels[ltMessage];
	if ( label->text() != currentMessage() )
	{
		QFontMetrics fm( font() );
		label->setText( fm.elidedText( currentMessage(), Qt::ElideRight, label->rect().width() ) );
		label->setToolTip( currentMessage() );
	}
}

void QueuedStatusBar::messageShown( const pQueuedMessage& /*message*/ )
{
	mStacked->setCurrentIndex( mStacked->indexOf( mQueuedWidget ) );
	setFixedHeight( mStacked->sizeHint().height() );
}

void QueuedStatusBar::messageFinished()
{
	mStacked->setCurrentIndex( mStacked->indexOf( mWidgetLabels ) );
	setFixedHeight( mHeight );
}

void QueuedStatusBar::setMessage( const QString& message )
{
	QFontMetrics fm( font() );
	QLabel* label = mLabels[ltMessage];
	label->setText( fm.elidedText( message, Qt::ElideRight, label->rect().width() ) );
	label->setToolTip( message );
}

void QueuedStatusBar::setModified( bool modified )
{ label( ltSaveState )->setPixmap( QIcon( QPixmap( ":/file/icons/file/save.png" ) ).pixmap( QSize( 16, 16 ), modified ? QIcon::Normal : QIcon::Disabled ) ); }

void QueuedStatusBar::setEOLMode( QsciScintilla::EolMode mode )
{
	switch ( mode )
	{
		case QsciScintilla::EolWindows:
			label( ltEOLMode )->setText("Windows");
			break;
		case QsciScintilla::EolUnix:
			label( ltEOLMode )->setText("Unix");
			break;
		case QsciScintilla::EolMac:
			label( ltEOLMode )->setText("Mac");
			break;
		default:
			label( ltEOLMode )->setText("-");
			break;
	}
}

void QueuedStatusBar::setIndentMode( int mode )
{
	switch ( mode )
	{
		case 0:
			label( ltIndentMode )->setText("Spaces" );
			break;
		case 1:
			label( ltIndentMode )->setText("Tabs" );
			break;
		default:
			label( ltIndentMode )->setText("-");
			break;
	}
}

void QueuedStatusBar::setCursorPosition( const QPoint& pos )
{
	QString s = tr( "Line: %1 Column: %2" );
	label( ltCursorPosition )->setText( pos == QPoint( -1, -1 ) ? s.arg( "-" ).arg( "-" ) : s.arg( pos.y() ).arg( pos.x() ) );
}
