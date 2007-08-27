/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox <pasnox@gmail.com>
** Project   : ConsoleManager
** FileName  : ConsoleManager.h
** Date      : mar. août 21 20:52:27 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PCONSOLEMANAGER_H
#define PCONSOLEMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "pCommand.h"

#include <QApplication>
#include <QProcess>

class Q_MONKEY_EXPORT pConsoleManager : public QProcess, public QSingleton<pConsoleManager>
{
	Q_OBJECT
	friend class QSingleton<pConsoleManager>;
	
public:
	pCommand* currentCommand() const { return mCommands.value( 0 ); }

protected:
	int mTimerId;
	pCommandList mCommands;
	void timerEvent( QTimerEvent* );

private:
	pConsoleManager( QObject* = QApplication::instance() );
	~pConsoleManager();

public slots:
	void sendRawData( const QByteArray& );
	void addCommand( pCommand* );
	void addCommands( const pCommandList& );
	void removeCommand( pCommand* );
	void removeCommands( const pCommandList& );

private slots:
	void executeProcess();
	void error( QProcess::ProcessError );
	void finished( int, QProcess::ExitStatus );
	void readyRead();
	void started();
	void stateChanged( QProcess::ProcessState );

signals:
	void commandError( pCommand*, QProcess::ProcessError );
	void commandFinished( pCommand*, int, QProcess::ExitStatus );
	void commandReadyRead( pCommand*, const QByteArray& );
	void commandStarted( pCommand* );
	void commandStateChanged( pCommand*, QProcess::ProcessState );

};

#endif // PCONSOLEMANAGER_H
