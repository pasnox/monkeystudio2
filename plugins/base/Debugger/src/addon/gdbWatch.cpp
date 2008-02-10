//==========================================

/*
	Class GdbWatch
*/

#include "gdbWatch.h"

#define TOTAL_CHAMP		4


//QString varTest = "<QCoreApplication> = {<QObject> = {_vptr.QObject = 0xbfcef448,  static staticMetaObject = {d = {superdata = 0x0,           stringdata = 0xb76b48e0 QObject, data = 0xb76b4960,           extradata = 0x0}}, d_ptr = 0x8052608, static staticQtMetaObject = {        d = {superdata = 0x0, stringdata = 0xb76b7620 Qt, data = 0xb76b80a0,           extradata = 0x0}}}, static staticMetaObject = {d = {        superdata = 0x80bab60, stringdata = 0xb76b98a0 QCoreApplication,         data = 0xb76b9920, extradata = 0x0}}, static self = 0x0},  static staticMetaObject = {d = {superdata = 0xb76e90a4,       stringdata = 0xb7e6b580 QApplication, data = 0xb7e6b760,      extradata = 0x0}}" ; 



GdbWatch::GdbWatch(GdbParser *p) :  GdbCore( p), 
	mWidget(NULL), currentRow(0),	currentColumn(1)
{

	mWidget = new QTableWidgetEx();
	mWidget->setEnabled(false);
	mWidget->setColumnCount(TOTAL_CHAMP);
	mWidget->setColumnWidth(3,200);
	mWidget->setColumnWidth(2,200);
	mWidget->setColumnWidth(1,200);
	mWidget->setHorizontalHeaderLabels(QStringList() << "Name" << "Type" << "Address" << "Value"  ); 
	mWidget->setSelectionBehavior (QAbstractItemView::SelectRows);

	getContainer()->setWidget(mWidget);
//	getContainer()->setWidget(rootTree);
	getContainer()->setWindowTitle(name());

// fo test
/*	addNewVar("var test");

	rootTree = new QTreeWidget();
	rootTree->setColumnCount(5);

	QTreeWidgetItem *it = new QTreeWidgetItem(0);
	it->setText(0, "var name");
	rootTree->insertTopLevelItem(0, it);
	decompilStrut( it , varTest);

	mWidget->setCellWidget(0,0, rootTree); 
*/
// end test

	// drag and drop
	connect(mWidget, SIGNAL(newVariable(QString)), this , SLOT(addNewVar(QString)));
	// when remove variable
	connect(mWidget, SIGNAL(removeVariable(int)), this , SLOT(removeVar(int)));

	// create command
	cmd.setClass(this);	
	cmd.setResumeError(true);
	cmd.connectEventStart("breakpoint-hit" , NULL);
	cmd.connectEventStart("end-stepping-range" , NULL);
} 
//
GdbWatch::~GdbWatch()
{
	delete getContainer();
} 
//
QString GdbWatch::name()
{
	 return "GdbWatch"; 
}
//
void GdbWatch::gdbStarted()
{
	GdbCore::gdbStarted();
}
//
void GdbWatch::gdbFinished()
{
	GdbCore::gdbFinished();
	mWidget->setEnabled(false);

	while(mWidget->rowCount())	
	{
		mWidget->removeRow(0);
		removeVar(0);
	}
}
//
void GdbWatch::targetLoaded()
{
	GdbCore::targetLoaded();
	mWidget->setEnabled(true);
}
//
void GdbWatch::targetRunning()
{
	GdbCore::targetRunning();
	mWidget->setEnabled(false);
}
//
void GdbWatch::targetStopped()
{
	GdbCore::targetStopped();
	mWidget->setEnabled(true);
}
//
void GdbWatch::targetExited()
{
	GdbCore::targetExited();
	mWidget->setEnabled(true);
}
//
int GdbWatch::process(QGdbMessageCore m)
{
	return cmd.dispatchProcess(m);
}
//
void GdbWatch::showColor(QTableWidgetItem *p, QString a)
{
	if(p->text() != a)
		p->setForeground ( QBrush(Qt::red));
	else
		p->setForeground ( QBrush(Qt::black));
}
//
int GdbWatch::processError(QGdbMessageCore m)
{

	QString s = getParametre("answerGdb=" , m.msg);

	// show error in current column variable list
	switch(currentColumn)
	{
		case 1 : showColor(mWidget->item(currentRow, 3), s) ;mWidget->item(currentRow, 3)-> setText(s);break;
		case 2 : showColor(mWidget->item(currentRow, 2), s) ;mWidget->item(currentRow, 2)-> setText(s);break;
		case 3 : showColor(mWidget->item(currentRow, 1), s) ;mWidget->item(currentRow, 1)-> setText(s);break;
	}

	// reset print mode if error occure
	isPrintSequence = false;
	currentColumn++;
	buffer.clear();

	// rotate current Column for next row
	if( currentColumn >=  TOTAL_CHAMP) {currentRow++; currentColumn = 1;}

	return PROCESS_TERMINED;
}
// prompt event
void GdbWatch::processExit()
{
	if(isPrintSequence)
	{
		QRegExp exp("^\\$\\d+\\s=\\s(.*)\\(gdb\\) " );
		if(exp.exactMatch(  buffer))
		{
			QStringList list = exp.capturedTexts();
			showColor(mWidget->item(currentRow, 3), list.at(1));
			mWidget->item(currentRow, 3)->setText(list.at(1));
		}
		currentColumn++;
		isPrintSequence = false;
	}
	else
	{	// promp event
		currentRow=0;
		currentColumn=1;
	}
	buffer.clear();
}

