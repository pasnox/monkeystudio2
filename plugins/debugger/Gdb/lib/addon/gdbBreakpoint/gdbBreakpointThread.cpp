


#include "gdbBreakpointThread.h"
#include "gdbBreakpoint.h"


 
GdbBreakpointThread::GdbBreakpointThread(GdbBreakpoint *p):QThread(p), mStop(true)
{
    parent = p;
}


GdbBreakpointThread::~GdbBreakpointThread()
{
}

void GdbBreakpointThread::restoreBreakpoint(QStringList l)
{
    mBreakpointPending = l;
}

void GdbBreakpointThread::next()
{
    mStop = false;
}

void GdbBreakpointThread::run()
{
    parent->mForce = true;
    foreach(QString s, mBreakpointPending)
    {
        mStop = true;
        QRegExp r("(.*):(\\d+)$");
        if(r.exactMatch(s))
        {
            QStringList t = r.capturedTexts();
//qDebug() << "toggle";
            parent->onToggleBreakpoint(t.at(1), t.at(2).toInt() -1);
//qDebug() << "waitting : " + QString::number(mStop);
            while(mStop){msleep(10);}
//qDebug() << "fin waitting";
        }
    }
    parent->mForce = false;
}


