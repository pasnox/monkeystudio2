

/*
	Class GdbKernelDispatcher
*/

#include "gdbKernelDispatcher.h"


GdbKernelDispatcher::GdbKernelDispatcher( GdbParser *p) : GdbCore(p)
{
	mWidget = new QTextEdit;

	start();
}
//
GdbKernelDispatcher::~GdbKernelDispatcher()
{
	delete mWidget;
}
//
QString GdbKernelDispatcher::name()
{
	return "GdbKernelDispatcher";
}
//
void GdbKernelDispatcher::add(GdbCore * p)
{
	listPlugin <<  p;
	connect(p, SIGNAL( sendRawData(GdbCore *, QByteArray)), this ,SLOT(onSendRawData( GdbCore * , QByteArray)));
	connect(p, SIGNAL( sendPostMessage(int, QByteArray)), this ,SLOT(onPostMessage( int , QByteArray)));
	mWidget->append("Pluging add  " + p->name() + " ... OK");
}
//
void GdbKernelDispatcher::stopAndDelete()
{
	int j = listPlugin.count();
	for(int i=0; i<j; i++)
	{
		listPlugin.at(0)->setStopProcess();
		listPlugin.at(0)->wait();
//		QMessageBox::warning(NULL,"delete", listPlugin.at(0)->name());
		delete listPlugin.at(0);
		listPlugin.removeAt(0);
	}
}
// process version 3
int GdbKernelDispatcher::process(QGdbMessageCore m)
{
	// posting message to all plugin
	for(int i=0; i<listPlugin.count(); i++)
	{
		if(listPlugin.at(i) != NULL) listPlugin.at(i)->postGdbMessage(m);
	}
	return 0;
}
//
int GdbKernelDispatcher::processError(QGdbMessageCore m)
{
	// TODO
	m=m;
	return PROCESS_TERMINED;
}
//
void GdbKernelDispatcher::processPrompt()
{
}
//
void GdbKernelDispatcher::onPostMessage( int id, QByteArray data)
{
	QGdbMessageCore m={data,id};

	for(int i=0; i < listPlugin.count(); i++)
		if(listPlugin.at(i) != NULL)  listPlugin.at(i)->postGdbMessage(m);
}
//
void GdbKernelDispatcher::onSendRawData( GdbCore *plug, QByteArray data)
{
	emit sendRawData(this, data);
}
//
void GdbKernelDispatcher::gdbStarted()
{
	for(int i=0; i < listPlugin.count(); i++)
		if(listPlugin.at(i) != NULL) listPlugin.at(i)->gdbStarted();
}
//
void GdbKernelDispatcher::gdbFinished()
{
	for(int i=0; i < listPlugin.count(); i++)
		if(listPlugin.at(i) != NULL) listPlugin.at(i)->gdbFinished();
}
//
void GdbKernelDispatcher::targetLoaded()
{
	for(int i=0; i < listPlugin.count(); i++)
		if(listPlugin.at(i) != NULL) listPlugin.at(i)->targetLoaded();
}
//
void GdbKernelDispatcher::targetRunning()
{
	for(int i=0; i < listPlugin.count(); i++)
		if(listPlugin.at(i) != NULL) listPlugin.at(i)->targetRunning();
}
//
void GdbKernelDispatcher::targetStopped()
{
	for(int i=0; i < listPlugin.count(); i++)
		if(listPlugin.at(i) != NULL) listPlugin.at(i)->targetStopped();
}
//
void GdbKernelDispatcher::targetExited()
{
	for(int i=0; i < listPlugin.count(); i++)
		if(listPlugin.at(i) != NULL) listPlugin.at(i)->targetExited();
}
