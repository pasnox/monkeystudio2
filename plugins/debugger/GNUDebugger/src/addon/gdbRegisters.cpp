//==========================================

/*
	Class GdbRegister
*/

#include "gdbRegisters.h"


GdbRegisters::GdbRegisters(GdbParser *p) :  GdbCore( p)
{
	mWidget = new QTableWidget;
	mWidget->setEnabled(false);
	mWidget->setColumnCount(2);
	mWidget->setHorizontalHeaderLabels(QStringList() << "Registers" << "Value" );
 	mWidget->setSelectionBehavior (QAbstractItemView::SelectRows);

//	getContainer()->setWidget(mWidget);
//	getContainer()->setWindowTitle(name());

	numberOfRegisters = 0;

	cmd.setClass(this);
	// start my interpreter when breakpoint-hit or end-stepping-range occur
	cmd.connectEventStart("breakpoint-hit" , NULL);
	cmd.connectEventStart("end-stepping-range" , NULL);

	// my interpreter
	interpreterRegisters = new QGdbInterpreter("info-registers",	// event name
		"info register",												// command for Gdb
		QRegExp("^info register"),									// command RegExp (for parser)
		QRegExp("^\\w+\\s+\\w+\\s+.*"),							// wait answer RegExp
			"");														// no answer extention

	// install interpreter. if event start occur, i execute my command and call processRegister()
	cmd.connectEventInterpreter( interpreterRegisters, &GdbRegisters::processRegisters);

//	connect(getContainer(), SIGNAL(  topLevelChanged ( bool) ), this, SLOT( onTopLevelChanged ( bool  )));

	start();
} 
//
void GdbRegisters::onTopLevelChanged ( bool b)
{
	// resize widget recommended
/*	if(b)
	{
		getContainer()->restoreGeometry(widgetSize);
	}
	else
	{
		widgetSize = getContainer()->saveGeometry();
	}
*/
}
//
GdbRegisters::~GdbRegisters()
{
	delete mWidget;
	delete interpreterRegisters;
//	delete getContainer();
} 
//
QString GdbRegisters::name()
{
	 return "GdbRegisters"; 
}
//
void GdbRegisters::gdbStarted()
{
	GdbCore::gdbStarted();
}
//
void GdbRegisters::gdbFinished()
{
	GdbCore::gdbFinished();
	mWidget->setEnabled(false);

	while(mWidget->rowCount())
		mWidget->removeRow(0);
}
//
void GdbRegisters::targetLoaded()
{
	GdbCore::targetLoaded();
}
//
void GdbRegisters::targetRunning()
{
	GdbCore::targetRunning();
	mWidget->setEnabled(false);
}
//
void GdbRegisters::targetStopped()
{
	GdbCore::targetStopped();
	mWidget->setEnabled(true);
}
//
void GdbRegisters::targetExited()
{
	GdbCore::targetExited();
	mWidget->setEnabled(false);
}
//
int GdbRegisters::process(QGdbMessageCore m)
{
	return cmd.dispatchProcess(m);
}
//
int GdbRegisters::processError(QGdbMessageCore m)
{
	// TODO
	QMessageBox::warning(NULL, "Error in GDB Registers",m.msg);
	return PROCESS_TERMINED;
}
//
void GdbRegisters::processPrompt()
{
	numberOfRegisters = 0;
}
//
void GdbRegisters::showColor(QTableWidgetItem *p, QString a)
{
	// toggle color (black / red if value in tableWidget is not egal than new value
		if(p->text() != a)
			p->setForeground ( QBrush(Qt::red));
		else
			p->setForeground ( QBrush(Qt::black));
}
//
int GdbRegisters::processRegisters(QGdbMessageCore m)
{
	// TODO
	// extract answer from gdb
	QByteArray value = getParametre("answerGdb=", m.msg);
	// extract value from answer
	QRegExp exp("^(\\w+)\\s+(\\w+)\\s+(.*)");
	if(exp.exactMatch( value))
	{
		QStringList list = exp.capturedTexts();

		if(mWidget->item(numberOfRegisters, 0) != NULL)
		{
			mWidget->item(numberOfRegisters, 0)->setText(list.at(1));
		
			showColor(mWidget->item(numberOfRegisters, 1), list.at(2));
			mWidget->item(numberOfRegisters, 1)->setText(list.at(2));
		}
		else
		{
			mWidget->insertRow(numberOfRegisters);
			mWidget->setRowHeight ( numberOfRegisters, 20 );

			mWidget->setItem(numberOfRegisters, 0, new QTableWidgetItem);
			mWidget->item(numberOfRegisters, 0)->setText(list.at(1));
			mWidget->setItem(numberOfRegisters, 1, new QTableWidgetItem);
			mWidget->item(numberOfRegisters, 1)->setText(list.at(2));

			Qt::ItemFlags readOnly =  Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
		 	mWidget->item(numberOfRegisters, 0)->setFlags(readOnly );
		 	mWidget->item(numberOfRegisters, 1)->setFlags(readOnly );
		}
		numberOfRegisters++;
	}

	// auto stopping when promt event

	return PROCESS_WAITING;
}
//
