//==========================================

/*
	Class GdbBreakPoint
*/

#include "gdbBreakPoint.h"

GdbBreakPoint::GdbBreakPoint( GdbParser *p) :  GdbCore( p), 
			bJustAdd(0),
			interpreterAdd(0), interpreterDel(0),interpreterEnable(0),interpreterDisable(0)
{
	tabWidget = new QTableWidget();

//	getContainer()->setWidget(tabWidget);
//	getContainer()->setWindowTitle(name());

	tabWidget->setEnabled(false);
	tabWidget->setColumnCount(5);
	tabWidget->setColumnWidth(4,200);
	tabWidget->setColumnWidth(1,150);
	tabWidget->setColumnWidth(2,150);
	tabWidget->setColumnWidth(3,150);
	tabWidget->setHorizontalHeaderLabels(QStringList() << "Enable/Hit" << "Conditions" << "Bkpt Num" << "Line" << "File" );
 
	tabWidget->setSelectionBehavior (QAbstractItemView::SelectRows);

	connect(tabWidget, SIGNAL( itemClicked  ( QTableWidgetItem *)), this, SLOT(onEnableChanged(QTableWidgetItem*)));	

// function not available
//	connect(tabWidget, SIGNAL( itemChanged  ( QTableWidgetItem *)), this, SLOT(onConditionChanged(QTableWidgetItem*)));	
	connect(tabWidget, SIGNAL(  itemDoubleClicked( QTableWidgetItem *)) , this , SLOT(onBreakpointDoubleClicked(QTableWidgetItem*)));

	icon_hit = ":/icons/buttonok.png" ;

	cmd.setClass(this);

	cmd.connectEventStart("CTRL+B",&GdbBreakPoint::processFromEditor);
	cmd.connectEventStart("breakpoint-hit",&GdbBreakPoint::hitBreakpoint);
	cmd.connectEventStart("end-stepping-range",&GdbBreakPoint::hitBreakpoint);

	cmd.connectEventNotify("requested-breakpoint", &GdbBreakPoint::requestedBreakpoint);

	/*
			breapoint hit splited in two lines
			"Breakpoint 1, GdbPluginManger::addPlugin(QObject*) (this=0x4095318, "
			"  p=0x404543) at src/fsdfsdf.cpp:41"
	 */
	getParser()->addRestorLine(QRegExp("^Breakpoint\\s\\d+,\\s.*$"),QRegExp("^.*at\\s.*:\\d+$"));

//	connect(getContainer(), SIGNAL(  topLevelChanged ( bool) ), this, SLOT( onTopLevelChanged ( bool  )));

	waitEndProcess = false;
	
	start();
} 
//
void GdbBreakPoint::onTopLevelChanged ( bool b)
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
*/}
//
GdbBreakPoint::~GdbBreakPoint()
{
	delete tabWidget;
//	delete getContainer();
} 
//
QString GdbBreakPoint::name()
{
	return "GdbBreakPoint"; 
}
//

