/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : pDockMessageBox
** FileName  : pDockMessageBox.cpp
** Date      : 2007-08-30T18:52:51
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pDockMessageBox.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QTabWidget>
#include <QListWidget>
#include <QTextBrowser>
#include <QScrollBar>

pDockMessageBox::pDockMessageBox( QWidget* w )
	: QDockWidget( w ), mShown( false )
{
	// create central widget
	QWidget* c = new QWidget;
	// create vlayout
	QVBoxLayout* vl = new QVBoxLayout( c );
	vl->setMargin( 0 );
	vl->setSpacing( 3 );
	// create hlayout
	QHBoxLayout* hl = new QHBoxLayout;
	hl->setMargin( 3 );
	hl->setSpacing( 3 );
	// create label
	lRawCommand = new QLabel;
	lRawCommand->setText( tr( "Send Raw Command :" ) );
	// create lineedit
	leRawCommand = new QLineEdit;
	leRawCommand->setToolTip( tr( "You can enter here a command and press return to execute it." ) );
	// create toolbutton
	tbStopCommand = new QToolButton;
	tbStopCommand->setToolTip( tr( "Stop the current command." ) );
	tbStopCommand->setIcon( QIcon( ":/icons/stop.png" ) );
	tbStopCommand->setEnabled( false );
	// add widget to hlayout
	hl->addWidget( lRawCommand );
	hl->addWidget( leRawCommand );
	hl->addWidget( tbStopCommand );
	// add hlayout into vlayout
	vl->addLayout( hl );
	// create tabwidget
	twMessageBox = new QTabWidget;
	twMessageBox->setMinimumHeight( 80 );
	twMessageBox->setTabPosition( QTabWidget::East );
	// create listwidget
	lwBuildSteps = new QListWidget;
	//lwBuildSteps->setMinimumHeight( 1 );
	lwBuildSteps->setFrameShape( QFrame::NoFrame );
	// create textbrowser
	tbOutput = new QTextBrowser;
	tbOutput->setFrameShape( QFrame::NoFrame );
	tbOutput->setLineWrapMode( QTextEdit::NoWrap );
	tbOutput->setTabStopWidth( 40 );
	tbOutput->setOpenExternalLinks( true );
	// create textedit
	teLog = new QTextEdit;
	teLog->setReadOnly( true );
	teLog->setFrameShape( QFrame::NoFrame );
	teLog->setLineWrapMode( QTextEdit::NoWrap );
	teLog->setTabStopWidth( 40 );
	// add widget to tabwidget
	twMessageBox->addTab( lwBuildSteps, QIcon( ":/icons/tabbuild.png" ), "" );
	twMessageBox->addTab( tbOutput, QIcon( ":/icons/taboutput.png" ), "" );
	twMessageBox->addTab( teLog, QIcon( ":/icons/tablog.png" ), "" );
	// add tab widget into vlayout
	vl->addWidget( twMessageBox );
	// set central widget to w
	setWidget( c );
	// connections
	connect( leRawCommand, SIGNAL( returnPressed() ), this, SLOT( leRawCommand_returnPressed() ) );
	connect( tbStopCommand, SIGNAL( clicked() ), this, SLOT( tbStopCommand_clicked() ) );
	connect( pConsoleManager::instance(), SIGNAL( commandError( pCommand*, QProcess::ProcessError ) ), this, SLOT( commandError( pCommand*, QProcess::ProcessError ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandFinished( pCommand*, int, QProcess::ExitStatus ) ), this, SLOT( commandFinished( pCommand*, int, QProcess::ExitStatus ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandReadyRead( pCommand*, const QByteArray& ) ), this, SLOT( commandReadyRead( pCommand*, const QByteArray& ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandStarted( pCommand* ) ), this, SLOT( commandStarted( pCommand* ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandStateChanged( pCommand*, QProcess::ProcessState ) ), this, SLOT( commandStateChanged( pCommand*, QProcess::ProcessState ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandSkipped( pCommand* ) ), this, SLOT( commandSkipped( pCommand* ) ) );
}

pDockMessageBox::~pDockMessageBox()
{}

void pDockMessageBox::showEvent( QShowEvent* e )
{
	// default event
	QDockWidget::showEvent( e );
	// check if first showEvent
	if ( !mShown )
	{
		mShown = true;
		// restore drive and path
		emit restoreSettings();
	}
}

void pDockMessageBox::hideEvent( QHideEvent* e )
{
	// default event
	QDockWidget::hideEvent( e );
	// save drive and path
	if ( mShown )
		emit saveSettings();
}

QString pDockMessageBox::colourText( const QString& s, const QColor& c )
{ return QString( "<font color=\"%1\">%2</font>" ).arg( c.name() ).arg( s ); }

void pDockMessageBox::appendOutput( const QString& s )
{
	// we check if the scroll bar is at maximum
	int p = tbOutput->verticalScrollBar()->value();
	bool b = p == tbOutput->verticalScrollBar()->maximum();
	// appendOutput text
	tbOutput->moveCursor( QTextCursor::End );
	tbOutput->insertHtml( s +"<br />" );
	// if scrollbar is at maximum, increase it
	tbOutput->verticalScrollBar()->setValue( b ? tbOutput->verticalScrollBar()->maximum() : p );
}

void pDockMessageBox::appendLog( const QString& s )
{
	// we check if the scroll bar is at maximum
	int p = teLog->verticalScrollBar()->value();
	bool b = p == teLog->verticalScrollBar()->maximum();
	// appendOutput text
	teLog->moveCursor( QTextCursor::End );
	teLog->insertHtml( s +"<br />" );
	// if scrollbar is at maximum, increase it
	teLog->verticalScrollBar()->setValue( b ? teLog->verticalScrollBar()->maximum() : p );
}

void pDockMessageBox::appendInBox( const QString& s, const QColor& c )
{
	appendLog( colourText( "********************************************************************************", c ) );
	appendLog( s );
	appendLog( colourText( "********************************************************************************", c ) );
}

void pDockMessageBox::showBuild()
{
	// show it if need
	if ( !isVisible() )
		show();
	// show correct tab if needed
	if ( twMessageBox->currentWidget() != lwBuildSteps )
		twMessageBox->setCurrentWidget( lwBuildSteps );
}

void pDockMessageBox::showOutput()
{
	// show it if need
	if ( !isVisible() )
		show();
	// show correct tab if needed
	if ( twMessageBox->currentWidget() != tbOutput )
		twMessageBox->setCurrentWidget( tbOutput );
}

void pDockMessageBox::showLog()
{
	// show it if need
	if ( !isVisible() )
		show();
	// show correct tab if needed
	if ( twMessageBox->currentWidget() != teLog )
		twMessageBox->setCurrentWidget( teLog );
}

void pDockMessageBox::leRawCommand_returnPressed()
{
	// send command
	pConsoleManager::instance()->sendRawCommand( leRawCommand->text() );
	// clear lineedit
	leRawCommand->clear();
}

void pDockMessageBox::tbStopCommand_clicked()
{ pConsoleManager::instance()->stopCurrentCommand(); }

void pDockMessageBox::commandError( pCommand* c, QProcess::ProcessError e )
{
	QString s( tr( "* Error            : '%1'<br />" ).arg( colourText( c->text() ) ) );
	s.append( tr( "* Command          : %1<br />" ).arg( colourText( c->command() ) ) );
	s.append( tr( "* Arguments        : %1<br />" ).arg( colourText( c->arguments().join( " " )  )) );
	s.append( tr( "* Working Directory: %1<br />" ).arg( colourText( c->workingDirectory() ) ) );
	s.append( tr( "* Error            : #%1<br />" ).arg( colourText( QString::number( e ) ) ) );
	//
	switch ( e )
	{
		case QProcess::FailedToStart:
			s.append( colourText( tr( "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program." ), Qt::darkGreen ) );
			// disable stop button
			tbStopCommand->setEnabled( false );
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
	// appendOutput to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
}

void pDockMessageBox::commandFinished( pCommand* c, int i, QProcess::ExitStatus e )
{
	QString s( tr( "* Finished   : '%1'<br />" ).arg( colourText( c->text() ) ) );
	s.append( tr( "* Exit Code  : #%1<br />" ).arg( colourText( QString::number( i ) ) ) );
	s.append( tr( "* Status Code: #%1<br />" ).arg( colourText( QString::number( e ) ) ) );
	//
	switch ( e )
	{
		case QProcess::NormalExit:
			s.append( colourText( tr( "The process exited normally." ), Qt::darkGreen ) );
			break;
		case QProcess::CrashExit:
			s.append( colourText( tr( "The process crashed." ), Qt::darkGreen ) );
			break;
		default:
			s.append( colourText( tr( "An unknown error occurred." ), Qt::darkGreen ) );
	}
	// appendOutput to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
	// disable stop button
	tbStopCommand->setEnabled( false );
}

void pDockMessageBox::commandReadyRead( pCommand*, const QByteArray& a )
{
	// we check if the scroll bar is at maximum
	int p = teLog->verticalScrollBar()->value();
	bool b = p == teLog->verticalScrollBar()->maximum();
	// appendOutput log
	teLog->moveCursor( QTextCursor::End );
	teLog->insertPlainText( a );
	// if scrollbar is at maximum, increase it, else restore last position
	teLog->verticalScrollBar()->setValue( b ? teLog->verticalScrollBar()->maximum() : p );
}

void pDockMessageBox::commandStarted( pCommand* c )
{
	QString s( tr( "* Started          : '%1'<br />" ).arg( colourText( c->text() ) ) );
	s.append( tr( "* Command          : %1<br />" ).arg( colourText( c->command() ) ) );
	s.append( tr( "* Arguments        : %1<br />" ).arg( colourText( c->arguments().join( " " )  )) );
	s.append( tr( "* Working Directory: %1" ).arg( colourText( c->workingDirectory() ) ) );
	// appendOutput to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
}

void pDockMessageBox::commandStateChanged( pCommand* c, QProcess::ProcessState s )
{
	QString ss;
	switch ( s )
	{
		case QProcess::NotRunning:
			ss = tr( "Not Running" );
			break;
		case QProcess::Starting:
			ss = tr( "Starting" );
			tbStopCommand->setEnabled( true );
			break;
		case QProcess::Running:
			ss = tr( "Running" );
			break;
	}
	// appendOutput to console log
	appendOutput( colourText( tr( "*** State changed to %1" ).arg( ss ), Qt::gray ) );
	appendLog( colourText( tr( "*** State changed to #%1 (%2) for command: '%3'" ).arg( s ).arg( ss ).arg( c->text() ), Qt::gray ) );
}

void pDockMessageBox::commandSkipped( pCommand* c )
{
	QString s( tr( "* Skipped          : '%1'<br />" ).arg( colourText( c->text() ) ) );
	s.append( tr( "* Command          : %1<br />" ).arg( colourText( c->command() ) ) );
	s.append( tr( "* Arguments        : %1<br />" ).arg( colourText( c->arguments().join( " " )  )) );
	s.append( tr( "* Working Directory: %1" ).arg( colourText( c->workingDirectory() ) ) );
	s.append( colourText( tr( "The command has been skipped due to previous error." ), Qt::darkGreen ) );
	// appendOutput to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
}
