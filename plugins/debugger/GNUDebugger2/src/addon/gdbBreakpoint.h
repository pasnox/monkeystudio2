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
#include "../kernel/gdbCore.h"


	typedef struct BaseBreakpoint
	{
		int line;
		int index;
		int type;
		int enable;
	};


	typedef struct Breakpoint
	{
		QString fileName;
		QList<BaseBreakpoint> bp;
	};



class GdbBreakpoint : public GdbCore
{
	Q_OBJECT


public:

	GdbBreakpoint(QObject * parent = 0, QPointer<GdbParser> = 0, 	QPointer<GdbProcess> = 0);
	~GdbBreakpoint();

public slots:


	void onBreakpointAdd( int , QString );
	void onBreakpointDelete( int , QString );
	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	void toggleBreakpoint(const QString &, const int &);
	void onRequestBreakpoint(const QString & );
	
	// gdb
	void gdbExited();
	void gdbStarted();
	// target
	void targetRunning(const int &, const QString &);
	void targetStopped(const int &, const QString &);

	QString name();
	QPointer<QWidget> widget();

private:

	GdbConnectTemplate<GdbBreakpoint> *Connect;
	QPointer<BaseInterpreter> interpreterAddBreakpoint;
	QPointer<BaseInterpreter> interpreterDelBreakpoint;
	QList<Breakpoint *>  breakpointList;

	Breakpoint * findByName(const QString &);
	int asBreakpointAtLine(Breakpoint *, int);

	void removeBreakpoint(Breakpoint * bp);
	void removeAllBreakpoint();

	void breakpointMoved(const QString & , const int & , const int & );

	QPointer<QTextEdit> mWidget;

signals:

	void onToggleBreakpoint(const QString &, const int &, const bool );
};

#endif
