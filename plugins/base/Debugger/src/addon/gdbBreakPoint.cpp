//==========================================

/*
	Class GdbBreakPoint
*/

#include "gdbBreakPoint.h"

GdbBreakPoint::GdbBreakPoint(QWidget *o) :  GdbBase(o), bJustAdd(0), bTargetLoaded(0), bTargetRunning(0), bGdbStarted(0)
{
	tabWidget = new QTableWidget(this);
	tabWidget->setEnabled(false);
	tabWidget->setColumnCount(5);
	tabWidget->setColumnWidth(4,200);
	tabWidget->setColumnWidth(1,150);
	tabWidget->setColumnWidth(2,150);
	tabWidget->setColumnWidth(3,150);
	tabWidget->setHorizontalHeaderLabels(QStringList() << "Enable/Hit" << "Conditions" << "Bkpt Num" << "Line" << "File" );
 
	tabWidget->setSelectionBehavior (QAbstractItemView::SelectRows);
	
//	connect(tabWidget, SIGNAL( itemClicked  ( QTableWidgetItem *)), this, SLOT(onEnableChanged(QTableWidgetItem*)));	
//	connect(tabWidget, SIGNAL( itemChanged  ( QTableWidgetItem *)), this, SLOT(onConditionChanged(QTableWidgetItem*)));	

	icon_hit = ":/icons/buttonok.png" ;

	cmd.setClass(this);
	cmd.connectEventStartToProcess("CTRL+B",&GdbBreakPoint::processFromEditor);
} 
//
GdbBreakPoint::~GdbBreakPoint()
{
} 
//
QString GdbBreakPoint::name()
{
	return "GdbBreakPoint"; 
}
//
QWidget * GdbBreakPoint::widget()
{ 
	return (QWidget*) tabWidget ; 
}
//
void GdbBreakPoint::setupDockWidget(QMainWindow *mw)
{
	mw = mw;
	setWidget(widget());
	setWindowTitle(name());
	setAllowedAreas(Qt::AllDockWidgetAreas);
	setFeatures (QDockWidget::DockWidgetMovable |QDockWidget::DockWidgetFloatable);
//	mw->addDockWidget(Qt::RightDockWidgetArea, this);
}
//
void GdbBreakPoint::gdbStarted()
{
	bGdbStarted = true;
	hit(NULL, -1);
}
//
void GdbBreakPoint::gdbFinished()
{
	bGdbStarted = false;
	bTargetLoaded = false;
	hit(NULL, -1);
}
//
void GdbBreakPoint::targetLoaded()
{
	bTargetLoaded = true;
	tabWidget->setEnabled(true);
	hit(NULL, -1);
}
//
void GdbBreakPoint::targetRunning()
{
	bTargetRunning = true;
	tabWidget->setEnabled(false);
	hit(NULL, -1);
}
//
void GdbBreakPoint::targetStopped()
{
	bTargetRunning = false;
	tabWidget->setEnabled(true);
}
//
void GdbBreakPoint::targetExited()
{
	bTargetRunning = false;
	tabWidget->setEnabled(true);
	hit(NULL, -1);
}
//
int GdbBreakPoint::processFromEditor(int id, QByteArray data)
{
id = id;

	QByteArray event = getParametre("event=", data);
	QByteArray fullName = QFileInfo(getParametre("fullname=", data)).fileName ().toLocal8Bit();
	QByteArray line = getParametre("line=", data);
	toggleBreakPoint(fullName, line.toInt());

	return cmd.lockProcess();
}
//
int GdbBreakPoint::process(int id ,QByteArray data)
{
	if(!bGdbStarted || bTargetRunning || !bTargetLoaded) return PROCESS_TERMINED;

	if(cmd.startProcess(data))
			return hitBreakpoint(id, data);

	// efface les breakpoint hit si le prog est termine
	if(getParametre("event=", data) == "exited-normally")
			hit(NULL, -1);

	return cmd.dispatchProcess(id ,data);
}
//
int GdbBreakPoint::processError(int id, QByteArray data)
{
	// TODO
	id = id;
	data = data; 

	// END TOTO
	return PROCESS_TERMINED;
}
//
void GdbBreakPoint::processExit()
{
		cmd.disconnectEventToProcess(interpreterAdd);
		cmd.leaveEventToProcess(&interpreterAdd);
		cmd.disconnectEventToProcess(interpreterDel);
		cmd.leaveEventToProcess(&interpreterDel);
		cmd.disconnectEventToProcess(interpreterEnable);
		cmd.leaveEventToProcess(&interpreterEnable);
		cmd.disconnectEventToProcess(interpreterDisable);
		cmd.leaveEventToProcess(&interpreterDisable);
}
//
int GdbBreakPoint::addBreakpoint(int id , QByteArray data)
{	
	id=id; // no warning

	// Add BreakPoint
	//^info,reason="breakpoint-add",value="Breakpoint 1 at 0x805b850: file src/main.cpp, line 17."
	
	QByteArray value = getParametre("answerGdb=", data);
	QRegExp exp("^Breakpoint\\s+(\\d+)\\s+at\\s(\\w+):\\s+file\\s+([^,]+),\\s+line\\s+(\\d+)\\." );
	
	if(exp.exactMatch(value))
	{
		QStringList list = exp.capturedTexts();


		QByteArray fullName = QFileInfo("./"+ list.at(3).toLocal8Bit()).fileName().toLocal8Bit();
		QByteArray line = list.at(4).toLocal8Bit();
		QByteArray number = list.at(1).toLocal8Bit();

		QBreakPoint *bp = getBreakPointByName(fullName);
		if(bp != NULL)
		{
			// fichier source exist et comporte deja de bp , ajout du nouveau bp
			if(bp->lineInFile.indexOf(line.toInt()) == -1)
			{
				bp->indexInGdb << number.toInt();
				bp->lineInFile << line.toInt();

				addRowInTab( bp,number, line,fullName);

				// noitify other plugin 
				processPostMessage(-1, 
					"sender=\"" + name().toLocal8Bit() + "\",event=\"breakpoint-add\",fileName=\"" + fullName + "\",line=\"" + line + "\"");
			}
			else qDebug("(Class GdbBreakPoint function process::AddBreakpoint) CRITICAL erreur GdbBreakPoint is already set !");
		}
		else
		{
			// le fichier source n'existe pas, donc ne contient pas de bp
			QBreakPoint *bp = new QBreakPoint;
			bp->fileName = fullName;
			bp->indexInGdb << number.toInt();
			bp->lineInFile << line.toInt();	

			addRowInTab(bp ,number, line, fullName);

			BreakPointList << bp;
		
			// noitify other plugin 
			processPostMessage(-1, "sender=\"" + name().toLocal8Bit() + "\",event=\"breakpoint-add\",fileName=\"" + fullName + "\",line=\"" + line + "\"");
		}
		
		cmd.disconnectEventToProcess(interpreterAdd);
		cmd.leaveEventToProcess(&interpreterAdd);
	}
	return PROCESS_TERMINED;
}
//
int GdbBreakPoint::deleteBreakpoint(int id, QByteArray data)
{
	id=id; // no warning
	//	"^info,interpreter=\"GdbBreakPoint\",reason=\"breakpoint-deleted\",fullname=\"" + fullName.toLocal8Bit() + "\",nobkpt=\"" + QByteArray::number(bp->indexInGdb.at(index)) + "\",value=\"");
	QByteArray answerExtention = getParametre("answerExtention=",data);

	QByteArray fullName = getParametre("fullname=", answerExtention);
	QByteArray  nobkpt = getParametre("nobkpt=", answerExtention);

	QBreakPoint *bp = getBreakPointByName(fullName);
	if(bp != NULL)
	{
		int index = bp->indexInGdb.indexOf(nobkpt.toInt());
		if(index!=-1 )
		{
			processPostMessage(-1,
				"sender=\"" + name().toLocal8Bit() + "\",event=\"breakpoint-deleted\",fileName=\"" + bp->fileName.toLocal8Bit() + "\",line=\"" + QByteArray::number(bp->lineInFile.at(index)) + "\"");

			int indexLine = getBreakPoint( nobkpt );

			bp->lineInFile.removeAt(index);
			bp->indexInGdb.removeAt(index);				

			delete bp->enable.at(index);	// delete the QTableWidgetItem
			bp->enable.removeAt(index);		// remove it in list
			delete bp->condition.at(index);	// delete the QTableWidgetItem
			bp->condition.removeAt(index);		// remove it in list
			delete bp->bkptno.at(index);	// delete the QTableWidgetItem
			bp->bkptno.removeAt(index);		// remove it in list
			delete bp->line.at(index);	// delete the QTableWidgetItem
			bp->line.removeAt(index);		// remove it in list
			delete bp->file.at(index);	// delete the QTableWidgetItem
			bp->file.removeAt(index);		// remove it in list

			tabWidget->removeRow( indexLine );	// delete line 


			// FIX ME
			// if bp->lineInFile.count() = 0 you can delete bp
		}
		else qDebug("(Class GdbBreakPoint function process::RemoveBreakpoint) CRITICAL erreur BreakPoint no found!");
	}
	else qDebug("(Class GdbBreakPoint function process::RemoveBreakpoint) CRITICAL erreur file source not found !");


	cmd.disconnectEventToProcess(interpreterDel);
	cmd.leaveEventToProcess(&interpreterDel);

	return PROCESS_TERMINED;
}
//
int GdbBreakPoint::hitBreakpoint(int id, QByteArray data)
{
	id=id; // no warning

	QByteArray value = getParametre("answerGdb=", data);
	QRegExp exp("^Breakpoint\\s+(\\d+),\\s+.*\\s+at\\s+([^:]+):(\\d+)" );

	if(exp.exactMatch(value))
	{
		QStringList list = exp.capturedTexts();

		// verif
		if(list.at(0) != value) 
		{ 
			qDebug("(Class GdbBreakpoint function proccess:hitBreakpoint)  ERROR list RegExpt empty");
			qDebug("* " + list.at(0).toLocal8Bit());
			qDebug("-> " + value);
			return PROCESS_TERMINED;
		}

		QByteArray  bkptno = list.at(1).toLocal8Bit();
		QByteArray fullName = QFileInfo("./"+ list.at(2).toLocal8Bit()).fileName().toLocal8Bit();
		QByteArray  line = list.at(3).toLocal8Bit();

		QBreakPoint *bp = getBreakPointByName(fullName);
		if(bp != NULL)
		{
			for(int i=0; i< bp->indexInGdb.count(); i++)
			{
				if(bp->indexInGdb.at(i) == bkptno.toInt()) 
				{
						if(bp->lineInFile.at(i) != line.toInt()) 
						{
							processPostMessage(-1,
								"sender=\"" + name().toLocal8Bit() + "\",event=\"breakpoint-moved\",fileName=\"" + bp->fileName.toLocal8Bit() + "\",beforLine=\"" + QByteArray::number(bp->lineInFile.at(i)) + "\",afterLine=\" "  + line + "\"");
							qDebug("(Class GdbBreakPoint function process::BreakpointHit) Warnning : Move BreakPoint in line " + QByteArray::number(bp->lineInFile.at(i)) + " to " + line); 
							bp->lineInFile.replace (i,line.toInt());
							bp->line.at(i)->setText(line);
						}
						hit(bp , i);
						return PROCESS_TERMINED;
				}
			}
			// del all icon if reason="end-stepping-range" and if line have no BreakPoint 
			hit( NULL ,-1);
		}
	}
	else hit( NULL ,-1);

	return PROCESS_TERMINED;
}
//
/*
	Enable break
*/
int GdbBreakPoint::enableBreakpoint(int id, QByteArray data)
{
	id = id;

	//answerExtention={nobkpt="5",fullname="main.cpp"}
	QByteArray answerExtention = getParametre("answerExtention=" , data);
	
	cmd.disconnectEventToProcess(interpreterEnable);
	cmd.leaveEventToProcess(&interpreterEnable);

//	removeInterpreter();
	processPostMessage(-1,
		"sender=\"" + name().toLocal8Bit() + "\",event=\"breakpoint-enabled\",fileName=\"" + getParametre("fileName=", answerExtention) + "\",line=\"" + getParametre("line=", answerExtention) + "\"");
	return PROCESS_TERMINED;
}
//
/*
	Disable break
*/
int GdbBreakPoint::disableBreakpoint(int id, QByteArray data)
{
	id = id;

	QByteArray answerExtention = getParametre("answerExtention=" , data);

	cmd.disconnectEventToProcess(interpreterDisable);
	cmd.leaveEventToProcess(&interpreterDisable);
//removeInterpreter();
	processPostMessage(-1,
		"sender=\"" + name().toLocal8Bit() + "\",event=\"breakpoint-disabled\",fileName=\"" + getParametre("fileName=", answerExtention) + "\",line=\"" + getParametre("line=", answerExtention) + "\"");
	return PROCESS_TERMINED;
}
//
int GdbBreakPoint::asBreakPointAt(QBreakPoint *bp, int line)
{
	return bp->lineInFile.indexOf(line);
}
//
QBreakPoint * GdbBreakPoint::getBreakPointByName(QByteArray fileName)
{
	for(int i=0; i<BreakPointList.count() ;i++)
	{
		if(BreakPointList.at(i)->fileName == fileName) return BreakPointList.at(i);
	}
	return NULL;
}
//
QBreakPoint *GdbBreakPoint::getBreakPointByItem(QTableWidgetItem * item)
{
	for(int i=0 ;i<BreakPointList.count(); i++)
	{
		for(int j=0; j<BreakPointList.at(i)->enable.count(); j++)
		{
			if(item == BreakPointList.at(i)->enable.at(j))
				return  BreakPointList.at(i);
		}
	}
	return NULL;
}
//
int GdbBreakPoint::getBreakPoint(QString d)
{
	for(int i=0 ;i<tabWidget->rowCount(); i++)
	{
		QTableWidgetItem *it = tabWidget->item(i,2);
		if(d == it->text())
			return i;
	}
	return 0;
}
//
void GdbBreakPoint::toggleBreakPoint(QString fullName, int numLine)
{
	QBreakPoint *bp = getBreakPointByName( fullName.toLocal8Bit());
	if(bp !=NULL)
	{
		// file source exist
		// have already bp in this line ?
		int index  = asBreakPointAt(bp,numLine);

		if(index!=-1)
		{
			interpreterDel = new QGdbInterpreter("breakpoint-deleted",
				"delete " + QByteArray::number(bp->indexInGdb.at(index)),
				QRegExp("^delete\\s\\d+"),
				QRegExp("^\\(gdb\\)\\s"),
				"fullname=\"" + fullName + "\",nobkpt=\"" + QString::number(bp->indexInGdb.at(index)) + "\"");

			cmd.connectEventToProcess(interpreterDel, &GdbBreakPoint::deleteBreakpoint);
			cmd.forceProcess();
		}
		else // add
		{
			interpreterAdd = new QGdbInterpreter("breakpoint-add",
				"break " + fullName.toLocal8Bit() + ":" + QByteArray::number(numLine) ,
				QRegExp("^break\\s[^:]+:\\d+"),
				QRegExp("^Breakpoint\\s\\d+\\sat\\s0x[^:]+:\\sfile\\s[^,]+,\\sline\\s\\d+\\."),
				"");

			cmd.connectEventToProcess(interpreterAdd, &GdbBreakPoint::addBreakpoint);
			cmd.forceProcess();
		}
	}
	else // pas de fichier source donc add bp
	{
		interpreterAdd = new QGdbInterpreter("breakpoint-add",
			"break " + fullName.toLocal8Bit() + ":" + QByteArray::number(numLine) ,
			QRegExp("^break\\s[^:]+:\\d+"),
			QRegExp("^Breakpoint\\s\\d+\\sat\\s0x[^:]+:\\sfile\\s[^,]+,\\sline\\s\\d+\\."),
			"");

	
		cmd.connectEventToProcess(interpreterAdd, &GdbBreakPoint::addBreakpoint);
		cmd.forceProcess();
	}
}
// change enable / disable GdbBreakPoint
void GdbBreakPoint::onEnableChanged(QTableWidgetItem *item)
{
	for(int i=0 ;i<BreakPointList.count(); i++)
	{
		for(int j=0; j<BreakPointList.at(i)->enable.count(); j++)
		{
			if(item == BreakPointList.at(i)->enable.at(j))
			{
				if(item->checkState () == Qt::Checked) 
				{
					interpreterEnable = new QGdbInterpreter("breakpoint-enable",
						"enable " + QByteArray::number(BreakPointList.at(i)->indexInGdb.at(j)),
						QRegExp("^enable\\s\\d+"),
						QRegExp("^\\(gdb\\)\\s"), 	
						"line=\"" + QByteArray::number(BreakPointList.at(i)->lineInFile.at(j) )+ "\",nobkpt=\"" + QByteArray::number( BreakPointList.at(i)->indexInGdb.at(j) ) + "\",fileName=\"" + BreakPointList.at(i)->fileName.toLocal8Bit() + "\"");
	
					cmd.connectEventToProcess(interpreterEnable, &GdbBreakPoint::enableBreakpoint);
					cmd.forceProcess();
	/*				QBaseInterpreter interpreter={"breakpoint-enable",
						QRegExp("^enable\\s\\d+"),
						QRegExp("^\\(gdb\\)\\s"), 	
						"line=\"" + QByteArray::number(BreakPointList.at(i)->lineInFile.at(j) )+ "\",nobkpt=\"" + QByteArray::number( BreakPointList.at(i)->indexInGdb.at(j) ) + "\",fileName=\"" + BreakPointList.at(i)->fileName.toLocal8Bit() + "\""};

					addInterpreter(interpreter);

					emit sendRawData(this, "enable " + QByteArray::number(BreakPointList.at(i)->indexInGdb.at(j)));
	*/			}
				else
				{
					interpreterDisable = new QGdbInterpreter("breakpoint-disable",
						"disable " + QByteArray::number(BreakPointList.at(i)->indexInGdb.at(j)),
						QRegExp("^^disable\\s\\d+"),
						QRegExp("^\\(gdb\\)\\s"), 	
						"line=\"" + QByteArray::number(BreakPointList.at(i)->lineInFile.at(j)) + "\",nobkpt=\"" + QByteArray::number( BreakPointList.at(i)->indexInGdb.at(j) ) + "\",fileName=\"" + BreakPointList.at(i)->fileName.toLocal8Bit() + "\"");
	
					cmd.connectEventToProcess(interpreterDisable, &GdbBreakPoint::disableBreakpoint);
					cmd.forceProcess();

/*					QBaseInterpreter interpreter={"breakpoint-disable",
						QRegExp("^disable\\s\\d+"),
						QRegExp("^\\(gdb\\)\\s"),
						"line=\"" + QByteArray::number(BreakPointList.at(i)->lineInFile.at(j)) + "\",nobkpt=\"" + QByteArray::number( BreakPointList.at(i)->indexInGdb.at(j) ) + "\",fileName=\"" + BreakPointList.at(i)->fileName.toLocal8Bit() + "\""};

					addInterpreter(interpreter);
					
					emit sendRawData(this, "disable " + QByteArray::number(BreakPointList.at(i)->indexInGdb.at(j)));
*/				}
			}
		}
	}
}
//
void GdbBreakPoint::onConditionChanged(QTableWidgetItem *item)
{
	// find item in GdbBreakPointList
	for(int i=0 ;i<BreakPointList.count(); i++)
	{
		for(int j=0; j<BreakPointList.at(i)->condition.count(); j++)
		{
			if(item == BreakPointList.at(i)->condition.at(j))
			{
				if(item->text().isEmpty() && !bJustAdd) 
				{
					item->setText("none");
					return ;
				}

				if(!bJustAdd) // do not send after add breakoint
				{
					if(item->text()=="none") 
					{
						processPostMessage(-1,
							"sender=\"" + name().toLocal8Bit() + "\",event=\"breakpoint-unconditionned\",fileName=\"" + BreakPointList.at(i)->fileName.toLocal8Bit() + "\",line=\"" + QByteArray::number(BreakPointList.at(i)->lineInFile.at(j)) + "\"");
						emit sendRawData(this,"condition " + QByteArray::number(BreakPointList.at(i)->indexInGdb.at(j) ));
					}
					else
					{
						processPostMessage(-1,
							"sender=\"" + name().toLocal8Bit() + "\",event=\"breakpoint-conditionned\",fileName=\"" + BreakPointList.at(i)->fileName.toLocal8Bit() + "\",line=\"" + QByteArray::number(BreakPointList.at(i)->lineInFile.at(j)) + "\"");
						emit sendRawData(this,"condition " + QByteArray::number(BreakPointList.at(i)->indexInGdb.at(j)) + " " + item->text().toLocal8Bit());
					}
				}
				bJustAdd = false;
				return;
			}
		}
	}	
}
//
// Contener
//
void GdbBreakPoint::addRowInTab(QBreakPoint *bp ,QByteArray number, QByteArray line, QByteArray file)
{
	// because when you add new row in tab
	// it send itemChanged signal!
	bJustAdd = true;

	// recherche le nombre de ligne
	int index = tabWidget->rowCount();

	// create a new item
	bp->enable << new QTableWidgetItem();
	bp->condition << new QTableWidgetItem();
	bp->bkptno << new QTableWidgetItem();
	bp->line << new QTableWidgetItem();
	bp->file << new QTableWidgetItem();

	// add row
	tabWidget->insertRow(index);
	tabWidget->setRowHeight ( index, 20 );

	int nbr = bp->bkptno.count();

	// set item read only
	Qt::ItemFlags readOnly = bp->file.at(nbr-1)->flags() ^ Qt::ItemIsEditable;

	bp->enable.at(nbr-1)->setIcon(QIcon(""));
	bp->enable.at(nbr-1)->setCheckState(Qt::Checked);
 	bp->enable.at(nbr-1)->setFlags(readOnly );

	bp->condition.at(nbr-1)->setText("none"); 
	bp->condition.at(nbr-1)->setFlags(readOnly ); 

	bp->bkptno.at(nbr-1)->setText(number); 
	bp->bkptno.at(nbr-1)->setFlags(readOnly );

	bp->file.at(nbr-1)->setText(file); 
	bp->file.at(nbr-1)->setFlags(readOnly );

	bp->line.at(nbr-1)->setText(line); 
	bp->line.at(nbr-1)->setFlags(readOnly );

	tabWidget->setItem(index,0, bp->enable.at(nbr-1));
	tabWidget->setItem(index,1, bp->condition.at(nbr-1));
	tabWidget->setItem(index,2, bp->bkptno.at(nbr-1));
	tabWidget->setItem(index,3, bp->line.at(nbr-1));
	tabWidget->setItem(index,4, bp->file.at(nbr-1));

}
//
// show GdbBreakPoint Hit in tabWidget (icon)
void GdbBreakPoint::hit(QBreakPoint *bp, int index)
{
	tabWidget->clearSelection ();

	for(int i=0; i< tabWidget->rowCount(); i++)
	{
		QTableWidgetItem *p = tabWidget->item(i,0);
		if(bp && p == bp->enable.at(index))
		{
			p->setIcon(QIcon(icon_hit)); 
			p->setText("hit");
			tabWidget->selectRow(index);
		}
		else
		{
			p->setText("");
			p->setIcon(QIcon("")); 
		}
	}
}
//

