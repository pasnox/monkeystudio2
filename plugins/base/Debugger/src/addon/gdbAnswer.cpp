//==========================================

/*
	Class GdbAnswer
*/

#include "./addon/gdbAnswer.h"


GdbAnswer::GdbAnswer(QWidget *o) :  GdbBase(o),  bTargetLoaded(0), bTargetRunning(0), bGdbStarted(0)
{
	mWidget = new QTextEdit(this);
	mWidget->setEnabled(false);
} 
//
GdbAnswer::~GdbAnswer()
{
} 
//
QString GdbAnswer::name()
{
	 return "GdbAnswer"; 
}
//
QWidget * GdbAnswer::widget()
{
	return (QWidget*) mWidget ; 
}
//
void GdbAnswer::gdbStarted()
{
	bGdbStarted = true;
}
//
void GdbAnswer::gdbFinished()
{
	bGdbStarted = false;
	bTargetLoaded = false;
	mWidget->setEnabled(false);
}
//
void GdbAnswer::targetLoaded()
{
	bTargetLoaded = true;
}
//
void GdbAnswer::targetRunning()
{
	bTargetRunning = true;
	mWidget->setEnabled(false);
}
//
void GdbAnswer::targetStopped()
{
	bTargetRunning = false;
	mWidget->setEnabled(true);
}
//
void GdbAnswer::targetExited()
{
	bTargetRunning = false;
	mWidget->setEnabled(false);
}
//
void GdbAnswer::setupDockWidget(QMainWindow *mw)
{
	mw = mw;
	setWidget(widget());
	setWindowTitle(name());
	setFeatures (QDockWidget::DockWidgetMovable |QDockWidget::DockWidgetFloatable);
	setAllowedAreas(Qt::AllDockWidgetAreas);
}
//

int GdbAnswer::process(int id,QByteArray data)
{
	if(!bGdbStarted || bTargetRunning || !bTargetLoaded) return PROCESS_TERMINED;

	QString answer = getParametre("answerGdb=", data);

	if(answer.endsWith("(y or n) "))
	{ 
		int ret = QMessageBox::warning(NULL, "Question from Gdb", answer, QMessageBox::Ok | QMessageBox::No); 
		if(ret == QMessageBox::Ok)
			emit sendRawData(this,"y");
		else
			emit sendRawData(this,"n");

//		return PROCESS_SEND_DATA;
	}

	return PROCESS_TERMINED;
}
//
int GdbAnswer::processError(int id, QByteArray data)
{
	id = id;
	// TODO
 
	return PROCESS_TERMINED;

}
//
void GdbAnswer::processExit()
{
}
//

