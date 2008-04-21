#include "MessageBoxDocks.h"

#include <QScrollBar>
#include <QInputDialog>

#include <coremanager.h>
#include <workspacemanager.h>
#include <xupmanager.h>

MessageBoxDocks::MessageBoxDocks( QObject* parent )
	: QObject( parent )
{
	mBuildStep = new UIBuildStep;
	mOutput = new UIOutput;
	mCommand = new UICommand;
	mSearchResult = new UISearchResult;
	// connections
	connect( mBuildStep->lwBuildSteps, SIGNAL( itemPressed( QListWidgetItem* ) ), this, SLOT( lwBuildSteps_itemPressed( QListWidgetItem* ) ) );
	connect( mOutput->cbRawCommand->lineEdit(), SIGNAL( returnPressed() ), this, SLOT( cbRawCommand_returnPressed() ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandError( const pCommand&, QProcess::ProcessError ) ), this, SLOT( commandError( const pCommand&, QProcess::ProcessError ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandFinished( const pCommand&, int, QProcess::ExitStatus ) ), this, SLOT( commandFinished( const pCommand&, int, QProcess::ExitStatus ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandReadyRead( const pCommand&, const QByteArray& ) ), this, SLOT( commandReadyRead( const pCommand&, const QByteArray& ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandStarted( const pCommand& ) ), this, SLOT( commandStarted( const pCommand& ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandStateChanged( const pCommand&, QProcess::ProcessState ) ), this, SLOT( commandStateChanged( const pCommand&, QProcess::ProcessState ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandSkipped( const pCommand& ) ), this, SLOT( commandSkipped( const pCommand& ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ), this, SLOT( appendStep( const pConsoleManager::Step& ) ) );
	connect( mSearchResult->lwSearchResults, SIGNAL( itemPressed( QListWidgetItem* ) ), this, SLOT( lwSearchResults_itemPressed( QListWidgetItem* ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( appendSearchResult( const pConsoleManager::Step& ) ), this, SLOT( appendSearchResult( const pConsoleManager::Step& ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( clearSearchResults() ), this, SLOT( clearSearchResults() ) );
}

MessageBoxDocks::~MessageBoxDocks()
{
	delete mBuildStep;
	delete mOutput;
	delete mCommand;
	delete mSearchResult;
}

QString MessageBoxDocks::colourText( const QString& s, const QColor& c )
{ return QString( "<font color=\"%1\">%2</font>" ).arg( c.name() ).arg( s ); }

void MessageBoxDocks::appendOutput( const QString& s )
{
	// we check if the scroll bar is at maximum
	int p = mOutput->tbOutput->verticalScrollBar()->value();
	bool b = p == mOutput->tbOutput->verticalScrollBar()->maximum();
	// appendOutput text
	mOutput->tbOutput->moveCursor( QTextCursor::End );
	mOutput->tbOutput->insertHtml( s +"<br />" );
	// if scrollbar is at maximum, increase it
	mOutput->tbOutput->verticalScrollBar()->setValue( b ? mOutput->tbOutput->verticalScrollBar()->maximum() : p );
}

void MessageBoxDocks::appendLog( const QString& s )
{
	// we check if the scroll bar is at maximum
	int p = mCommand->teLog->verticalScrollBar()->value();
	bool b = p == mCommand->teLog->verticalScrollBar()->maximum();
	// appendOutput text
	mCommand->teLog->moveCursor( QTextCursor::End );
	mCommand->teLog->insertHtml( s +"<br />" );
	// if scrollbar is at maximum, increase it
	mCommand->teLog->verticalScrollBar()->setValue( b ? mCommand->teLog->verticalScrollBar()->maximum() : p );
}

void MessageBoxDocks::appendInBox( const QString& s, const QColor& c )
{
	appendLog( colourText( "********************************************************************************", c ) );
	appendLog( s );
	appendLog( colourText( "********************************************************************************", c ) );
}

void MessageBoxDocks::appendStep( const pConsoleManager::Step& s )
{
	// get last type
	pConsoleManager::StepType t = pConsoleManager::stUnknown;
	QListWidgetItem* lastIt = mBuildStep->lwBuildSteps->item( mBuildStep->lwBuildSteps->count() -1 );
	if ( lastIt )
		t = ( pConsoleManager::StepType )lastIt->data( Qt::UserRole +1 ).toInt();
	
	// create new/update item
	QListWidgetItem* it;
	switch( t )
	{
		case pConsoleManager::stCompiling:
		case pConsoleManager::stLinking:
			if ( s.mType == pConsoleManager::stWarning || s.mType == pConsoleManager::stError )
			{   /* move down */
				lastIt = mBuildStep->lwBuildSteps->takeItem( mBuildStep->lwBuildSteps->count() -1 );
				it = new QListWidgetItem( mBuildStep->lwBuildSteps );
				mBuildStep->lwBuildSteps->addItem( lastIt );
			}
			else
			{
				it = lastIt; /* overwrite */
			}
		break;
		default:
			it = new QListWidgetItem( mBuildStep->lwBuildSteps );
		break;
	}
	
	// set item infos
	it->setText( s.mText );
	it->setToolTip( s.mFullText );
	it->setData( Qt::UserRole +1, s.mType ); // type
	it->setData( Qt::UserRole +2, s.mFileName ); // filename
	it->setData( Qt::UserRole +3, s.mPosition ); // position
	
	// if item is finish, need calculate error, warning
	if ( s.mType == pConsoleManager::stFinish )
		if ( s.mText.isNull() )
		{ //No own text
			// count error, warning
			int e = 0, w = 0;
			for ( int i = 0; i < mBuildStep->lwBuildSteps->count() -1; i++ )
			{
				lastIt = mBuildStep->lwBuildSteps->item( i );
				if ( lastIt->data( Qt::UserRole +1 ).toInt() == pConsoleManager::stError )
					e++;
				if ( lastIt->data( Qt::UserRole +1 ).toInt() == pConsoleManager::stWarning )
					w++;
			}
			it->setData( Qt::UserRole +1, e ? pConsoleManager::stBad : pConsoleManager::stGood );
			it->setText( tr( "Command terminated, error(s): %1, warning(s): %2" ).arg( e ).arg( w ) );
		}
		else //own text present
		{
			it->setData( Qt::UserRole +1,pConsoleManager::stBad );
			it->setText( s.mText );
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

void MessageBoxDocks::appendSearchResult( const pConsoleManager::Step& s )
{
	showSearchResults();
	QListWidgetItem* it = new QListWidgetItem( mSearchResult->lwSearchResults );
	// set item infos
	it->setText( s.mText );
	it->setToolTip( s.mFullText );
	it->setData( Qt::UserRole +1, s.mType ); // type
	it->setData( Qt::UserRole +2, s.mFileName ); // filename
	it->setData( Qt::UserRole +3, s.mPosition ); // position
}

void MessageBoxDocks::showBuild()
{
	// show it if need
	if ( !mBuildStep->isVisible() )
		mBuildStep->show();
}

void MessageBoxDocks::showOutput()
{
	// show it if need
	if ( !mOutput->isVisible() )
		mOutput->show();
}

void MessageBoxDocks::showLog()
{
	// show it if need
	if ( !mCommand->isVisible() )
		mCommand->show();
}

void MessageBoxDocks::showSearchResults()
{
	// show it if need
	if ( !mSearchResult->isVisible() )
		mSearchResult->show();
}

void MessageBoxDocks::showNextError()
{
	// show it if need
	if ( !mBuildStep->isVisible() )
		mBuildStep->show();
	// find next step item
	int i = mBuildStep->lwBuildSteps->currentRow () +1 ;
	while ( i < mBuildStep->lwBuildSteps->count() && mBuildStep->lwBuildSteps->item( i )->data( Qt::UserRole +2 ).toString().isEmpty() )
		i++;
	if ( i < mBuildStep->lwBuildSteps->count() ) //finded item with setted file name
	{
		mBuildStep->lwBuildSteps->setCurrentRow( i );
		lwBuildSteps_itemPressed( mBuildStep->lwBuildSteps->item( i ) );
	}
}

void MessageBoxDocks::clearSearchResults()
{
	mSearchResult->lwSearchResults->clear();
}

void MessageBoxDocks::lwBuildSteps_itemPressed( QListWidgetItem* it )
{
	// get filename
	QString s = it->data( Qt::UserRole +2 ).toString();
	
	// cancel if no file
	if ( s.isEmpty() )
		return;

	QStringList l;

	//search in the opened files
	foreach ( pAbstractChild* c, MonkeyCore::workspace()->children() )
		foreach ( QString f, c->files() )
			if ( f.endsWith( s ) )
				l << f;
	
	// search in the current project
	if ( XUPItem* pi = MonkeyCore::fileManager()->currentProject() )
	{
		// get top project of current project
		pi = pi->topLevelProject();
		// search file
		foreach ( XUPItem* cit, pi->children( true, false ) << pi )
		{
			if ( cit->isProject() )
			{
				QString file = cit->filePath( s );
				if ( QFile::exists( file ) && !l.contains( file ) )
					l << file;
			}
		}
	}
	
	// cancel if no file
	if ( l.isEmpty() )
		return;
	
	// ask user to select file
	bool b = true;
	s = l.value( 0 );
	if ( l.count() > 1 )
		s = QInputDialog::getItem( mBuildStep->window(), tr( "Choose a file..." ), tr( "Choose the file to open" ), l, 0, false, &b, Qt::Sheet );
	
	// open file if ok
	if ( b && !s.isEmpty() )
		MonkeyCore::fileManager()->goToLine( s, it->data( Qt::UserRole +3 ).toPoint(), true );
}

void MessageBoxDocks::lwSearchResults_itemPressed( QListWidgetItem* it )
{
	// get filename
	QString s = it->data( Qt::UserRole +2 ).toString();
	MonkeyCore::fileManager()->goToLine( s, it->data( Qt::UserRole +3 ).toPoint(), true );
}

void MessageBoxDocks::cbRawCommand_returnPressed()
{
	// send command
	MonkeyCore::consoleManager()->sendRawCommand( mOutput->cbRawCommand->currentText() );
	// clear lineedit
	mOutput->cbRawCommand->setCurrentIndex( -1 );
}

void MessageBoxDocks::commandError( const pCommand& c, QProcess::ProcessError e )
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

void MessageBoxDocks::commandFinished( const pCommand& c, int exitCode, QProcess::ExitStatus e )
{
	QString s( tr( "* Finished   : '%1'<br />" ).arg( colourText( c.text() ) ) );
	s.append( tr( "* Exit Code  : #%1<br />" ).arg( colourText( QString::number( exitCode ) ) ) );
	s.append( tr( "* Status Code: #%1<br />" ).arg( colourText( QString::number( e ) ) ) );
	//
	if (e == QProcess::NormalExit && exitCode == 0)
		s.append( colourText( tr( "The process exited normally." ), Qt::darkGreen ) );
	else if (e == QProcess::CrashExit)
		s.append( colourText( tr( "The process crashed." ), Qt::darkGreen ) );
	else
		s.append( colourText( tr( "The exited with exit code %1" ).arg(exitCode), Qt::darkGreen ) );

	// appendOutput to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
	// add finish step
	if (exitCode == 0)
		appendStep( pConsoleManager::Step( pConsoleManager::stFinish ) );
	else
	{
		pConsoleManager::Step st ( pConsoleManager::stFinish );
		st.mText = tr("Process finished with exit code %1").arg(exitCode);
		appendStep(st);
	}
	
}

void MessageBoxDocks::commandReadyRead( const pCommand&, const QByteArray& a )
{
	// we check if the scroll bar is at maximum
	int p = mOutput->tbOutput->verticalScrollBar()->value();
	bool b = p == mOutput->tbOutput->verticalScrollBar()->maximum();
	// appendOutput log
	mOutput->tbOutput->moveCursor( QTextCursor::End );
	mOutput->tbOutput->insertPlainText( QTextCodec::codecForLocale()->toUnicode( a ) );
	// if scrollbar is at maximum, increase it, else restore last position
	mOutput->tbOutput->verticalScrollBar()->setValue( b ? mOutput->tbOutput->verticalScrollBar()->maximum() : p );
}

void MessageBoxDocks::commandStarted( const pCommand& c )
{
	QString s( tr( "* Started          : '%1'<br />" ).arg( colourText( c.text() ) ) );
	s.append( tr( "* Command          : %1<br />" ).arg( colourText( c.command() ) ) );
	s.append( tr( "* Arguments        : %1<br />" ).arg( colourText( c.arguments() ) ) );
	s.append( tr( "* Working Directory: %1" ).arg( colourText( c.workingDirectory() ) ) );
	// appendOutput to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
	// show dock if needed
	if ( !mOutput->isVisible() )
		mOutput->setVisible( true );
}

void MessageBoxDocks::commandStateChanged( const pCommand& c, QProcess::ProcessState s )
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
			mBuildStep->lwBuildSteps->clear();
			mOutput->tbOutput->clear();
			mCommand->teLog->clear();
			break;
		case QProcess::Running:
			ss = tr( "Running" );
			break;
	}
	// appendOutput to console log
	appendOutput( colourText( tr( "*** State changed to %1" ).arg( ss ), Qt::gray ) );
	appendLog( colourText( tr( "*** State changed to #%1 (%2) for command: '%3'" ).arg( s ).arg( ss ).arg( c.text() ), Qt::gray ) );
}

void MessageBoxDocks::commandSkipped( const pCommand& c )
{
	QString s( tr( "* Skipped          : '%1'<br />" ).arg( colourText( c.text() ) ) );
	s.append( tr( "* Command          : %1<br />" ).arg( colourText( c.command() ) ) );
	s.append( tr( "* Arguments        : %1<br />" ).arg( colourText( c.arguments() ) ) );
	s.append( tr( "* Working Directory: %1" ).arg( colourText( c.workingDirectory() ) ) );
	s.append( colourText( tr( "The command has been skipped due to previous error." ), Qt::darkGreen ) );
	// appendOutput to console log
	appendInBox( colourText( s, Qt::blue ), Qt::red );
}
