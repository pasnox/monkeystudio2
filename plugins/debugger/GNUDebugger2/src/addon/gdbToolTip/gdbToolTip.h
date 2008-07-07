/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbToolTip
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbToolTip class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBTOOLTIP_H
#define GDBTOOLTIP_H

#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"


class GdbToolTip : public GdbCore
{
	Q_OBJECT


public:

	GdbToolTip(QObject * parent = 0);
	~GdbToolTip();

public slots:

	void onValue( int , QString );
	void onRequestShowVar(const QString &);

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

	GdbConnectTemplate<GdbToolTip> Connect;
	QPointer<BaseInterpreter> interpreterValue;

	QPointer<GdbSequencer> Sequencer;

	bool enteredShowVar;
	bool isGdbStarted;
	bool isTargetStopped;
};

#endif
