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

void UIMessageBox::commandError( pCommand* c, QProcess::ProcessError e )
{
}

void UIMessageBox::commandFinished( pCommand* c, int i, QProcess::ExitStatus e )
{
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
}

void UIMessageBox::commandStateChanged( pCommand* c, QProcess::ProcessState s )
{
}
