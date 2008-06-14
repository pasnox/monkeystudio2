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
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"
#include "./ui/UIGdbBacktrace.h"

class GdbBacktrace : public GdbCore
{
	Q_OBJECT


public:

	GdbBacktrace(QObject * parent = 0);
	~GdbBacktrace();

public slots:

	void onBacktrace( int , QString );
	void onInfoSource( int , QString );
	void onRequestBacktrace(const QString &);

	

	QString name();
	QPointer<QWidget> widget();
	QIcon icon();

	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	// gdb
	void gdbFinished();
	void gdbStarted();
	void gdbError();

	// target
	void targetLoaded(const int &, const QString &);
	void targetNoLoaded(const int &, const QString &);
	void targetRunning(const int &, const QString &);
	void targetStopped(const int &, const QString &);
	void targetExited(const int &, const QString &);

	// Parser
	void error(const int &, const QString &);
	void done(const int &, const QString &);
	void info(const int &, const QString &);

private:

	GdbConnectTemplate<GdbBacktrace> *Connect;
	QPointer<BaseInterpreter> interpreterBacktrace;
	QPointer<BaseInterpreter> interpreterInfoSource;

	QPointer<GdbSequencer> Sequencer;
	QPointer<UIGdbBacktrace> mWidget;

	int mCurrentLine;
	QString mCurrentFile;

signals:

	void onToggleBacktrace(const QString &, const int &);
};

#endif
