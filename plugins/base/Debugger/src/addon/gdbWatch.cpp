//==========================================

/*
	Class GdbWatch
*/

#include "gdbWatch.h"


QTableWidgetEx::QTableWidgetEx(QWidget *p) : QTableWidget(p)
{
	setAcceptDrops(true);

}

GdbWatch::GdbWatch(QWidget *o) :  GdbBase(o), bJustAdd(0), bTargetLoaded(0), bTargetRunning(0), bGdbStarted(0),mWidget(NULL), currentRow(0),
	currentColumn(0)
{
	mWidget = new QTableWidgetEx(this);
	mWidget->setEnabled(false);
	mWidget->setColumnCount(4);
	mWidget->setColumnWidth(3,200);
	mWidget->setColumnWidth(2,200);
	mWidget->setColumnWidth(1,200);
	mWidget->setHorizontalHeaderLabels(QStringList() << "Name" << "Type" << "Adress" << "Value"  ); 

	// drag and drop
	connect(mWidget, SIGNAL(newVariable(QString)), this , SLOT(addNewVar(QString)));
	// when remove variable
	connect(mWidget, SIGNAL(removeVariable(int)), this , SLOT(removeVar(int)));

	// create command
	cmd.setClass(this);	
	cmd.setResumeError(true);
} 
//
GdbWatch::~GdbWatch()
{
} 
//
QString GdbWatch::name()
{
	 return "GdbWatch"; 
}
//
QWidget * GdbWatch::widget()
{
	return (QWidget*) mWidget ; 
}
//
void GdbWatch::gdbStarted()
{
	bGdbStarted = true;
}
//
void GdbWatch::gdbFinished()
{
	bGdbStarted = false;
	bTargetLoaded = false;
	mWidget->setEnabled(false);
}
//
void GdbWatch::targetLoaded()
{
	mWidget->setEnabled(true);
	bTargetLoaded = true;
}
//
void GdbWatch::targetRunning()
{
	mWidget->setEnabled(false);
	bTargetRunning = true;
}
//
void GdbWatch::targetStopped()
{
	mWidget->setEnabled(true);
	bTargetRunning = false;
}
//
void GdbWatch::targetExited()
{
	mWidget->setEnabled(true);
	bTargetRunning = false;

}
//
void GdbWatch::setupDockWidget(QMainWindow *mw)
{
	mw = mw;
	if(mWidget)
	{
		setWidget(widget());
		setWindowTitle(name());
		setAllowedAreas(Qt::AllDockWidgetAreas);
		setFeatures (QDockWidget::DockWidgetMovable |QDockWidget::DockWidgetFloatable);
//	mw->addDockWidget(Qt::LeftDockWidgetArea, this);
	}
}
//
int GdbWatch::process(int id,QByteArray data)
{
	id = id;
	if(!bGdbStarted || bTargetRunning || !bTargetLoaded) return PROCESS_TERMINED;

	return cmd.dispatchProcess(id, data);
}
//
int GdbWatch::processError(int id, QByteArray data)
{
	id = id;

	// show error in current variable list

	switch(currentColumn)
	{
		case 0 : 	variableList.at(currentRow).varValueItem->setText(getParametre("answerGdb=" , data)); break;
		case 1 : 	variableList.at(currentRow).varAdresseItem->setText(getParametre("answerGdb=" , data)); break;
		case 2 : 	variableList.at(currentRow).varTypeItem->setText(getParametre("answerGdb=" , data)); break;
	}
	currentColumn++;

	if( currentColumn ==  3) {currentRow++; currentColumn = 0;}

	return PROCESS_TERMINED;
}
//
void GdbWatch::processExit()
{
	// promp event
	currentRow=0;
	currentColumn=0;
}
//
int GdbWatch::processWatchPrint(int id, QByteArray data)
{

	QByteArray value = getParametre("answerGdb=", data);

	QRegExp exp("^\\$\\d+\\s=\\s(.*)" );
	if(exp.exactMatch(  value))
	{
		QStringList list = exp.capturedTexts();

		// show var value
		variableList.at(currentRow).varValueItem->setText(list.at(1));
	
		currentColumn++;
	
		return PROCESS_TERMINED;
	}
	return PROCESS_WAITING;
}
//
int GdbWatch::processWatchAdresse(int id, QByteArray data)
{

	QByteArray value = getParametre("answerGdb=", data);

	QRegExp exp("^\\$\\d+\\s=\\s(.*)" );
	if(exp.exactMatch(  value))
	{
		QStringList list = exp.capturedTexts();

		// show var type
		variableList.at(currentRow).varAdresseItem->setText(list.at(1));

		currentColumn++;

		return PROCESS_TERMINED;
	}
	return PROCESS_WAITING;
}
//
int GdbWatch::processWatchType(int id, QByteArray data)
{

	QByteArray value = getParametre("answerGdb=", data);

	QRegExp exp("^type\\s=\\s(.*)" );
	if(exp.exactMatch(  value))
	{
		QStringList list = exp.capturedTexts();

		// show var type
		variableList.at(currentRow).varTypeItem->setText(list.at(1));

		currentColumn=0;
		currentRow++;

		return PROCESS_TERMINED;
	}
	return PROCESS_WAITING;
}
//

