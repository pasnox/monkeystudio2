/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbCli
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbCli class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBCLI_H
#define GDBCLI_H

#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"
#include "./ui/UIGdbCli.h"

class GdbCli : public GdbCore
{
	Q_OBJECT


public:

	GdbCli(QObject * parent = 0);
	~GdbCli();

public slots:

	void onSend();

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
	void prompt(const int &, const QString &);

private:

	QPointer<UIGdbCli> mWidget;
};

#endif
