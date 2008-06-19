/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbRegister
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbResgiter class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBREGISTER_H
#define GDBREGISTER_H

#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"
#include "./ui/UIGdbRegister.h"

class GdbRegister : public GdbCore
{
	Q_OBJECT


public:

	GdbRegister(QObject * parent = 0);
	~GdbRegister();

public slots:

	void onRegister( int , QString );

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

	GdbConnectTemplate<GdbRegister> *Connect;
	QPointer<BaseInterpreter> interpreterRegister;

	QPointer<GdbSequencer> Sequencer;
	QPointer<UIGdbRegister> mWidget;

	int numRegister;

	void showColor(QTreeWidgetItem *, QString );
};

#endif