//
int GdbWatch::processWatchPrint(QGdbMessageCore m)
{
	// entering in print sequence for multi lines
	isPrintSequence = true;

	QByteArray value = getParametre("answerGdb=", m.msg);

	QRegExp exp("(.*)" );
	if(exp.exactMatch(  value))
	{
		QStringList list = exp.capturedTexts();
		// show var value
		// in processExit , because the value can be splitted in two lines
		buffer+= list.at(1);
	}
	return PROCESS_WAITING;
}
//
int GdbWatch::processWatchAdresse(QGdbMessageCore m)
{

	QByteArray value = getParametre("answerGdb=", m.msg);

	QRegExp exp("^\\$\\d+\\s=\\s(.*)" );
	if(exp.exactMatch(  value))
	{
		QStringList list = exp.capturedTexts();

		showColor(mWidget->item(currentRow, 2), list.at(1));

		// show var address
		mWidget->item(currentRow, 2)->setText(list.at(1));

		currentColumn++;

		return PROCESS_TERMINED;
	}
	return PROCESS_WAITING;
}
//
int GdbWatch::processWatchType(QGdbMessageCore m)
{

	QByteArray value = getParametre("answerGdb=", m.msg);

	QRegExp exp("^type\\s=\\s(.*)" );
	if(exp.exactMatch(  value))
	{
		QStringList list = exp.capturedTexts();

		// show var type
		mWidget->item(currentRow, 1)->setText(list.at(1));

		currentColumn=1;
		currentRow++;

		return PROCESS_TERMINED;
	}
	return PROCESS_WAITING;
}
//
void GdbWatch::addNewVar(QString varName)
{
	int numOfvar = varList.interpreterType.count() ;

	// create interpreter and command
	varList.interpreterPrint << new QGdbInterpreter("watch-print",
		"p " + varName,
		QRegExp("^p " + cmd.convertRegExp(varName)),
// because some time the answer is splitted in multi lines
//		QRegExp("^\\$\\d+\\s=\\s.*"),
		QRegExp(".*"),
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

	cmd.connectEventInterpreter(varList.interpreterPrint.at(numOfvar), &GdbWatch::processWatchPrint);
	cmd.connectEventInterpreter(varList.interpreterAdresse.at(numOfvar),  &GdbWatch::processWatchAdresse);
	cmd.connectEventInterpreter(varList.interpreterType.at(numOfvar),  &GdbWatch::processWatchType);

	int numberOfRow = mWidget->rowCount();

	mWidget->insertRow(numberOfRow);
	mWidget->setRowHeight ( numberOfRow, 20 );

	mWidget->setItem(numberOfRow, 0, new QTableWidgetItem);
	mWidget->setItem(numberOfRow, 1, new QTableWidgetItem);
	mWidget->setItem(numberOfRow, 2, new QTableWidgetItem);
	mWidget->setItem(numberOfRow, 3, new QTableWidgetItem);
	mWidget->item(numberOfRow,0)->setText(varName);

	Qt::ItemFlags readOnly =  Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
 	mWidget->item(numberOfRow, 0)->setFlags(readOnly );
	mWidget->item(numberOfRow, 1)->setFlags(readOnly );
	mWidget->item(numberOfRow, 2)->setFlags(readOnly );
	mWidget->item(numberOfRow, 3)->setFlags(readOnly );

	cmd.forceProcess();
}
//
void GdbWatch::removeVar(int index)
{
	cmd.disconnectEventInterpreter(varList.interpreterPrint.at(index));
	cmd.disconnectEventInterpreter(varList.interpreterAdresse.at(index));
	cmd.disconnectEventInterpreter(varList.interpreterType.at(index));

	QGdbInterpreter *p = varList.interpreterPrint.at(index);
	cmd.leaveEventInterpreter(&p);
	p = varList.interpreterAdresse.at(index);
	cmd.leaveEventInterpreter(&p);
	p = varList.interpreterType.at(index);
	cmd.leaveEventInterpreter(&p);

	varList.interpreterPrint.removeAt(index);
	varList.interpreterAdresse.removeAt(index);
	varList.interpreterType.removeAt(index);
}

/*
	Drag and Drop
*/
#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>

//==============================
QTableWidgetEx::QTableWidgetEx(QWidget *p) : QTableWidget(p)
{
	setAcceptDrops(true);
}

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
		if(index != -1)
		{
			removeRow(index);
			emit removeVariable(index);
		}
	}
	QTableWidget::mousePressEvent(event);
}


