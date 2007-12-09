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
#include "pFileManager.h"
#include "ProjectItem.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>
#include <QListWidget>
#include <QTextBrowser>
#include <QScrollBar>
#include <QTextCodec>

#include <QInputDialog>
#include <QFile>

#include "pMonkeyStudio.h"

pDockMessageBox::pDockMessageBox( QWidget* w )
	: QDockWidget( w ), mShown( false )
{
	// create tabwidget
	twMessageBox = new QTabWidget;
	twMessageBox->setTabPosition( QTabWidget::East );
	twMessageBox->setMinimumHeight( 120 );
	// create listwidget
	lwBuildSteps = new QListWidget;
	lwBuildSteps->setFrameShape( QFrame::NoFrame );
	// create textbrowser and raw command
	// create widget background
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
	// add widget to hlayout
	hl->addWidget( lRawCommand );
	hl->addWidget( leRawCommand );
	// add hlayout into vlayout
	vl->addLayout( hl );
	// text browser
	tbOutput = new QTextBrowser;
	tbOutput->setFrameShape( QFrame::NoFrame );
	tbOutput->setLineWrapMode( QTextEdit::WidgetWidth );
	tbOutput->setTabStopWidth( 40 );
	tbOutput->setOpenExternalLinks( true );
	// add it to layout
	vl->addWidget( tbOutput );
	// create textedit
	teLog = new QTextEdit;
	teLog->setReadOnly( true );
	teLog->setFrameShape( QFrame::NoFrame );
	teLog->setLineWrapMode( QTextEdit::NoWrap );
	teLog->setTabStopWidth( 40 );
	// add widget to tabwidget
	twMessageBox->addTab( lwBuildSteps, QIcon( ":/icons/tabbuild.png" ), QString::null );
	twMessageBox->addTab( c, QIcon( ":/icons/taboutput.png" ), QString::null );
	twMessageBox->addTab( teLog, QIcon( ":/icons/tablog.png" ), QString::null );
	// set tabs tooltip
	twMessageBox->setTabToolTip( 0, tr( "Build Step" ) );
	twMessageBox->setTabToolTip( 1, tr( "Program Output" ) );
	twMessageBox->setTabToolTip( 2, tr( "Commands Log" ) );
	// set central widget to tabwidget
	setWidget( twMessageBox );
	// connections
	connect( lwBuildSteps, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), this, SLOT( lwBuildSteps_itemDoubleClicked( QListWidgetItem* ) ) );
	connect( leRawCommand, SIGNAL( returnPressed() ), this, SLOT( leRawCommand_returnPressed() ) );
	connect( pConsoleManager::instance(), SIGNAL( commandError( const pCommand&, QProcess::ProcessError ) ), this, SLOT( commandError( const pCommand&, QProcess::ProcessError ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandFinished( const pCommand&, int, QProcess::ExitStatus ) ), this, SLOT( commandFinished( const pCommand&, int, QProcess::ExitStatus ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandReadyRead( const pCommand&, const QByteArray& ) ), this, SLOT( commandReadyRead( const pCommand&, const QByteArray& ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandStarted( const pCommand& ) ), this, SLOT( commandStarted( const pCommand& ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandStateChanged( const pCommand&, QProcess::ProcessState ) ), this, SLOT( commandStateChanged( const pCommand&, QProcess::ProcessState ) ) );
	connect( pConsoleManager::instance(), SIGNAL( commandSkipped( const pCommand& ) ), this, SLOT( commandSkipped( const pCommand& ) ) );
	connect( pConsoleManager::instance(), SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ), this, SLOT( appendStep( const pConsoleManager::Step& ) ) );
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

void pDockMessageBox::appendStep( const pConsoleManager::Step& s )
{
	// get last type
	pConsoleManager::StepType t = pConsoleManager::stUnknown;
	QListWidgetItem* lastIt = lwBuildSteps->item( lwBuildSteps->count() -1 );
	if ( lastIt )
		t = ( pConsoleManager::StepType )lastIt->data( Qt::UserRole +1 ).toInt();
	lastIt = 0;
	
	// create new/update item
	QListWidgetItem* it;
	switch( t )
	{
		case pConsoleManager::stCompiling:
		case pConsoleManager::stLinking:
			if ( s.mType == pConsoleManager::stWarning || s.mType == pConsoleManager::stError )
			{
				lastIt = lwBuildSteps->takeItem( lwBuildSteps->count() -1 );
				it = new QListWidgetItem( lwBuildSteps );
			}
			else
				it = lwBuildSteps->item( lwBuildSteps->count() -1 );
			break;
		default:
			it = new QListWidgetItem( lwBuildSteps );
			break;
	}
	if ( lastIt )
		lwBuildSteps->addItem( lastIt );
	
	// set item infos
	it->setText( s.mText );
	it->setToolTip( s.mFullText );
	it->setData( Qt::UserRole +1, s.mType ); // type
	it->setData( Qt::UserRole +2, s.mFileName ); // filename
	it->setData( Qt::UserRole +3, s.mPosition ); // position
	
	// if item is finish, need calculate error, warning
	if ( s.mType == pConsoleManager::stFinish )
	{
		// count error, warning
		int e = 0, w = 0;
		for ( int i = 0; i < lwBuildSteps->count() -1; i++ )
		{
			lastIt = lwBuildSteps->item( i );
			if ( lastIt->data( Qt::UserRole +1 ).toInt() == pConsoleManager::stError )
				e++;
			if ( lastIt->data( Qt::UserRole +1 ).toInt() == pConsoleManager::stWarning )
				w++;
		}
		it->setData( Qt::UserRole +1, e ? pConsoleManager::stBad : pConsoleManager::stGood );
		it->setText( tr( "Command terminated, error(s): %1, warning(s): %2" ).arg( e ).arg( w ) );
	}
	
	// set icon and color
	switch ( it->data( Qt::UserRole +1 ).toInt() )
	{
		case pConsoleManager::stError:
			it->setIcon( QIcon( ":/icons/error.png" ) );
			it->setBackground( QColor( 255, 0, 0, 20 ) );
			break;
		case pConsoleManager::stWarning:
			it->setIcon( QIcon( ":/icons/warning.png" ) );
			it->setBackground( QColor( 0, 255, 0, 20 ) );
			break;
		case pConsoleManager::stCompiling:
			it->setIcon( QIcon( ":/icons/clock.png" ) );
			it->setBackground( QColor( 0, 0, 255, 20 ) );
			break;
		case pConsoleManager::stLinking:
			it->setIcon( QIcon( ":/icons/clock.png" ) );
			it->setBackground( QColor( 0, 0, 125, 20 ) );
			break;
		case pConsoleManager::stGood:
			it->setIcon( QIcon( ":/icons/warning.png" ) );
			it->setBackground( QColor( 0, 255, 0, 90 ) );
			break;
		case pConsoleManager::stBad:
			it->setIcon( QIcon( ":/icons/error.png" ) );
			it->setBackground( QColor( 255, 0, 0, 90 ) );
			break;
		case pConsoleManager::stFinish:
			it->setBackground( QColor( 65, 65, 65, 20 ) );
			break;
		default:
			it->setIcon( QIcon() );
			it->setBackground( QColor( 125, 125, 125, 20 ) );
			break;
	}
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
	if ( twMessageBox->currentWidget() != tbOutput->parentWidget() )
		twMessageBox->setCurrentWidget( tbOutput->parentWidget() );
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

void pDockMessageBox::lwBuildSteps_itemDoubleClicked( QListWidgetItem* it )
{
	// get filename
	QString s = it->data( Qt::UserRole +2 ).toString();
	
	// cancel if no file
	if ( s.isNull() )
		return;
	
	// get current project
	if ( ProjectItem* pi = pFileManager::instance()->currentProject() )
	{
		// get top project of current project
		while ( ProjectItem* ppi = pi->parentProject() )
			pi = ppi;
		
		// search file
		QStringList l;
		foreach ( ProjectItem* it, pi->childrenProjects() << pi )
			if ( QFile::exists( it->canonicalFilePath( s ) ) )
				l << it->canonicalFilePath( s );
		
		// cancel if no file
		if ( l.isEmpty() )
			return;
		
		// ask user to select file
		bool b = true;
		s = l.value( 0 );
		if ( l.count() > 1 )
			s = QInputDialog::getItem( window(), tr( "Choose a file..." ), tr( "Choose the file to open" ), l, 0, false, &b, Qt::Sheet );
		
		// open file if ok
		if ( b && !s.isEmpty() )
			pFileManager::instance()->goToLine( s, it->data( Qt::UserRole +3 ).toPoint(), true );
	}
}

void pDockMessageBox::leRawCommand_returnPressed()
{
	// send command
	pConsoleManager::instance()->sendRawCommand( leRawCommand->text() );
	// clear lineedit
	leRawCommand->clear();
}

void pDockMessageBox::commandError( const pCommand& c, QProcess::ProcessError e )
{
	QString s( tr( "* Error            : '%1'<br />" ).arg( colourText( c.text() ) ) );
	s.append( tr( "* Command          : %1<br />" ).arg( colourText( c.command() ) ) );
	s.append( tr( "* Arguments        : %1<br />" ).arg( colourText( c.arguments() ) ) );
	s.append( tr( "* Working Directory: %1<br />" ).arg( colourText( c.workingDirectory() ) ) );
	s.append( tr( "* Error            : #%1<br />" ).arg( colourText( QString::number( e ) ) ) );
	//
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
	// appendOutput to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
}

void pDockMessageBox::commandFinished( const pCommand& c, int i, QProcess::ExitStatus e )
{
	QString s( tr( "* Finished   : '%1'<br />" ).arg( colourText( c.text() ) ) );
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
	// add finish step
	appendStep( pConsoleManager::Step( pConsoleManager::stFinish ) );
}

void pDockMessageBox::commandReadyRead( const pCommand&, const QByteArray& a )
{
	// we check if the scroll bar is at maximum
	int p = tbOutput->verticalScrollBar()->value();
	bool b = p == tbOutput->verticalScrollBar()->maximum();
	// appendOutput log
	tbOutput->moveCursor( QTextCursor::End );
	tbOutput->insertPlainText( QTextCodec::codecForLocale()->toUnicode( a ) );
	// if scrollbar is at maximum, increase it, else restore last position
	tbOutput->verticalScrollBar()->setValue( b ? tbOutput->verticalScrollBar()->maximum() : p );
}

void pDockMessageBox::commandStarted( const pCommand& c )
{
	QString s( tr( "* Started          : '%1'<br />" ).arg( colourText( c.text() ) ) );
	s.append( tr( "* Command          : %1<br />" ).arg( colourText( c.command() ) ) );
	s.append( tr( "* Arguments        : %1<br />" ).arg( colourText( c.arguments() ) ) );
	s.append( tr( "* Working Directory: %1" ).arg( colourText( c.workingDirectory() ) ) );
	// appendOutput to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
	// show dock if needed
	if ( !isVisible() )
		setVisible( true );
}

void pDockMessageBox::commandStateChanged( const pCommand& c, QProcess::ProcessState s )
{
	QString ss;
	switch ( s )
	{
		case QProcess::NotRunning:
			ss = tr( "Not Running" );
			break;
		case QProcess::Starting:
			ss = tr( "Starting" );
			// clear all tabs
			lwBuildSteps->clear();
			tbOutput->clear();
			teLog->clear();
			break;
		case QProcess::Running:
			ss = tr( "Running" );
			break;
	}
	// appendOutput to console log
	appendOutput( colourText( tr( "*** State changed to %1" ).arg( ss ), Qt::gray ) );
	appendLog( colourText( tr( "*** State changed to #%1 (%2) for command: '%3'" ).arg( s ).arg( ss ).arg( c.text() ), Qt::gray ) );
}

void pDockMessageBox::commandSkipped( const pCommand& c )
{
	QString s( tr( "* Skipped          : '%1'<br />" ).arg( colourText( c.text() ) ) );
	s.append( tr( "* Command          : %1<br />" ).arg( colourText( c.command() ) ) );
	s.append( tr( "* Arguments        : %1<br />" ).arg( colourText( c.arguments() ) ) );
	s.append( tr( "* Working Directory: %1" ).arg( colourText( c.workingDirectory() ) ) );
	s.append( colourText( tr( "The command has been skipped due to previous error." ), Qt::darkGreen ) );
	// appendOutput to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
}
