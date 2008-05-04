#include "QueuedStatusBar.h"

#include <QLabel>

QueuedStatusBar::QueuedStatusBar( QWidget* parent )
	: pQueuedStatusBar( parent )
{
	// create labels
	QLabel* l;
	l = ( mLabels[ltSaveState] = new QLabel( this ) );
	l->setFrameShape( QFrame::StyledPanel );
	//l->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	l = ( mLabels[ltEOLMode] = new QLabel( this ) );
	l->setFrameShape( QFrame::StyledPanel );
	//l->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	l = ( mLabels[ltIndentMode] = new QLabel( this ) );
	l->setFrameShape( QFrame::StyledPanel );
	//l->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	l = ( mLabels[ltCursorPosition] = new QLabel( this ) );
	l->setFrameShape( QFrame::StyledPanel );
	//l->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	// add labels to status bar as permanent widgets
	addPermanentWidget( mLabels[ltSaveState] );
	addPermanentWidget( mLabels[ltEOLMode] );
	addPermanentWidget( mLabels[ltIndentMode] );
	addPermanentWidget( mLabels[ltCursorPosition] );
	//
	setFixedHeight( 24 );
}

void QueuedStatusBar::hideOrShow()
{
	pQueuedStatusBar::hideOrShow();
	if ( mQueuedWidget->isVisible() && mQueuedWidget->messagesCount() == 0 )
		mQueuedWidget->hide();
}

QLabel* QueuedStatusBar::label( QueuedStatusBar::LabelType type )
{
	return mLabels[type];
}

void QueuedStatusBar::setModified( bool modified )
{
	label( ltSaveState )->setPixmap( QIcon( QPixmap( ":/file/icons/file/save.png" ) ).pixmap( QSize( 16, 16 ), modified ? QIcon::Normal : QIcon::Disabled ) );
}

void QueuedStatusBar::setEOLMode( QsciScintilla::EolMode mode )
{
	QString s = tr( "Eol Mode: %1" );
	switch ( mode )
	{
		case QsciScintilla::EolWindows:
			label( ltEOLMode )->setText( s.arg( "Windows" ) );
			break;
		case QsciScintilla::EolUnix:
			label( ltEOLMode )->setText( s.arg( "Unix" ) );
			break;
		case QsciScintilla::EolMac:
			label( ltEOLMode )->setText( s.arg( "Mac" ) );
			break;
		default:
			label( ltEOLMode )->setText( s.arg( "-" ) );
			break;
	}
}

void QueuedStatusBar::setIndentMode( int mode )
{
	QString s = tr( "Indent Mode: %1" );
	switch ( mode )
	{
		case -1:
			label( ltIndentMode )->setText( s.arg( "-" ) );
			break;
		case 0:
			label( ltIndentMode )->setText( s.arg( tr( "Spaces" ) ) );
			break;
		case 1:
			label( ltIndentMode )->setText( s.arg( tr( "Tabs" ) ) );
			break;
	}
}

void QueuedStatusBar::setCursorPosition( const QPoint& pos )
{
	QString s = tr( "Line: %1, Column: %2" );
	label( ltCursorPosition )->setText( pos == QPoint( -1, -1 ) ? s.arg( "-" ).arg( "-" ) : s.arg( pos.y() ).arg( pos.x() ) );
}
