/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbBreakpoint
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbBreakpoint class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBBREAKPOINT_H
#define GDBBREAKPOINT_H

#include <QObject>
#include <QTextEdit>
#include "../../kernel/gdbCore.1.3.h"
#include "./ui/UIGdbBreakpoint.h"

#include "gdbBreakpointStruct.h"



class GdbBreakpoint : public GdbCore
{
	Q_OBJECT


public:

	GdbBreakpoint(QObject * parent = 0 );
	~GdbBreakpoint();

public slots:

	void onBreakpointEnabled(int, QString);
	void onBreakpointDisabled(int, QString);
	void toggleEnabledBreakpoint(const QString & ,const int & , const bool &);

	void onBreakpointAdd( int , QString );
	void onBreakpointDelete( int , QString );
	void toggleBreakpoint(const QString &, const int &);

	void onBreakpointConditionned(int, QString);
	void onBreakpointUnConditionned(int, QString);
	void toggleConditionnedBreakpoint(const QString &, const int &, const QString &);

	void onRequestBreakpoint(const QString & );
	void onBreakpointPending(int, QString );
	
	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	QString name();
	QPointer<QWidget> widget();
	QIcon icon();

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

	GdbConnectTemplate<GdbBreakpoint> *Connect;
	QList<Breakpoint *>  breakpointList;

	QPointer<BaseInterpreter> interpreterAddBreakpoint;
	QPointer<BaseInterpreter> interpreterDelBreakpoint;

	QPointer<BaseInterpreter> interpreterEnabledBreakpoint;
	QPointer<BaseInterpreter> interpreterDisabledBreakpoint;

	QPointer<BaseInterpreter> interpreterBreakpointPending;

	QPointer<BaseInterpreter> interpreterConditionnedBreakpoint;
	QPointer<BaseInterpreter> interpreterUnConditionnedBreakpoint;

	Breakpoint * findByName(const QString &);
	int asBreakpointAtLine(Breakpoint *, int);
	int asBreakpointIndex(Breakpoint *b, int index);

	void removeBreakpoint(Breakpoint * bp);
	void removeAllBreakpoint();

	void breakpointMoved(const QString & , const int & , const int & );
	void desableBreakpointHit();

	QPointer<UIGdbBreakpoint> mWidget;

signals:

	void onToggleBreakpoint(const Breakpoint & , const BaseBreakpoint & , const bool &);
//	void onToggleBreakpointEnabled(const Breakpoint & , const BaseBreakpoint &);
//	void onToggleBreakpointConditionned(const Breakpoint & , const BaseBreakpoint &);
};

#endif
