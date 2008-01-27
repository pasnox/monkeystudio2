//==========================================

/*
	Class GdbControl
*/

#include "./addon/gdbBackTrace.h"

#include <QFileInfo>
//#include <QMessageBox>

GdbBackTrace::GdbBackTrace(QWidget *o) :  GdbBase(o), bJustAdd(0), bTargetLoaded(0), bTargetRunning(0), bGdbStarted(0)
{
	mWidget = new QTextEdit(this);
	mWidget->setEnabled(false);

	cmd.setClass(this);

	interpreterBackTrace = new QGdbInterpreter("back-trace",
			"bt",
			QRegExp("^bt"),
			QRegExp("^#0\\s.*"),
			"");

	cmd.connectEventToProcess(interpreterBackTrace,  &GdbBackTrace::processBacktrace);


	interpreterInfoSource = new QGdbInterpreter("info-source",
			"info source",
			QRegExp("^info source"),
			QRegExp("^Located\\sin\\s.*"),
			"");

	cmd.connectEventToProcess(interpreterInfoSource,  &GdbBackTrace::processInfosource);
} 
//
GdbBackTrace::~GdbBackTrace()
{
} 
//
QString GdbBackTrace::name()
{
	 return "GdbBackTrace"; 
}
//
QWidget * GdbBackTrace::widget()
{
	return (QWidget*) mWidget ; 
}
//
void GdbBackTrace::gdbStarted()
{
	bGdbStarted = true;
}
//
void GdbBackTrace::gdbFinished()
{
	bGdbStarted = false;
	bTargetLoaded = false;
	mWidget->setEnabled(false);
}
//
void GdbBackTrace::targetLoaded()
{
	bTargetLoaded = true;
}
//
void GdbBackTrace::targetRunning()
{
	bTargetRunning = true;
	mWidget->setEnabled(false);
}
//
void GdbBackTrace::targetStopped()
{
	bTargetRunning = false;
	mWidget->setEnabled(true);
}
//
void GdbBackTrace::targetExited()
{
	bTargetRunning = false;
	mWidget->setEnabled(false);
}
//
void GdbBackTrace::setupDockWidget(QMainWindow *mw)
{
	mw = mw;
	setWidget(widget());
	setWindowTitle(name());
	setFeatures (QDockWidget::DockWidgetMovable |QDockWidget::DockWidgetFloatable);
	setAllowedAreas(Qt::AllDockWidgetAreas);
}
//

int GdbBackTrace::process(int id,QByteArray data)
{
	if(!bGdbStarted || bTargetRunning || !bTargetLoaded) return PROCESS_TERMINED;

	return cmd.dispatchProcess(id , data);
}
//
int GdbBackTrace::processError(int id, QByteArray data)
{
	id = id;
	// TODO
 
	mWidget->append(getParametre("answerGdb=", data));
	buffer.clear();

	return PROCESS_TERMINED;

}
//
void GdbBackTrace::processExit()
{
}
//
int GdbBackTrace::processBacktrace(int id, QByteArray data)
{
id = id;
		QByteArray value = getParametre("answerGdb=", data);

		//#0  main (argc=1, argv=0xbf9b29c4) at src/main.cpp:20
		//#0  main (argc=Cannot access memory at address 0x0) at src/main.cpp:13
		//#0  QObject::connect(QObject const*, char const*, QObject const*, char const*, Qt::ConnectionType) (sender=0x22fde0, signal=0x45d938 "2lastWindowClosed()  receiver=0x22fde0, method=0x45d930 1quit(), type=AutoConnection)   at kernel/qobject.cpp:2482

		/*
		#0  QObject::connect(QObject const*, char const*, QObject const*, char const*, Qt::ConnectionType) (sender=0x22fde0, signal=0x45d938 "2lastWindowClosed()
		    receiver=0x22fde0, method=0x45d930 1quit(), type=AutoConnection)
		    at kernel/qobject.cpp:2482
		*/

		QRegExp exp(".*\\sat\\s([^:]+):(\\d+)$" );
		if(exp.exactMatch( buffer + value))
		{
			QStringList list = exp.capturedTexts();

			line = list.at(2);
		
			mWidget->append("(dispatching) Current file line : " + line);
			buffer.clear();

			/* restor interpreter if line is splited */
			QGdbInterpreter * tmp = interpreterBackTrace;
			
			interpreterBackTrace =  cmd.modifyEventToProcess(interpreterBackTrace , new QGdbInterpreter("back-trace",
				"bt",
				QRegExp("^bt"),
				QRegExp("^#0\\s.*"),
				""));
			
			cmd.leaveEventToProcess(&tmp);
			
			return PROCESS_TERMINED;
		}
		else
		{ 
			buffer = value;

			QGdbInterpreter * tmp = interpreterBackTrace;
			
			interpreterBackTrace = cmd.modifyEventToProcess(interpreterBackTrace,   new QGdbInterpreter("back-trace",
				"bt",
				QRegExp("^bt"),
				QRegExp(".*"),
				""));
			
			cmd.leaveEventToProcess(&tmp);
			
			return PROCESS_WAITING;
		}
}
//
int GdbBackTrace::processInfosource(int id, QByteArray data)
{
	id = id;
	QByteArray value = getParametre("answerGdb=", data);

	QRegExp exp("Located in (.*)" );
	if(exp.exactMatch( value))
	{
		QStringList list = exp.capturedTexts();
		fileName = list.at(1);

		mWidget->append("(dispatching) Current file : " + fileName);
		// notify other plugin 

		processPostMessage(0, "sender=\"" + name().toLocal8Bit() + "\",event=\"back-trace\",fileName=\"" + fileName.toLocal8Bit() + "\",line=\"" + line.toLocal8Bit() + "\"");

		return PROCESS_TERMINED; 
	}
	
	return PROCESS_WAITING;
}
//
