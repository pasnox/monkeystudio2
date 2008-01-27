//==========================================

/*
	Class GdbControl
*/

#include "gdbRegisters.h"


GdbRegisters::GdbRegisters(QWidget *o) :  GdbBase(o), bJustAdd(0), bTargetLoaded(0), bTargetRunning(0), bGdbStarted(0), enteringBlock(0)
{
	mWidget = new QTextEdit(this);
	mWidget->setEnabled(false);

	cmd.setClass(this);

	interpreterRegisters = new QGdbInterpreter("info-registers",
		"info register",
		QRegExp("^info register"),
		QRegExp("^\\w+\\s+\\w+\\s+.*"),
			"");

	cmd.connectEventToProcess(interpreterRegisters,/* "info-registers",*/ &GdbRegisters::processRegisters);
} 
//
GdbRegisters::~GdbRegisters()
{
} 
//
QString GdbRegisters::name()
{
	 return "GdbRegisters"; 
}
//
QWidget * GdbRegisters::widget()
{
	return (QWidget*) mWidget ; 
}
//
void GdbRegisters::gdbStarted()
{
	bGdbStarted = true;
}
//
void GdbRegisters::gdbFinished()
{
	bGdbStarted = false;
	bTargetLoaded = false;
	mWidget->setEnabled(false);
}
//
void GdbRegisters::targetLoaded()
{
	bTargetLoaded = true;
}
//
void GdbRegisters::targetRunning()
{
	mWidget->setEnabled(false);
	bTargetRunning = true;
}
//
void GdbRegisters::targetStopped()
{
	mWidget->setEnabled(true);
	bTargetRunning = false;
}
//
void GdbRegisters::targetExited()
{
	mWidget->setEnabled(false);
	bTargetRunning = false;
}
//
void GdbRegisters::setupDockWidget(QMainWindow *mw)
{
	mw = mw;
	setWidget(widget());
	setWindowTitle(name());
	setAllowedAreas(Qt::AllDockWidgetAreas);
	setFeatures (QDockWidget::DockWidgetMovable |QDockWidget::DockWidgetFloatable);
}
//
int GdbRegisters::process(int id,QByteArray data)
{

	if(!bGdbStarted || bTargetRunning || !bTargetLoaded) return PROCESS_TERMINED;

	return cmd.dispatchProcess(id ,data);
}
//
int GdbRegisters::processError(int id, QByteArray data)
{
	// TODO
	id = id ;
	data = data;
 
	return PROCESS_TERMINED;
}
//
void GdbRegisters::processExit()
{
}
//
int GdbRegisters::processRegisters(int id, QByteArray data)
{
id = id;
	QByteArray event = getParametre("event=", data);
	// TODO
	enteringBlock = true;
	
	QByteArray value = getParametre("answerGdb=", data);

	QRegExp exp("^(\\w+)\\s+(\\w+)\\s+(.*)");
	if(exp.exactMatch( value))
	{
		QStringList list = exp.capturedTexts();
		mWidget->append(list.at(1).toLocal8Bit()  + "\t" + list.at(2).toLocal8Bit() + "\t\t" + list.at(3).toLocal8Bit());
	}

	// auto stopping when promt found

	return PROCESS_WAITING;
}
//
