//==========================================

/*
	Class GdbControl
*/

#include "./addon/gdbControl.h"

#include <QFileDialog>

GdbControl::GdbControl(QWidget *o) :  GdbBase(o), bJustAdd(0), bTargetLoaded(0), bTargetRunning(0), bGdbStarted(0)
{
	mWidget = new QFrame(this);
	QVBoxLayout *h = new QVBoxLayout(mWidget);
	
	bStepOver = new QPushButton("step over");
	bRun = new QPushButton("run");
	bStepInto = new QPushButton("step into");
	bContinue = new QPushButton("Continue");
	bStop = new QPushButton("Stop");
	bLoadTarget = new QPushButton("load target");
	
	bRun->setEnabled(false);
	bStepOver->setEnabled(false);
	bStepInto->setEnabled(false);
	bContinue->setEnabled(false);
	bStop->setEnabled(false);

	h->addWidget(bLoadTarget);
	h->addWidget(bRun);
	h->addWidget(bContinue);
	h->addWidget(bStepOver);
	h->addWidget(bStepInto);
	h->addWidget(bStop);

	connect (bLoadTarget, SIGNAL(clicked()), this, SLOT(onLoadTarget()));
	connect (bStepOver, SIGNAL(clicked()), this, SLOT(onStepOver()));
	connect (bStepInto, SIGNAL(clicked()), this, SLOT(onStepInto()));
	connect (bRun, SIGNAL(clicked()), this, SLOT(onRun()));
	connect (bContinue, SIGNAL(clicked()), this, SLOT(onContinue()));
	connect (bStop, SIGNAL(clicked()), this, SLOT(onStop()));

	cmd.setClass(this);

} 
//
GdbControl::~GdbControl()
{
} 
//
QString GdbControl::name()
{
	 return "GdbControl"; 
}
//
QWidget * GdbControl::widget()
{
	return (QWidget*) mWidget ; 
}
//
void GdbControl::setupDockWidget(QMainWindow *mw)
{
mw = mw;
	
	setWidget(widget());
	setWindowTitle(name());

	setMaximumWidth(150);
	setFeatures (QDockWidget::NoDockWidgetFeatures);
//	mw->addDockWidget(Qt::LeftDockWidgetArea, this);
}

//
int GdbControl::process(int id,QByteArray data)
{
id = id;
	if(!bGdbStarted || bTargetRunning || !bTargetLoaded) return PROCESS_TERMINED;


	//	start process if event == end-stepping-range or event == breakpoint-hit)
	if(cmd.startProcess(data))
	{
		bStepOver->setEnabled(true);
		bStepInto->setEnabled(true);
		bContinue->setEnabled(true);
		bStop->setEnabled(true);

		cmd.disconnectEventToProcess(interpreterEndSteppingRange);
		cmd.leaveEventToProcess(&interpreterEndSteppingRange);
	}

	return PROCESS_TERMINED;
}
//
int GdbControl::processError(int id, QByteArray data)
{
	// TODO
	id=id;	
	data = data;
 
	return PROCESS_TERMINED;
}
//
void GdbControl::processExit()
{
}
//
void GdbControl::onLoadTarget()
{
	QFileDialog *d = new QFileDialog(this,"Select target");
	 if (d->exec())
	 {
     QStringList fileNames = d->selectedFiles(); 
		QString file = fileNames.at(0);
		 file.replace('\\','/');
		emit sendRawData(this,"file " + file.toLocal8Bit());
	 }
}


void GdbControl::onStepOver()
{
	bRun->setEnabled(false);

	bStepOver->setEnabled(false);
	bStepInto->setEnabled(false);
	bContinue->setEnabled(false);
	bStop->setEnabled(false);

	interpreterEndSteppingRange = new QGdbInterpreter("end-stepping-range",
	"n",
	QRegExp("^n"), 
	QRegExp("^\\d+\\s+.*"),
	"");
	
	cmd.connectEventToProcess(interpreterEndSteppingRange, NULL);
	cmd.forceProcess();
}
//
void GdbControl::onStepInto()
{
	bRun->setEnabled(false);

	bStepOver->setEnabled(false);
	bStepInto->setEnabled(false);
	bContinue->setEnabled(false);
	bStop->setEnabled(false);

	interpreterEndSteppingRange = new QGdbInterpreter("end-stepping-range",
	"s",
	QRegExp("^s"), 
	QRegExp("^\\d+\\s+.*"),
	"");
	
	cmd.connectEventToProcess(interpreterEndSteppingRange, NULL);
	cmd.forceProcess();
}
//
void GdbControl::onRun()
{
	bRun->setEnabled(false);
	emit sendRawData(this,"r");
}
//
void GdbControl::onContinue()
{
	emit sendRawData(this,"c");
}
//
void GdbControl::onStop()
{
	sendRawData(this,"kill");

	bRun->setEnabled(true);
	bStop->setEnabled(false);
	bStepOver->setEnabled(false);
	bStepInto->setEnabled(false);
	bContinue->setEnabled(false);
}
//
void GdbControl::gdbStarted()
{
	bGdbStarted = true;
}
//
void GdbControl::gdbFinished()
{
	bGdbStarted = false;
	bTargetLoaded = false;
}
//
void GdbControl::targetLoaded()
{
	bTargetLoaded = true;
	bRun->setEnabled(true);
}
//
void GdbControl::targetRunning()
{
	bTargetRunning = true;

	bRun->setEnabled(false);
	bStepOver->setEnabled(false);
	bStepInto->setEnabled(false);
	bContinue->setEnabled(false);
	bStop->setEnabled(false);

}
//
void GdbControl::targetStopped()
{
	bTargetRunning = false;

	bStepOver->setEnabled(true);
	bStepInto->setEnabled(true);
	bContinue->setEnabled(true);
	bStop->setEnabled(true);
}
//
void GdbControl::targetExited()
{
	bTargetRunning = false;

	bRun->setEnabled(true);
	bStepOver->setEnabled(false);
	bStepInto->setEnabled(false);
	bContinue->setEnabled(false);
	bStop->setEnabled(false);
}
//
