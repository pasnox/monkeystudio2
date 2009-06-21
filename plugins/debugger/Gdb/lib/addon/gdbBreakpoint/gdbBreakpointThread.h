#ifndef GDBBREAKPOINTTHREAD_H
#define GDBBREAKPOINTTHREAD_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QMutex>

class GdbBreakpoint;

class GdbBreakpointThread : public QThread
{
	 Q_OBJECT
public:
	GdbBreakpointThread(GdbBreakpoint*);
	~GdbBreakpointThread();
	void restoreBreakpoint(QStringList);
	void run();
	void next();
	bool mStop;

private:
	GdbBreakpoint *parent;
	QStringList mBreakpointPending;
};
#endif