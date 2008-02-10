//==========================================

/*
	Class GdbControl
*/

#include "./addon/gdbControl.h"

#include <QFileDialog>

GdbControl::GdbControl(GdbParser *p) :  GdbCore( p),
	interpreterEndSteppingRange(NULL)
{
	mWidget = new QFrame();
	QVBoxLayout *h = new QVBoxLayout(mWidget);
	
	bStepOver = new QPushButton("step over");
	bRun = new QPushButton("run");
	bStepInto = new QPushButton("step into");
	bContinue = new QPushButton("Continue");
	bStop = new QPushButton("Stop");
	bLoadTarget = new QPushButton("load target");
	bExitGdb = new QPushButton("Exit");
	
	bRun->setEnabled(false);
	bStepOver->setEnabled(false);
	bStepInto->setEnabled(false);
	bContinue->setEnabled(false);
	bStop->setEnabled(false);
	bExitGdb->setEnabled(false);

	h->setSpacing(0);
	h->addWidget(bLoadTarget);
	h->addWidget(bRun);
	h->addWidget(bContinue);
	h->addWidget(bStepOver);
	h->addWidget(bStepInto);
	h->addWidget(bStop);
	h->addWidget(bExitGdb);

	getContainer()->setWidget(mWidget);
	getContainer()->setWindowTitle(name());

	connect (bLoadTarget, SIGNAL(clicked()), this, SLOT(onLoadTarget()));
	connect (bStepOver, SIGNAL(clicked()), this, SLOT(onStepOver()));
	connect (bStepInto, SIGNAL(clicked()), this, SLOT(onStepInto()));
	connect (bRun, SIGNAL(clicked()), this, SLOT(onRun()));
	connect (bContinue, SIGNAL(clicked()), this, SLOT(onContinue()));
	connect (bStop, SIGNAL(clicked()), this, SLOT(onStop()));
	connect (bExitGdb, SIGNAL(clicked()), this,SLOT(onExit()));

	cmd.setClass(this);
	
	cmd.connectEventNotify("notify-question", &GdbControl::processQuestion);
} 
//
GdbControl::~GdbControl()
{
	delete getContainer();
} 
//
QString GdbControl::name()
{
	 return "GdbControl"; 
}
//
int GdbControl::process(QGdbMessageCore m)
{
	return cmd.dispatchProcess(m);
}
//
int GdbControl::processError(QGdbMessageCore)
{
	// TODO
 
	return PROCESS_TERMINED;
}
//
void GdbControl::processExit()
{
}
//
void GdbControl::onLoadTarget()
{
	QFileDialog *d = new QFileDialog(NULL,"Select target");
	 if (d->exec())
	 {
		QStringList fileNames = d->selectedFiles(); 
		QString file = fileNames.at(0);
		file.replace('\\','/');
		emit wantStart(file);

//		emit sendRawData(this,"file " + file.toLocal8Bit());
	 }
}
// step over or into
int GdbControl::processSteps(QGdbMessageCore m)
{
m=m;
		bRun->setEnabled(true);
		bStepOver->setEnabled(true);
		bStepInto->setEnabled(true);
		bContinue->setEnabled(true);
		bStop->setEnabled(true);
		bExitGdb->setEnabled(true);

		cmd.disconnectEventInterpreter(interpreterEndSteppingRange);
		cmd.leaveEventInterpreter(&interpreterEndSteppingRange);
		return PROCESS_TERMINED;
}
//
int GdbControl::processQuestion(QGdbMessageCore m)
{
	QByteArray currentQuestion = getParametre("currentQuestion=", m.msg);

	// questions pris en charges
	QRegExp kill("^Kill the program being debugged\\? \\(y or n\\) $");
	QRegExp restart("^Start it from the beginning\\? \\(y or n\\) $");

	if(kill.exactMatch( currentQuestion))
	{
		bRun->setEnabled(true);
		bStop->setEnabled(false);
		bStepOver->setEnabled(false);
		bStepInto->setEnabled(false);
		bContinue->setEnabled(false);
		bExitGdb->setEnabled(false);
}

	if(restart.exactMatch( currentQuestion))
	{
		bRun->setEnabled(false);
		bStop->setEnabled(false);
		bStepOver->setEnabled(false);
		bStepInto->setEnabled(false);
		bContinue->setEnabled(false);
		bExitGdb->setEnabled(false);
	}

	return PROCESS_TERMINED;
}
//
void GdbControl::onStepOver()
{
	bRun->setEnabled(false);

	bStepOver->setEnabled(false);
	bStepInto->setEnabled(false);
	bContinue->setEnabled(false);
	bStop->setEnabled(false);
	bExitGdb->setEnabled(false);

	interpreterEndSteppingRange = new QGdbInterpreter("end-stepping-range",
	"n",
	QRegExp("^n"), 
	QRegExp("^\\d+\\s+.*"),
	"");
	
	cmd.connectEventInterpreter(interpreterEndSteppingRange, &GdbControl::processSteps);
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
	bExitGdb->setEnabled(false);

	interpreterEndSteppingRange = new QGdbInterpreter("end-stepping-range",
	"s",
	QRegExp("^s"), 
	QRegExp("^\\d+\\s+.*"),
	"");
	cmd.connectEventInterpreter(interpreterEndSteppingRange, &GdbControl::processSteps);
	cmd.forceProcess();
}
//
void GdbControl::onRun()
{
//	bRun->setEnabled(false);
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
	emit sendRawData(this,"kill");
}
//
void GdbControl::onExit()
{
	emit wantExit();
}
//
void GdbControl::gdbStarted()
{
	GdbCore::gdbStarted();
	bExitGdb->setEnabled(true);
}
//
void GdbControl::gdbFinished()
{
	GdbCore::gdbFinished();

	bLoadTarget->setEnabled(true);
	bExitGdb->setEnabled(false);
	bRun->setEnabled(false);
}
//
void GdbControl::targetLoaded()
{
	GdbCore::targetLoaded();
	bRun->setEnabled(true);
	bExitGdb->setEnabled(true);
	bLoadTarget->setEnabled(false);
}
//
void GdbControl::targetRunning()
{
	GdbCore::targetRunning();

	bRun->setEnabled(false);
	bStepOver->setEnabled(false);
	bStepInto->setEnabled(false);
	bContinue->setEnabled(false);
	bStop->setEnabled(false);
	bExitGdb->setEnabled(false);

}
//
void GdbControl::targetStopped()
{
	GdbCore::targetStopped();

	bRun->setEnabled(true);
	bStepOver->setEnabled(true);
	bStepInto->setEnabled(true);
	bContinue->setEnabled(true);
	bStop->setEnabled(true);
	bExitGdb->setEnabled(true);
}
//
void GdbControl::targetExited()
{
	GdbCore::targetExited();

	bRun->setEnabled(true);
	bStepOver->setEnabled(false);
	bStepInto->setEnabled(false);
	bContinue->setEnabled(false);
	bStop->setEnabled(false);
	bExitGdb->setEnabled(true);
}
//