//===============================
/*

Info	:	decompilation des structures 	
by	:	Xiantia
version	:	1.0
date	:	28/11/06
licence	:	owner
 	
*/



// extrait les membres de la  structure
// exemple : "rtf = {art = {0}, a = 5 , b = 6 , rtf  = {t = 12 , r = 34}}"
// return  :
// 1 : art = {0}
// 2 : a = 5
// 3 : b = 6
// 4 : rtf = {t = 12 , r = 34}

QStringList GdbWatch::extractMember(QString val)
{
int count =-1;
QString member;
QStringList memberList;

	for(int i = val.indexOf("{")+1; i<val.length(); i++)
	{
		if(val.at(i) == '{')
		{
			do
			{
				if(val.at(i) == '}')
					count--;
				if(val.at(i) == '{')
					count==-1 ? count=1 : count++;
		
				 member.append(val.at(i));
				
				i++;
			}
			while(count !=0);
		
			memberList << member;
			member.clear();
		}
		else
		{
			if(val.at(i) == ',' || val.at(i) == '}')
			{
				if(!member.isEmpty())
				{
					memberList << member;
				 }
				member.clear();
			}
			else
				member.append(val.at(i));
		}
	}
	return memberList;
}


// return le type du membre de la structure
// 2 : c'est un autre structure
// 1 : membre " x= y"
// 0 : membre orphelin "0x00"
int GdbWatch::memberType(QString val)
{
QString name;
QStringList arg;

	arg = val.split("{");
	if(arg.at(0).isEmpty()) return 0;
	
	if(val.contains(" = {"))
		 return 2;

	if(val.contains("="))
		return 1;
	return 0; // member orphelin
}


// formatage des membres
QStringList GdbWatch::formatMember(QString val)
{
	if(val.contains("="))
	{
		QStringList list = val.split(" = ");
		return QStringList()<<list.at(0) <<list.at(1);
	}	
	if(val.contains(" = {"))
	{
		return QStringList()<<val <<"";
	}	
	return QStringList()<<val << val;
}


// recursive function
void GdbWatch::decompilStrut(QTreeWidgetItem *parentItem, QString val)
{
QString memberBlock;
int indexTab=0;

	QStringList memberList = extractMember(val);
	
	for(int i=0; i<memberList.count();i++)
	{
		memberBlock = memberList.at(i);
		while(memberBlock.startsWith(" ")) memberBlock.remove(0,1);
		QTreeWidgetItem *child;	
		child = new QTreeWidgetItem(parentItem);
		switch(memberType(memberBlock))
		{
			case 0:
				child->setText(0, "[0x" + QString::number(indexTab++,16) +"]");
				child->setText(3, "$ = " + memberBlock);
				decompilStrut(child, memberBlock);
			break;

			case 1 :
				child->setText(0, formatMember(memberBlock).at(0));
				child->setText(3, "$ = " + formatMember(memberBlock).at(1));
			break;
			case 2:
				child->setText(0, formatMember(memberBlock).at(0));
				child->setText(3, "$ = " + memberList.at(i).right(memberList.at(i).length() - memberList.at(i).indexOf(" = {") - 3));
				decompilStrut(child, memberBlock);
		} // end switch
	}// end for
}


void GdbWatch::decompilStrutUpdate(QTreeWidgetItem *parentItem, QString val)
{
QString memberBlock;
int index=0;

	QStringList memberList = extractMember(val);

	for(int i=0; i<memberList.count();i++)
	{
qDebug(memberList.at(i).toLocal8Bit());

		memberBlock = memberList.at(i);
		while(memberBlock.startsWith(" ")) memberBlock.remove(0,1);
		QTreeWidgetItem *child;	
		child = parentItem->child(index);
		// quelques fois Gdb ajout un nouveau membre et comme il n'est 
		// pas initialis� on plante d'iou le test suivant
		if(child == NULL ) return;
		
		switch(memberType(memberBlock))
		{
			case 0:
				child->setText(3, "$ = " + memberBlock);
				decompilStrutUpdate(child, memberBlock);
			break;
			case 1 :
//				isVariableChangedValue(child,"$ = " + formatMember(memberBlock).at(1));
				child->setText(3, "$ = " + formatMember(memberBlock).at(1));
			break;
			case 2:
//				isVariableChangedValue(child,"$ = " + memberList.at(i).right(memberList.at(i).length() - memberList.at(i).indexOf(" = {") - 3));
				child->setText(3, "$ = " + memberList.at(i).right(memberList.at(i).length() - memberList.at(i).indexOf(" = {") - 3));
				decompilStrutUpdate(child, memberBlock);
		} // end switch
		index++;
	}// end for
}