void GdbWatch::addNewVar(QString varName)
{
	int index = mWidget->rowCount();

	mWidget->insertRow(index);
	mWidget->setRowHeight ( index, 20 );
	mWidget->setSelectionBehavior (QAbstractItemView::SelectRows);

	int numOfvar = varList.interpreterType.count() ;

	// create interpreter and command
	varList.interpreterPrint << new QGdbInterpreter("watch-print",
		"p " + varName,
		QRegExp("^p " + cmd.convertRegExp(varName)),
		QRegExp("^\\$\\d+\\s=\\s.*"),
		"");


	varList.interpreterAdresse  << new QGdbInterpreter("watch-adresse",
		"p &" + varName,
		QRegExp("^p &" + cmd.convertRegExp(varName)),
		QRegExp("^\\$\\d+\\s=\\s.*"),
		"");


	varList.interpreterType  << new QGdbInterpreter("watch-type",
		"whatis " + varName,
		QRegExp("^whatis " + cmd.convertRegExp(varName)),
		QRegExp("^type\\s=\\s.*"),
		"");

	cmd.connectEventToProcess(varList.interpreterPrint.at(numOfvar), &GdbWatch::processWatchPrint);
	cmd.connectEventToProcess(varList.interpreterAdresse.at(numOfvar),  &GdbWatch::processWatchAdresse);
	cmd.connectEventToProcess(varList.interpreterType.at(numOfvar),  &GdbWatch::processWatchType);


	// create var list
	svariable v={varName,new QTableWidgetItem(), "..." ,new QTableWidgetItem(), "...",new QTableWidgetItem(), "...",new QTableWidgetItem()};
	variableList << v;

	v.varNameItem->setText(v.varName);
	mWidget->setItem(index,0, v.varNameItem);

	v.varTypeItem->setText(v.varType);
	mWidget->setItem(index,1, v.varTypeItem);
	 v.varTypeItem->setFlags(v.varTypeItem->flags() ^ Qt::ItemIsEditable);

	v.varAdresseItem->setText(v.varAdresse);
	mWidget->setItem(index,2, v.varAdresseItem);
	 v.varAdresseItem->setFlags(v.varAdresseItem->flags() ^ Qt::ItemIsEditable);

	v.varValueItem->setText(v.varValue);

	mWidget->setItem(index,3, v.varValueItem);

	cmd.forceProcess();
}

void GdbWatch::removeVar(int index)
{

/*	cmd.disconnectEventToProcess(varList.interpreterPrint.at(index));
	cmd.disconnectEventToProcess(varList.interpreterWatch.at(index));
	cmd.disconnectEventToProcess(varList.interpreterType.at(index));

	cmd.leaveEventToProcess(&varList.interpreterPrint.at(index));
	cmd.leaveEventToProcess(&varList.interpreterWatch.at(index));
	cmd.leaveEventToProcess(&varList.interpreterType.at(index));
*/
	variableList.removeAt(index);

}


#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>

//==============================
void QTableWidgetEx::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void QTableWidgetEx::dragMoveEvent(QDragMoveEvent *event)
{
   event->acceptProposedAction();
}

void QTableWidgetEx::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

	if (mimeData->hasText()) 
		emit newVariable(mimeData->text());
}

void QTableWidgetEx::dragLeaveEvent(QDragLeaveEvent *event)
{
   event->accept();
}

void QTableWidgetEx::mousePressEvent ( QMouseEvent * event ) 
{
	if(event->button() == Qt::RightButton)
	{
		int index = currentRow();
		removeRow(index);
		emit removeVariable(index);
	}
	QTableWidget::mousePressEvent(event);
}
