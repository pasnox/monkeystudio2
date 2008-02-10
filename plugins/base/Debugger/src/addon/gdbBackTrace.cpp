//==========================================

/*
	Class GdbBackTrace
*/

#include "./addon/gdbBackTrace.h"

#include <QFileInfo>
#include <QTextCursor>

GdbBackTrace::GdbBackTrace(GdbParser *p) :  GdbCore(p)
{
	qRegisterMetaType<QTextCursor>( "QTextCursor" );

	mWidget = new QTextEdit();
	mWidget->setEnabled(false);

	getContainer()->setWidget(mWidget);
	getContainer()->setWindowTitle(name());

	cmd.setClass(this);
	cmd.connectEventStart("breakpoint-hit" , NULL);
	cmd.connectEventStart("end-stepping-range" , NULL);

	/*
		Backtrace line splitted in two lines
			"#0  UIForm (this=0x80dc268, parent=0x0) "
			"  at src/ui/UIForm.cpp:43"
	*/
	getParser()->addRestorLine(QRegExp("^#0\\s.*$") , QRegExp("^[^#].*\\sat\\s.*:\\d+$"));

	interpreterBackTrace = new QGdbInterpreter("back-trace",
			"bt",
			QRegExp("^bt"),
			QRegExp("^#0\\s.*"),
			"");

	cmd.connectEventInterpreter(interpreterBackTrace,  &GdbBackTrace::processBacktrace);


	interpreterInfoSource = new QGdbInterpreter("info-source",
			"info source",
			QRegExp("^info source"),
			QRegExp("^Located\\sin\\s.*"),
			"");

	cmd.connectEventInterpreter(interpreterInfoSource,  &GdbBackTrace::processInfosource);
} 
//
GdbBackTrace::~GdbBackTrace()
{
		delete getContainer();
} 
//
QString GdbBackTrace::name()
{
	 return "GdbBackTrace"; 
}
//
void GdbBackTrace::gdbStarted()
{
	GdbCore::gdbStarted();
}
//
void GdbBackTrace::gdbFinished()
{
	GdbCore::gdbFinished();
	mWidget->setEnabled(false);
}
//
void GdbBackTrace::targetLoaded()
{
	GdbCore::targetLoaded();
}
//
void GdbBackTrace::targetRunning()
{
	GdbCore::targetRunning();
	mWidget->setEnabled(false);
}
//
void GdbBackTrace::targetStopped()
{
	GdbCore::targetStopped();
	mWidget->setEnabled(true);
}
//
void GdbBackTrace::targetExited()
{
	GdbCore::targetExited();
	mWidget->setEnabled(false);
}
//
int GdbBackTrace::process(QGdbMessageCore m )
{
	return cmd.dispatchProcess(m);
}
//
int GdbBackTrace::processError(QGdbMessageCore m)
{
	// TODO
 
	mWidget->append(getParametre("answerGdb=", m.msg));

	return PROCESS_TERMINED;

}
//
void GdbBackTrace::processExit()
{
}
//
int GdbBackTrace::processBacktrace(QGdbMessageCore m)
{

		QByteArray value = getParametre("answerGdb=", m.msg);

		QRegExp exp("^#0.*\\sat\\s([^:]+):(\\d+)$" );
		if(exp.exactMatch( value))
		{
			QStringList list = exp.capturedTexts();

			line = list.at(2);
		
			mWidget->append("(dispatching) Current file line : " + line);
		}
		return PROCESS_TERMINED;
}
//
int GdbBackTrace::processInfosource(QGdbMessageCore m)
{

	QByteArray value = getParametre("answerGdb=", m.msg);

	QRegExp exp("Located in (.*)" );
	if(exp.exactMatch( value))
	{
		QStringList list = exp.capturedTexts();
		fileName = list.at(1);

		mWidget->append("(dispatching) Current file : " + fileName);
		// notify other plugin 

		processPostMessage(0, "sender=\"" + name().toLocal8Bit() + "\",event=\"notify-back-trace\",fileName=\"" + fileName.toLocal8Bit() + "\",line=\"" + line.toLocal8Bit() + "\"");

		return PROCESS_TERMINED; 
	}
	return PROCESS_WAITING;
}
//
