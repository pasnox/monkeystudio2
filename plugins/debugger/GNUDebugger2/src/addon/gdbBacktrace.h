/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbBacktrace
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbBacktrace class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBBACKTRACE_H
#define GDBBACKTRACE_H

#include <QObject>
#include <QTextEdit>
#include "../kernel/gdbCore.h"



class GdbBacktrace : public GdbCore
{
	Q_OBJECT


public:

	GdbBacktrace(QObject * parent = 0, QPointer<GdbParser> = 0, 	QPointer<GdbProcess> = 0);
	~GdbBacktrace();

public slots:

	void onBacktrace( int , QString );
	void onInfoSource( int , QString );
	void onRequestBacktrace();

	
	// gdb
	void gdbExited();
	void gdbStarted();
	// target
	void targetRunning(const int &, const QString &);
	void targetStopped(const int &, const QString &);
	void targetExited(const int & id, const QString & s);

	QString name();
	QPointer<QWidget> widget();

	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

private:

	GdbConnectTemplate<GdbBacktrace> *Connect;
	QPointer<BaseInterpreter> interpreterBacktrace;
	QPointer<BaseInterpreter> interpreterInfoSource;


	QPointer<QTextEdit> mWidget;

	int mCurrentLine;
	QString mCurrentFile;

signals:

	void onToggleBacktrace(const QString &, const int &);
};

#endif
