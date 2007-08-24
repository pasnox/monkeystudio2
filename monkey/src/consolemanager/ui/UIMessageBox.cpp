#include "UIMessageBox.h"

#include <QScrollBar>

UIMessageBox::UIMessageBox()
	: QDockWidget()
{
	setupUi( this );
	setMinimumHeight( 100 );
	
	// connections
	connect( pConsoleManager::instance(), SIGNAL( commandError( pCommand*, QProcess::ProcessError ) ), this, SLOT( commandError( pCommand*, QProcess::ProcessError ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandFinished( pCommand*, int, QProcess::ExitStatus ) ), this, SLOT( commandFinished( pCommand*, int, QProcess::ExitStatus ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandReadyRead( pCommand*, const QByteArray& ) ), this, SLOT( commandReadyRead( pCommand*, const QByteArray& ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandStarted( pCommand* ) ), this, SLOT( commandStarted( pCommand* ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandStateChanged( pCommand*, QProcess::ProcessState ) ), this, SLOT( commandStateChanged( pCommand*, QProcess::ProcessState ) ) );
}

QString UIMessageBox::colourText( const QString& s, const QColor& c )
{
	return QString( "<font color=\"%1\">%2</font>" ).arg( c.name() ).arg( s );
}

void UIMessageBox::append( const QString& s )
{
	// we check if the scroll bar is at maximum
	int p = tbMessages->verticalScrollBar()->value();
	bool b = p == tbMessages->verticalScrollBar()->maximum();
	
	// append text
	tbMessages->moveCursor( QTextCursor::End );
	tbMessages->insertHtml( s +"<br />" );
	
	// if scrollbar is at maximum, increase it
	tbMessages->verticalScrollBar()->setValue( b ? tbMessages->verticalScrollBar()->maximum() : p );
}

void UIMessageBox::appendInBox( const QString& s, const QColor& c )
{
	append( colourText( "********************************************************************************", c ) );
	append( s );
	append( colourText( "********************************************************************************", c ) );
}

void UIMessageBox::commandError( pCommand* c, QProcess::ProcessError e )
{
	QString s( tr( "* Error            : '%1'<br />" ).arg( colourText( c->text() ) ) );
	s.append( tr( "* Command          : %1<br />" ).arg( colourText( c->command() ) ) );
	s.append( tr( "* Arguments        : %1<br />" ).arg( colourText( c->arguments().join( " " )  )) );
	s.append( tr( "* Working Directory: %1<br />" ).arg( colourText( c->workingDirectory() ) ) );
	s.append( tr( "* Error            : #%1<br />" ).arg( colourText( QString::number( e ) ) ) );
	
	switch ( e )
	{
		case QProcess::FailedToStart:
			s.append( colourText( tr( "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program." ), Qt::darkGreen ) );
			break;
		case QProcess::Crashed:
			s.append( colourText( tr( "The process crashed some time after starting successfully." ), Qt::darkGreen ) );
			break;
		case QProcess::Timedout:
			s.append( colourText( tr( "The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again." ), Qt::darkGreen ) );
			break;
		case QProcess::WriteError:
			s.append( colourText( tr( "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel." ), Qt::darkGreen ) );
			break;
		case QProcess::ReadError:
			s.append( colourText( tr( "An error occurred when attempting to read from the process. For example, the process may not be running." ), Qt::darkGreen ) );
			break;
		case QProcess::UnknownError:
		default:
			s.append( colourText( tr( "An unknown error occurred. This is the default return value of error()." ), Qt::darkGreen ) );
			break;
	}
	
	// append to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
}

void UIMessageBox::commandFinished( pCommand* c, int i, QProcess::ExitStatus e )
{
	QString s( tr( "* Finished   : '%1'<br />" ).arg( colourText( c->text() ) ) );
	s.append( tr( "* Exit Code  : #%1<br />" ).arg( colourText( QString::number( i ) ) ) );
	s.append( tr( "* Status Code: #%1<br />" ).arg( colourText( QString::number( e ) ) ) );
	
	switch ( e )
	{
		case QProcess::NormalExit:
			s.append( colourText( tr( "The process exited normally." ), Qt::darkGreen ) );
			break;
		case QProcess::CrashExit:
			s.append( colourText( tr( "EThe process crashed." ), Qt::darkGreen ) );
			break;
		default:
			s.append( colourText( tr( "An unknown error occurred." ), Qt::darkGreen ) );
	}
	
	// append to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
}

void UIMessageBox::commandReadyRead( pCommand* c, const QByteArray& a )
{
	// we check if the scroll bar is at maximum
	int p = tbMessages->verticalScrollBar()->value();
	bool b = p == tbMessages->verticalScrollBar()->maximum();
	
	// append log
	tbMessages->moveCursor( QTextCursor::End );
	tbMessages->insertPlainText( a );
	
	// if scrollbar is at maximum, increase it, else restore last position
	tbMessages->verticalScrollBar()->setValue( b ? tbMessages->verticalScrollBar()->maximum() : p );
}

void UIMessageBox::commandStarted( pCommand* c )
{
	QString s( tr( "* Started          : '%1'<br />" ).arg( colourText( c->text() ) ) );
	s.append( tr( "* Command          : %1<br />" ).arg( colourText( c->command() ) ) );
	s.append( tr( "* Arguments        : %1<br />" ).arg( colourText( c->arguments().join( " " )  )) );
	s.append( tr( "* Working Directory: %1" ).arg( colourText( c->workingDirectory() ) ) );
	
	// append to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
}

void UIMessageBox::commandStateChanged( pCommand* c, QProcess::ProcessState s )
{
	QString ss;
	switch ( s )
	{
		case QProcess::NotRunning:
			ss = tr( "Not Running" );
			break;
		case QProcess::Starting:
			ss = tr( "Starting" );
			break;
		case QProcess::Running:
			ss = tr( "Running" );
			break;
	}
	
	// append to console log
	append( colourText( tr( "*** State changed to #%1 (%2) for command: '%3'" ).arg( s ).arg( ss ).arg( c->text() ), Qt::gray ) );
}
