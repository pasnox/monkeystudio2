//==========================================

/*
	Class GdbAnswer
*/

#include "./addon/gdbAnswer.h"


GdbAnswer::GdbAnswer(GdbParser *o) :  GdbCore(o),
	interpreterNo(NULL), interpreterYes(NULL)
{
	mWidget = new QTextEdit();
	mWidget->setEnabled(false);

	getContainer()->setWidget(mWidget);
	getContainer()->setWindowTitle(name());

	cmd.setClass(this);

	start();
} 
//
GdbAnswer::~GdbAnswer()
{
	delete mWidget;
	delete getContainer();
} 
//
QString GdbAnswer::name()
{
	 return "GdbAnswer"; 
}
//
void GdbAnswer::gdbStarted()
{
	GdbCore::gdbStarted();
}
//
void GdbAnswer::gdbFinished()
{
	GdbCore::gdbFinished();
	mWidget->setEnabled(false);
}
//
void GdbAnswer::targetLoaded()
{
	GdbCore::targetLoaded();
}
//
void GdbAnswer::targetRunning()
{
	GdbCore::targetRunning();
	mWidget->setEnabled(false);
}
//
void GdbAnswer::targetStopped()
{
	GdbCore::targetStopped();
	mWidget->setEnabled(true);
}
//
void GdbAnswer::targetExited()
{
	GdbCore::targetExited();
	mWidget->setEnabled(false);
}
//
int GdbAnswer::process(QGdbMessageCore m)
{

	QString answer = getParametre("answerGdb=", m.msg);

	if(answer.endsWith("(y or n) "))
	{ 
		int ret = QMessageBox::warning(NULL, "Question from Gdb", answer, QMessageBox::Ok | QMessageBox::No); 
		if(ret == QMessageBox::Ok)
		{
			mWidget->append("User answer : Yes"); 
			interpreterYes = new QGdbInterpreter("GdbYes",
			"y",
			QRegExp("^y"),
			QRegExp(".*"),
			"currentQuestion=\"" + getParametre("answerGdb=",m.msg) + "\"");

			cmd.connectEventInterpreter(interpreterYes, &GdbAnswer::processYes);
			cmd.forceProcess();
		}
		else
		{
			mWidget->append("User answer : No"); 
			interpreterNo = new QGdbInterpreter("GdbNo",
			"n",
			QRegExp("^n"),
			QRegExp(".*"),
			"currentQuestion=\"" + getParametre("answerGdb=",m.msg) + "\"");

			cmd.connectEventInterpreter(interpreterNo, &GdbAnswer::processNo);
			cmd.forceProcess();
		}
		return PROCESS_TERMINED;
	}

	return cmd.dispatchProcess(m);
}
//
int GdbAnswer::processError( QGdbMessageCore m)
{
	// TODO
 	m=m;
	return PROCESS_TERMINED;

}
//
int GdbAnswer::processNo(QGdbMessageCore m)
{
	// TODO
	mWidget->append(m.msg); 

 	cmd.disconnectEventInterpreter(interpreterNo);
	cmd.leaveEventInterpreter(&interpreterNo);
 
	processPostMessage(-1,
		 "sender=\"" + name().toLocal8Bit() + "\",event=\"notify-question\",currentQuestion=\"" + getParametre("answerGdb=", m.msg) + "\",userAnswer=\"No\",answerGdb=\"" + m.msg + "\"");

	return PROCESS_TERMINED;
}
//
int GdbAnswer::processYes(QGdbMessageCore m)
{
	// TODO
	mWidget->append(m.msg); 

 	cmd.disconnectEventInterpreter(interpreterYes);
	cmd.leaveEventInterpreter(&interpreterYes);
 
	processPostMessage(-1,
		 "sender=\"" + name().toLocal8Bit() + "\",event=\"notify-question\",currentQuestion=\"" + getParametre("currentQuestion=",getParametre("answerExtention=", m.msg)) + "\",userAnswer=\"Yes\",answerGdb=\"" + getParametre("answerGdb=",m.msg) + "\"");

	return PROCESS_TERMINED;
}
//
void GdbAnswer::processPrompt()
{
}
//

