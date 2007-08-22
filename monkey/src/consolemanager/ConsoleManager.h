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
#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QThread>
#include <QApplication>
#include <QProcess>

class QTimer;

class ConsoleManager : public QThread, public QSingleton<ConsoleManager>
{
	Q_OBJECT
	friend class QSingleton<ConsoleManager>;

public:
	ConsoleManager( QObject* = QApplication::instance() );
	~ConsoleManager();

protected:
	QProcess* mProcess;
	virtual void run();

private slots:
	void error( QProcess::ProcessError );
	void finished( int, QProcess::ExitStatus );
	void readyRead();
	void started();
	void stateChanged( QProcess::ProcessState );

signals:
	void error( QProcess::ProcessError, const QString& );
	void finished( int, QProcess::ExitStatus, const QString& );
	void readyRead( const QString& );
	void started( const QString& );
	void stateChanged( QProcess::ProcessState, const QString& );

};

#endif // CONSOLEMANAGER_H
