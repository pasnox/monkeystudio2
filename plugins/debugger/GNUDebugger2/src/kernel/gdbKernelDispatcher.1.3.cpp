/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbKernelDispatcher
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/


#include "gdbKernelDispatcher.1.3.h"


GdbKernelDispatcher::GdbKernelDispatcher(QObject * parent ) : QObject(parent)
{
	mParser = GdbParser::instance();
	addonList.clear();
}

GdbKernelDispatcher::~GdbKernelDispatcher()
{
}

void GdbKernelDispatcher::add(/*const  QPointer<class GdbParser> & a, */const QPointer<class GdbCore> & p)
{
	// add on to the dispatcher 
	addonList << p;
	connect(mParser, SIGNAL(onInterpreter(const QPointer<BaseInterpreter> & ,const int & , const QString &)), p , 
		SLOT(interpreter(const QPointer<BaseInterpreter> & , const int & , const QString &)));
}
/*
void GdbKernelDispatcher::remove(const QPointer<GdbCore> & p)
{
}
*/
void GdbKernelDispatcher::removeAll()
{
	addonList.clear();
}

void GdbKernelDispatcher::gdbStarted()
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled())		p->gdbStarted();
}

void GdbKernelDispatcher::gdbFinished()
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled())		p->gdbFinished();
}

void GdbKernelDispatcher::gdbError()
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled())		p->gdbError();
}

void GdbKernelDispatcher::targetLoaded(const int & id , const QString & s)
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled()) p->targetLoaded(id, s);
}

void GdbKernelDispatcher::targetNoLoaded(const int & id , const QString & s)
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled()) p->targetNoLoaded(id, s);
}

void GdbKernelDispatcher::targetRunning(const int & id , const QString & s)
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled()) p->targetRunning(id, s);
}

void GdbKernelDispatcher::targetStopped(const int & id , const QString & s)
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled()) p->targetStopped(id, s);
}

void GdbKernelDispatcher::targetExited(const int & id , const QString & s)
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled()) p->targetExited(id, s);
}

void GdbKernelDispatcher::error(const int & id, const QString & s)
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled())		p->error(id, s);
}

void GdbKernelDispatcher::done(const int & id, const QString & s)
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled())		p->done(id, s);}

void GdbKernelDispatcher::info(const int & id, const QString & s)
{
	foreach(QPointer<GdbCore> p , addonList)
		if(p->isEnabled())		p->info(id, s);
}