void GdbBreakPoint::gdbStarted()
{
	GdbCore::gdbStarted();
	hit(NULL, -1);
}
//
void GdbBreakPoint::gdbFinished()
{
	GdbCore::gdbFinished();

	tabWidget->setEnabled(false);
	while(tabWidget->rowCount() ) 
		tabWidget->removeRow(0);
	BreakPointList.clear();
}
//
void GdbBreakPoint::targetLoaded()
{
	GdbCore::targetLoaded();
	tabWidget->setEnabled(true);
	hit(NULL, -1);
}
//
void GdbBreakPoint::targetRunning()
{
	GdbCore::targetRunning();
	tabWidget->setEnabled(false);
	hit(NULL, -1);
}
//
void GdbBreakPoint::targetStopped()
{
	GdbCore::targetStopped();
	tabWidget->setEnabled(true);
}
//
void GdbBreakPoint::targetExited()
{
	GdbCore::targetExited();
	tabWidget->setEnabled(true);
	hit(NULL, -1);
}
//
int GdbBreakPoint::requestedBreakpoint(QGdbMessageCore m)
{
	QByteArray fullName = QFileInfo(getParametre("fileName=", m.msg)).fileName ().toLocal8Bit();

	QBreakPoint *bp = getBreakPointByName(fullName);
	if(bp)
	{
		for(int i=0; i<bp->lineInFile.count(); i++)
		processPostMessage(-1,
			 "sender=\"" + name().toLocal8Bit() + "\",event=\"notify-breakpoint-add\",type=\"" + bp->type.at(i) + "\",enable=\"" + bp->enable.at(i) + "\",fileName=\"" + fullName + "\",line=\"" +QByteArray::number( bp->lineInFile.at(i)) +"\"");
	}

	return PROCESS_TERMINED;
}
//
int GdbBreakPoint::processFromEditor(QGdbMessageCore m)
{
	// if target running
	if(isTargetRunning())
		QMessageBox::warning(NULL,"Information", "i can't add or delete breakpoint when your target running.");
	else
	{
		QByteArray event = getParametre("event=", m.msg);
		QByteArray fullName = QFileInfo(getParametre("fullname=", m.msg)).fileName ().toLocal8Bit();
		QByteArray line = getParametre("line=", m.msg);
		toggleBreakPoint(fullName, line.toInt());
		return cmd.lockProcess();
	}
	return cmd.unlockProcess();
}
//
int GdbBreakPoint::process(QGdbMessageCore m)
{
	// efface les breakpoint hit si le prog est termine
	if(getParametre("event=", m.msg) == "exited-normally")
			hit(NULL, -1);

	return cmd.dispatchProcess(m);
}
//
int GdbBreakPoint::processError(QGdbMessageCore m)
{
	// TODO
waitEndProcess = false;
	QMessageBox::warning(NULL, "Error in GDB BreakPoint",m.msg);
	// END TOTO
	return PROCESS_TERMINED;
}
//
void GdbBreakPoint::processPrompt()
{
waitEndProcess = false;
		cmd.disconnectEventInterpreter(interpreterAdd);
		cmd.leaveEventInterpreter(&interpreterAdd);
		cmd.disconnectEventInterpreter(interpreterDel);
		cmd.leaveEventInterpreter(&interpreterDel);
		cmd.disconnectEventInterpreter(interpreterEnable);
		cmd.leaveEventInterpreter(&interpreterEnable);
		cmd.disconnectEventInterpreter(interpreterDisable);
		cmd.leaveEventInterpreter(&interpreterDisable);
}
//
int GdbBreakPoint::addBreakpoint(QGdbMessageCore m)
{	

	// Add BreakPoint
	//^info,reason="breakpoint-add",value="Breakpoint 1 at 0x805b850: file src/main.cpp, line 17."
	
	QByteArray value = getParametre("answerGdb=", m.msg);
	QRegExp exp("^Breakpoint\\s+(\\d+)\\s+at\\s(\\w+):\\s+file\\s+([^,]+),\\s+line\\s+(\\d+)\\.(|\\s+\\(\\d+\\s\\w*\\))" );
	
	if(exp.exactMatch(value))
	{
		QStringList list = exp.capturedTexts();

		QByteArray fullName = QFileInfo(list.at(3).toLocal8Bit()).fileName().toLocal8Bit();
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
				bp->type << "unconditional-breakpoint";
				bp->enable << "true";

				addRowInTab(/* bp,*/number, line,fullName);

				// noitify other plugin 
			processPostMessage(-1,
				 "sender=\"" + name().toLocal8Bit() + "\",event=\"notify-breakpoint-add\",type=\"unconditional-breakpoint\",enable=\"true\",fileName=\"" + fullName + "\",line=\"" + line + "\"");
			}
			else QMessageBox::warning(NULL, "Error", "(Class GdbBreakPoint function process::AddBreakpoint) CRITICAL erreur GdbBreakPoint is already set !");
		}
		else
		{
			// le fichier source n'existe pas, donc ne contient pas de bp
			QBreakPoint *bp = new QBreakPoint;
			bp->fileName = fullName;
			bp->indexInGdb << number.toInt();
			bp->lineInFile << line.toInt();	
			bp->type << "unconditional-breakpoint";
			bp->enable << "true";
			addRowInTab(/* bp,*/ number, line, fullName);

			BreakPointList << bp;
		
			// noitify other plugin 
			processPostMessage(-1,
				 "sender=\"" + name().toLocal8Bit() + "\",event=\"notify-breakpoint-add\",type=\"unconditional-breakpoint\",enable=\"true\",fileName=\"" + fullName + "\",line=\"" + line + "\"");
		}
		
		cmd.disconnectEventInterpreter(interpreterAdd);
		cmd.leaveEventInterpreter(&interpreterAdd);
	}
	return PROCESS_TERMINED;
}
//
int GdbBreakPoint::deleteBreakpoint(QGdbMessageCore m)
{
//	id=id; // no warning
	//	"^info,interpreter=\"GdbBreakPoint\",reason=\"breakpoint-deleted\",fullname=\"" + fullName.toLocal8Bit() + "\",nobkpt=\"" + QByteArray::number(bp->indexInGdb.at(index)) + "\",value=\"");
	QByteArray answerExtention = getParametre("answerExtention=",m.msg);

	QByteArray fullName = getParametre("fullname=", answerExtention);
	QByteArray  nobkpt = getParametre("nobkpt=", answerExtention);

	QBreakPoint *bp = getBreakPointByName(fullName);
	if(bp != NULL)
	{
		int index = bp->indexInGdb.indexOf(nobkpt.toInt());
		if(index!=-1 )
		{
			processPostMessage(-1,
				"sender=\"" + name().toLocal8Bit() + "\",event=\"notify-breakpoint-deleted\",type=\"" + bp->type.at(index) + "\",enable=\"" + bp->enable.at(index) +"\",fileName=\"" + bp->fileName.toLocal8Bit() + "\",line=\"" + QByteArray::number(bp->lineInFile.at(index)) + "\"");

			int indexLine = getBreakPoint( nobkpt );

			bp->lineInFile.removeAt(index);
			bp->indexInGdb.removeAt(index);				
			bp->type.removeAt(index);
			bp->enable.removeAt(index);
			
			tabWidget->removeRow( indexLine );	// delete line 
			// FIX ME
			// if bp->lineInFile.count() = 0 you can delete bp
		}
		else QMessageBox::warning(NULL,"Error","(Class GdbBreakPoint function process::RemoveBreakpoint) CRITICAL erreur BreakPoint no found!");
	}
	else QMessageBox::warning(NULL, "Error" ,"(Class GdbBreakPoint function process::RemoveBreakpoint) CRITICAL erreur file source not found !");


	cmd.disconnectEventInterpreter(interpreterDel);
	cmd.leaveEventInterpreter(&interpreterDel);

	return PROCESS_TERMINED;
}
//
int GdbBreakPoint::hitBreakpoint(QGdbMessageCore m )
{
//	id=id; // no warning

	QByteArray value = getParametre("answerGdb=", m.msg);
	QRegExp exp("^Breakpoint\\s+(\\d+),\\s+.*\\s+at\\s+([^:]+):(\\d+)" );

	if(exp.exactMatch(value))
	{
		QStringList list = exp.capturedTexts();

		QByteArray  bkptno = list.at(1).toLocal8Bit();
		QByteArray fullName = QFileInfo( list.at(2).toLocal8Bit()).fileName().toLocal8Bit();
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
								"sender=\"" + name().toLocal8Bit() + "\",event=\"notify-breakpoint-moved\",type=\"" + bp->type.at(i) + "\",enable=\"" + bp->enable.at(i) +"\",fileName=\"" + bp->fileName.toLocal8Bit() + "\",beforLine=\"" + QByteArray::number(bp->lineInFile.at(i)) + "\",afterLine=\" "  + line + "\"");
							qDebug("(Class GdbBreakPoint function process::BreakpointHit) Warnning : Move BreakPoint in line " + QByteArray::number(bp->lineInFile.at(i)) + " to " + line); 
							bp->lineInFile.replace (i,line.toInt());
							
							tabWidget->item(getBreakPoint(bkptno), 3) ->setText(line);
						}
						hit(bp ,  bkptno.toInt());
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
int GdbBreakPoint::enableBreakpoint(QGdbMessageCore m)
{
	//answerExtention={nobkpt="5",fullname="main.cpp"}
	QByteArray answerExtention = getParametre("answerExtention=" , m.msg);
	
	cmd.disconnectEventInterpreter(interpreterEnable);
	cmd.leaveEventInterpreter(&interpreterEnable);
	
	QBreakPoint *bp = getBreakPointByName(getParametre("fileName=", answerExtention) );
	int index = asBreakPointAt(bp ,getParametre("line=", answerExtention).toInt() );
	bp->enable.replace(index, "true");

	processPostMessage(-1,
		"sender=\"" + name().toLocal8Bit() + "\",event=\"notify-breakpoint-enabled\",fileName=\"" + getParametre("fileName=", answerExtention) + "\",line=\"" + getParametre("line=", answerExtention) + "\"");
	return PROCESS_TERMINED;
}
//
/*
	Disable break
*/
int GdbBreakPoint::disableBreakpoint(QGdbMessageCore m)
{
//	id = id;

	QByteArray answerExtention = getParametre("answerExtention=" , m.msg);

	cmd.disconnectEventInterpreter(interpreterDisable);
	cmd.leaveEventInterpreter(&interpreterDisable);

	QBreakPoint *bp = getBreakPointByName(getParametre("fileName=", answerExtention) );
	int index = asBreakPointAt(bp , getParametre("line=", answerExtention).toInt() );
	bp->enable.replace(index,"false");

	processPostMessage(-1,
		"sender=\"" + name().toLocal8Bit() + "\",event=\"notify-breakpoint-disabled\",fileName=\"" + getParametre("fileName=", answerExtention) + "\",line=\"" + getParametre("line=", answerExtention) + "\"");
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
	if(waitEndProcess) return;

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

			waitEndProcess = true;
			cmd.connectEventInterpreter(interpreterDel, &GdbBreakPoint::deleteBreakpoint);
			cmd.forceProcess();
		}
		else // add
		{
/*
some time GDB version add at end of line "(4 Locations)"
Breakpoint 1 at 0x428176: file src/addon/gdbBreakPoint.cpp, line 23. (2 locations)
Breakpoint 3 at 0x427ed4: file src/addon/gdbBreakPoint.cpp, line 10.
*/
			interpreterAdd = new QGdbInterpreter("breakpoint-add",
				"break " + fullName.toLocal8Bit() + ":" + QByteArray::number(numLine) ,
				QRegExp("^break\\s[^:]+:\\d+"),
				QRegExp("^Breakpoint\\s\\d+\\sat\\s0x[^:]+:\\sfile\\s[^,]+,\\sline\\s\\d+\\.(|\\s+\\(\\d+\\s\\w*\\))"),
				"");

			waitEndProcess = true;
			cmd.connectEventInterpreter(interpreterAdd, &GdbBreakPoint::addBreakpoint);
			cmd.forceProcess();
		}
	}
	else // pas de fichier source donc add bp
	{
		interpreterAdd = new QGdbInterpreter("breakpoint-add",
			"break " + fullName.toLocal8Bit() + ":" + QByteArray::number(numLine) ,
			QRegExp("^break\\s[^:]+:\\d+"),
			QRegExp("^Breakpoint\\s\\d+\\sat\\s0x[^:]+:\\sfile\\s[^,]+,\\sline\\s\\d+\\.(|\\s+\\(\\d+\\s\\w*\\))"),
			"");
	
		waitEndProcess = true;
		cmd.connectEventInterpreter(interpreterAdd, &GdbBreakPoint::addBreakpoint);
		cmd.forceProcess();
	}
}
// change enable / disable GdbBreakPoint
void GdbBreakPoint::onEnableChanged(QTableWidgetItem *item)
{
	int numBp = tabWidget->item(tabWidget->currentRow(),2)->text().toInt();

	if(item == tabWidget->item(tabWidget->currentRow(),0))

	for(int i=0 ;i<BreakPointList.count(); i++)
	{
		for(int j=0; j<BreakPointList.at(i)->indexInGdb.count(); j++)
		{
			if(numBp == BreakPointList.at(i)->indexInGdb.at(j))
			{
				if(item->checkState () == Qt::Checked) 
				{
					interpreterEnable = new QGdbInterpreter("breakpoint-enable",
						"enable " + QByteArray::number(BreakPointList.at(i)->indexInGdb.at(j)),
						QRegExp("^enable\\s\\d+"),
						QRegExp("^\\(gdb\\)\\s"), 	
						"type=\"" + BreakPointList.at(i)->type.at(j) + "\",enable=\"true\",line=\"" + QByteArray::number(BreakPointList.at(i)->lineInFile.at(j) )+ "\",nobkpt=\"" + QByteArray::number( BreakPointList.at(i)->indexInGdb.at(j) ) + "\",fileName=\"" + BreakPointList.at(i)->fileName.toLocal8Bit() + "\"");
					cmd.connectEventInterpreter(interpreterEnable, &GdbBreakPoint::enableBreakpoint);
					cmd.forceProcess();
				}
				else
				{
					interpreterDisable = new QGdbInterpreter("breakpoint-disable",
						"disable " + QByteArray::number(BreakPointList.at(i)->indexInGdb.at(j)),
						QRegExp("^^disable\\s\\d+"),
						QRegExp("^\\(gdb\\)\\s"), 	
						"type=\"" + BreakPointList.at(i)->type.at(j) + "\",enable=\"false\",line=\"" + QByteArray::number(BreakPointList.at(i)->lineInFile.at(j)) + "\",nobkpt=\"" + QByteArray::number( BreakPointList.at(i)->indexInGdb.at(j) ) + "\",fileName=\"" + BreakPointList.at(i)->fileName.toLocal8Bit() + "\"");
					cmd.connectEventInterpreter(interpreterDisable, &GdbBreakPoint::disableBreakpoint);
					cmd.forceProcess();
				}
			}
		}
	}
}
//
void GdbBreakPoint::onConditionChanged(QTableWidgetItem *item)
{
	int numBp = tabWidget->item(tabWidget->currentRow(),2)->text().toInt();

	// find item in GdbBreakPointList
	for(int i=0 ;i<BreakPointList.count(); i++)
	{
		for(int j=0; j<BreakPointList.at(i)->indexInGdb.count(); j++)
		{
			if(numBp == BreakPointList.at(i)->indexInGdb.at(j))
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
void GdbBreakPoint::addRowInTab(/*QBreakPoint *bp )//,*/QByteArray number, QByteArray line, QByteArray file)
{

	// because when you add new row in tab
	// it send itemChanged signal!
	bJustAdd = true;

	// recherche le nombre de ligne
	int numberOfRow = tabWidget->rowCount();

	// add row
	tabWidget->insertRow(numberOfRow);

//QMetaObject::invokeMethod(tabWidget, SLOT(setRowHeight ( int, int)), 
//	Q_ARG(int, numberOfRow,).Q_ARG(int, 20);
	tabWidget->setRowHeight ( numberOfRow, 20 );

	tabWidget->setItem(numberOfRow,0, new QTableWidgetItem);//bp->enable.at(nbr-1));
	tabWidget->setItem(numberOfRow,1, new QTableWidgetItem);//bp->condition.at(nbr-1));
	tabWidget->setItem(numberOfRow,2, new QTableWidgetItem);//bp->bkptno.at(nbr-1));
	tabWidget->setItem(numberOfRow,3, new QTableWidgetItem);//bp->line.at(nbr-1));
	tabWidget->setItem(numberOfRow,4, new QTableWidgetItem);//bp->file.at(nbr-1));

	// set item read only
	Qt::ItemFlags readOnly =  Qt::ItemIsEnabled | Qt::ItemIsSelectable  | Qt::ItemIsUserCheckable;

	tabWidget->item(numberOfRow, 0)->setIcon(QIcon(""));
	tabWidget->item(numberOfRow, 0)->setCheckState(Qt::Checked);
 	tabWidget->item(numberOfRow, 0)->setFlags(readOnly );

	tabWidget->item(numberOfRow, 1)->setText("none"); 
	tabWidget->item(numberOfRow, 1)->setFlags(readOnly ); 

	tabWidget->item(numberOfRow, 2)->setText(number); 
	tabWidget->item(numberOfRow, 2)->setFlags(readOnly );

	tabWidget->item(numberOfRow, 4)->setText(file); 
	tabWidget->item(numberOfRow, 4)->setFlags(readOnly );

	tabWidget->item(numberOfRow, 3)->setText(line); 
	tabWidget->item(numberOfRow, 3)->setFlags(readOnly );
}
//
// show GdbBreakPoint Hit in tabWidget (icon)
void GdbBreakPoint::hit(QBreakPoint *bp, int index)
{
	tabWidget->clearSelection ();

	for(int i=0; i< tabWidget->rowCount(); i++)
	{
		QTableWidgetItem *p = tabWidget->item(i,2);
		if(bp && p->text() == QString::number(index))
		{
			tabWidget->item(i,0)->setIcon(QIcon(icon_hit)); 
			tabWidget->item(i,0)->setText("hit");
			tabWidget->selectRow(i);
		}
		else
		{
			tabWidget->item(i,0)->setText("");
			tabWidget->item(i,0)->setIcon(QIcon("")); 
		}
	}
}
//
void GdbBreakPoint::onBreakpointDoubleClicked(QTableWidgetItem *it)
{
	int numRow = tabWidget->currentRow();

	tabWidget->item(numRow, 2);
	processPostMessage(-1,
		"sender=\"" + name().toLocal8Bit() + "\",event=\"notify-goto-breakpoint\",fileName=\"" + 	tabWidget->item(numRow, 4)->text().toLocal8Bit() + "\",line=\"" + 	tabWidget->item(numRow, 3 )->text().toLocal8Bit()+ "\"" );
}



/*
bon lors de la suppression d'un breakpoint on envoit a gdb :

(gdb) delete 1
(gdb) _

on vois que gdb n'est pas tres parlant en ce qui concerne la commande executée

on va donc mettre en place un interpreter
*/
/*
 interpreterDel = new QGdbInterpreter("breakpoint-deleted", // nom de l'evenement
   "delete " + QByteArray::number(bp->indexInGdb.at(index)),// commande a executer
   QRegExp("^delete\\s\\d+"), // meme commande mais en QRexgExp
   QRegExp("^\\(gdb\\)\\s"),  // la reponce attendue
   // des informations suplementaires
   "fullname=\"" + fullName + "\",nobkpt=\"" + QString::number(bp->indexInGdb.at(index)) + "\"");

   // on connect l'interpreter a la fonction deleteBreakpoint
   cmd.connectEventToProcess(interpreterDel, &GdbBreakPoint::deleteBreakpoint);
   // on force l'execution de la commande 
   cmd.forceProcess();
*/
/*
-----------------

un fois la commande executer le parser recherche une relation entre la command 
en cours (commande QRegExp) et  la reponce attendue (reponce QRegExp)

si c'est bon le parser emet :
^info,interpreter="GdbBreakPoint",event="breakpoint-deleted",answerExtention={fullname="main.cpp",nobkpt="1"},answerGdb="(gdb) ",currentCmd="delete 1"  id : 2

sinon 

^error,.....

C'est est un peut plus parlant :)

Le kernelDispatcher() va mettre en relation l'interpreter et la fonction, on
a donc appel de la fonction deleteBreakpoint()
Celle-ci effectue quelque truc et supprime l'interpreter par
*/
/*
  cmd.disconnectEventToProcess(interpreterDel);
  // mise a null du pointeur
  cmd.leaveEventToProcess(&interpreterDel);
*/
/*
le leave... evite les fuite memoire (new) et surtout met a NULL le pointeur
ce qui evite les crash aleatoire :)
*/
