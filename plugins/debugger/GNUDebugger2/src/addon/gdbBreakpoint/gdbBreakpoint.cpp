/********************************************************************************************************
	* PROGRAM      : Debugger
	* DATE - TIME  : lundi 31 mai 2008 - 18h04
	* AUTHOR       : Xiantia
	* FILENAME     : GdbBreakpoint
	* LICENSE      : GPL
	* COMMENTARY   : 
	********************************************************************************************************/

/*

		GdbBreakpoint class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#include "gdbBreakpoint.h"
//#include <QMessageBox>
#include <QFileInfo>
#include <QTextCodec>


GdbBreakpoint::GdbBreakpoint(QObject * parent) : GdbCore(parent)
{

	setEnabled(true);
	setWaitEndProcess(false);


	/*
		create new parser : For add breakpoint

		cRegExpCmd = "b main.cpp:12"
		aRegExp = "Breakpoint 1 at 0x437bdf: file src/addon/gdbBackTrace.cpp, line 15."
		or
		aRegExp = "Breakpoint 2 at 0x437d2b: file src/addon/gdbBackTrace.cpp, line 22. (2 locations)"
	*/

	interpreterAddBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^b\\s.*:\\d+$"),
		QRegExp("^Breakpoint\\s+(\\d+)\\s+at\\s(\\w+):\\s+file\\s+([^,]+),\\s+line\\s+(\\d+)\\.(|\\s+\\(\\d+\\s\\w*\\))"),
		"");

	// connect interpreter to function
	Connect.add(this, interpreterAddBreakpoint, &GdbBreakpoint::onBreakpointAdd);


	/*
		create new parser : For delete breakpoint

		cRegExpCmd = "delete 1"
		aRegExp = "(gdb) "
	*/
	interpreterDelBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^delete\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-delete\",answerGdb=\"");

	Connect.add(this, interpreterDelBreakpoint, &GdbBreakpoint::onBreakpointDelete);

	/*
		create new parser : For enable breakpoint

		cRegExpCmd = "enable 1"
		aRegExp = "(gdb) "
	*/
	interpreterEnabledBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^enable\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"");

	Connect.add(this, interpreterEnabledBreakpoint, &GdbBreakpoint::onBreakpointEnabled);

	/*
		create new parser : For disable breakpoint

		cRegExpCmd = "disable 1"
		aRegExp = "(gdb) "
	*/
	interpreterDisabledBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^disable\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"");

	Connect.add(this, interpreterDisabledBreakpoint, &GdbBreakpoint::onBreakpointDisabled);

	/*
		create new parser : For breakpoint pending

		cRegExpCmd = "disable 1"
		aRegExp = "(gdb) "
	*/
	interpreterBreakpointPending = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^b\\s.*:\\d+$"),
		QRegExp("^Breakpoint\\s(\\d+)\\s\\((.*):(\\d+)\\)\\spending\\.$"),
		"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Add-Pending\",answerGdb=\"");

	Connect.add(this, interpreterBreakpointPending, &GdbBreakpoint::onBreakpointPending);

	/*
		create new parser : For breakpoint Condition

		cRegExpCmd = "condition 1 i==2"
		aRegExp = "(gdb) "
	*/
	interpreterConditionnedBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^condition\\s\\d+.+$"),
		QRegExp("^\\(gdb\\)\\s"),
		"");

	Connect.add(this, interpreterConditionnedBreakpoint, &GdbBreakpoint::onBreakpointConditionned);

	/*
		create new parser : For breakpoint unCondition

		cRegExpCmd = "condition 1"
		aRegExp = "Breakpoint 1 now unconditional."
	*/
	interpreterUnConditionnedBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^condition\\s\\d+$"),
		QRegExp("^Breakpoint\\s\\d+\\snow\\sunconditional.$"),
		"");

	Connect.add(this, interpreterUnConditionnedBreakpoint, &GdbBreakpoint::onBreakpointUnConditionned);

	mWidget = UIGdbBreakpoint::self(0);
	connect( mWidget, SIGNAL(enabledBreakpoint(const QString &, const int &, const bool &)), this, SLOT(toggleEnabledBreakpoint(const QString &, const int &, const bool &)));
	connect( mWidget, SIGNAL(conditionnedBreakpoint(const QString &,const  int &, const QString &)), this, SLOT(toggleConditionnedBreakpoint(const QString &, const  int &, const QString &)));
}

//

GdbBreakpoint::~GdbBreakpoint()
{
	removeAllBreakpoint();
	delete mWidget;
}

//

QString GdbBreakpoint::name()
{
	return "GdbBreakpoint";
}

//

QPointer<QWidget> GdbBreakpoint::widget()
{
	return (QPointer<QWidget>)( mWidget );
}

//

QIcon GdbBreakpoint::icon()
{
	return QIcon(":/icons/breakpoint.png");
}

//

void GdbBreakpoint::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect.call( i, id, s);
}

//

Breakpoint * GdbBreakpoint::findByName(const QString & fileName)
{
	/*
		QFileInfo requier :
		bp->filame =  C:/.../../src/main.cpp
		filename = scr/main.cpp
	*/
	foreach(Breakpoint *bp , breakpointList)
		if(QFileInfo(bp->fileName).fileName() == QFileInfo(fileName).fileName() ) return bp;
	return NULL;
}

//

int   GdbBreakpoint::asBreakpointAtLine(Breakpoint *b, int line)
{
	int i=0;
	foreach(BaseBreakpoint bp, b->bp)
	{	
		if(bp.line == line)  return i;
		i++;
	}
	return -1;
}

//

int   GdbBreakpoint::asBreakpointIndex(Breakpoint *b, int index)
{
	int i=0;
	foreach(BaseBreakpoint bp, b->bp)
	{	
		if(bp.index == index)  return i;
		i++;
	}
	return -1;
}

//

void GdbBreakpoint::removeBreakpoint(Breakpoint * bp)
{
	int i = breakpointList.indexOf(bp);
	if(i != -1) 
	{
		delete breakpointList.at(i);
		breakpointList.removeAt(i);
	}
}

//

void GdbBreakpoint::removeAllBreakpoint()
{
	foreach(Breakpoint * bp, breakpointList)
		delete bp;
}

// Gdb status

void GdbBreakpoint::gdbStarted()
{
	removeAllBreakpoint();
	breakpointList.clear();
	setWaitEndProcess(false);
	desableBreakpointHit();
	mWidget->upDateData(breakpointList);
}

//

void GdbBreakpoint::gdbFinished()
{
	removeAllBreakpoint();
	breakpointList.clear();
	setWaitEndProcess(false);
	desableBreakpointHit();
	mWidget->upDateData(breakpointList);
}

//

void GdbBreakpoint::gdbError(){}

// Target status

void GdbBreakpoint::targetLoaded(const int &, const QString &){}
void GdbBreakpoint::targetNoLoaded(const int &, const QString &){}

void GdbBreakpoint::targetRunning(const int & , const QString & )
{
	desableBreakpointHit();
	mWidget->upDateData(breakpointList);
}

//

void GdbBreakpoint::targetStopped(const int & , const QString & s)
{
	/*
		reason : breakpoint-hit
		reason : end-stepping-range (step over or step into)
		Breakpoint 1, main (argc=1, argv=0xbfe6fc74) at src/main.cpp:21
		Breakpoint 1, main (argc=604801882, argv=0x8b240489) at src/main.cpp:13
	*/

	QString answer = findValue(s, "answerGdb");
	QRegExp r("^Breakpoint\\s+(\\d+),\\s*.*at\\s+(.*):(\\d+)");

	desableBreakpointHit();

	if(r.exactMatch(answer))
	{
		QStringList l = r.capturedTexts();
		breakpointMoved(l.at(2), l.at(3).toInt(), l.at(1).toInt());
	}
	mWidget->upDateData(breakpointList);
}

//

void GdbBreakpoint::targetExited(const int & , const QString & s)
{
	desableBreakpointHit();
	mWidget->upDateData(breakpointList);
}

// Parser status

void GdbBreakpoint::error(const int &, const QString & s)
{
	showMessage(name() + " have generate error : " + s, 5000, _CRITICAL_);
	mWidget->upDateData(breakpointList);
	setWaitEndProcess(false);
}

//

void GdbBreakpoint::done(const int &, const QString &)
{
}

void GdbBreakpoint::info(const int &, const QString &)
{
}

void GdbBreakpoint::prompt(const int &, const QString &)
{
	setWaitEndProcess(false);
}

// Interpreters

void GdbBreakpoint::desableBreakpointHit()
{
	foreach(Breakpoint *bp , breakpointList)
	{
		for(int i=0; i<bp->bp.count(); i++)
		{
			bp->bp[i].hit = false;
		}
	}

}

void GdbBreakpoint::breakpointMoved(const QString & fileName, const int & line, const int & index)
{
	Breakpoint * bp = findByName(fileName);
	if(bp)
	{
		for(int i=0; i< bp->bp.count() ; i++)
		{
			// move breakpoint under editor
			BaseBreakpoint b = bp->bp.at(i);
			// line under gdb as changed
			if(b.index == index && b.line != line)
			{
				int r  = asBreakpointAtLine(bp,line);
				if(r != -1) 
				{
					// clear to editor
					showMessage("Breakpoint deleted allready set." , 2500, _WARNING_);
					toggleBreakpoint(bp->fileName, b.line);
					bp->bp[r].hit = true;
				}
				else
				{
					showMessage("Breakpoint moved." , 2500, _WARNING_);
					// clear current breakpoint
					emit onToggleBreakpoint(*bp, b, false);
					bp->bp[i].line = line;
					// move breakpoint
					emit onToggleBreakpoint(*bp, bp->bp.at(i) , true);
				}
			}

			// check if breakpoint hit for show icon under TreeView
			if(b.index == index) bp->bp[i].hit = true;
		}
	}
}

// ================= Add /Del Breakpoint ===========================

void GdbBreakpoint::toggleBreakpoint(const QString & fileName, const int & line)
{
	
	if(isWaitEndProcess()) 
		return;

	Breakpoint *  bp = findByName(fileName);

	if(bp)
	{
		// file source exist
		// have already bp in this line ?
		int index  = asBreakpointAtLine(bp,line);
		if(index != -1)
		{
			GdbCore::Parser()->setNextCommand(name(), "delete " + QString::number(bp->bp.at(index).index));
			GdbCore::Parser()->changeAnswerInterpreter(interpreterDelBreakpoint, 
				"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Delete\",fileName=\""+ fileName +"\",line=\""+ QString::number(line) +"\",answerGdb=\"");
			GdbCore::Process()->sendRawData("delete " +  QString::number(bp->bp.at(index).index));

			setWaitEndProcess(true);
		}
		else // add
		{
			GdbCore::Parser()->setNextCommand(name(), "b " + fileName + ":" + QString::number(line));
			GdbCore::Parser()->changeAnswerInterpreter(interpreterAddBreakpoint, 
				"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Add\",fileName=\""+ fileName +"\",line=\""+ QString::number(line) +"\",answerGdb=\"");
			GdbCore::Process()->sendRawData("b \"" + fileName + "\":" + QString::number(line));

			setWaitEndProcess(true);
		}
	}
	else // pas de fichier source donc add bp
	{
		GdbCore::Parser()->setNextCommand(name(), "b " + fileName + ":" + QString::number(line));
		GdbCore::Parser()->changeAnswerInterpreter(interpreterAddBreakpoint, 
			"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Add\",fileName=\""+ fileName +"\",line=\""+ QString::number(line) +"\",answerGdb=\"");
		GdbCore::Process()->sendRawData("b \"" + fileName + "\":" + QString::number(line));

		setWaitEndProcess(true);
	}
}

//

void GdbBreakpoint::onBreakpointAdd( int , QString s)
{
	QString n = findValue(s,"fileName");

	if( !n.isEmpty() )
	{
		Breakpoint * bp = findByName(n);
		QString a =findValue(s, "answerGdb");
		QRegExp r = interpreterAddBreakpoint->getAnswerRegExp();
		//("^Breakpoint\\s+(\\d+)\\s+at\\s(\\w+):\\s+file\\s+([^,]+),\\s+line\\s+(\\d+)\\.(|\\s+\\(\\d+\\s\\w*\\))");

		if(bp)
		{
			// le fichier exist est a deja des Bp 
			
			if(r.exactMatch(a))
			{
				QStringList l = r.capturedTexts();
				BaseBreakpoint p;
				p.hit = false;
				p.line = findValue(s,"line").toInt();
				p.index = l.at(1).toInt();
				p.type = 1;
				p.enable = true;
				p.condition = tr("none");
				bp->bp << p;

				setWaitEndProcess(false);
				emit  onToggleBreakpoint(*bp, p, true);
			}
		}
		else
		{
			if(r.exactMatch(a))
			{
				QStringList l = r.capturedTexts();
				Breakpoint * b = new Breakpoint;
				b->fileName = n;
				BaseBreakpoint p;
				p.hit = false;
				p.line = findValue(s,"line").toInt();
				p.index = l.at(1).toInt();
				p.type = 1;
				p.enable = true;
				p.condition = tr("none");

				b->bp << p;
				breakpointList << b;

				setWaitEndProcess(false);
				emit  onToggleBreakpoint(*b, p, true );
			}
		}
		mWidget->upDateData(breakpointList);
	}
}

//

void GdbBreakpoint::onBreakpointDelete( int , QString s)
{
	QString n = findValue(s,"fileName");

	if( !n.isEmpty())
	{
		Breakpoint * bp = findByName(n);
		if(bp)
		{
			int index  = asBreakpointAtLine(bp,findValue(s,"line").toInt());
			if(index != -1)
			{
				emit  onToggleBreakpoint(*bp, bp->bp.at(index), false );
				bp->bp.removeAt(index);	
				setWaitEndProcess(false);
			}
			else
				showMessage("Critical erreur : Repport this bug please : Delete breakpoint but no have this line !" , 0, _CRITICAL_);
		}
		else
			showMessage("Critical erreur : Repport this bug please : Delete breakpoint but no have this file !" , 0, _CRITICAL_);
	
		if(bp && bp->bp.count() == 0)
			removeBreakpoint(bp);

		mWidget->upDateData(breakpointList);
	}
}

//

void GdbBreakpoint::onRequestBreakpoint(const QString & fileName)
{
	Breakpoint *bp = findByName(fileName);
	if(bp)
	{
		foreach(BaseBreakpoint b , bp->bp)
			emit onToggleBreakpoint(*bp, b, true);
	}
}

//=================== Enable / desable breakpoint =====================

void GdbBreakpoint::toggleEnabledBreakpoint(const QString & fileName, const int & index, const bool &b)
{
	if(isWaitEndProcess())
		return;
	
	if(b)
	{
		GdbCore::Parser()->changeAnswerInterpreter(interpreterEnabledBreakpoint, 
			"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Enabled\",fileName=\""+ fileName +"\",index=\""+ QString::number(index) +"\",enabled=\"true\",answerGdb=\"");
		GdbCore::Parser()->setNextCommand(name(), "enable " + QString::number(index));
		GdbCore::Process()->sendRawData("enable " + QString::number(index));
		setWaitEndProcess(true);
	}
	else
	{
		GdbCore::Parser()->changeAnswerInterpreter(interpreterDisabledBreakpoint, 
			"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Disable\",fileName=\""+ fileName +"\",index=\""+ QString::number(index) +"\",enabled=\"false\",answerGdb=\"");
		GdbCore::Parser()->setNextCommand(name() ,"disable " + QString::number(index));
		GdbCore::Process()->sendRawData("disable " + QString::number(index));
		setWaitEndProcess(true);
	}
}

//

void GdbBreakpoint::onBreakpointEnabled(int, QString s)
{
	QString n = findValue(s,"fileName");

	if( !n.isEmpty() )
	{
		Breakpoint * bp = findByName(n);
		if(bp)
		{
			int index  = asBreakpointIndex(bp,findValue(s,"index").toInt());
			if(index != -1)
			{
				bp->bp[index].enable = true;
//				emit onToggleBreakpointEnabled(*bp, bp->bp.at(index) );
				emit  onToggleBreakpoint(*bp, bp->bp.at(index), true );
				setWaitEndProcess(false);
			}
		
		}
	}
}

//

void GdbBreakpoint::onBreakpointDisabled(int, QString s)
{
	QString n = findValue(s,"fileName");

	if( !n.isEmpty() )
	{
		Breakpoint * bp = findByName(n);
		if(bp)
		{
			int index  = asBreakpointIndex(bp,findValue(s,"index").toInt());
			if(index != -1)
			{
				bp->bp[index].enable = false;
				emit  onToggleBreakpoint(*bp, bp->bp.at(index), true);

//				emit onToggleBreakpointEnabled(*bp, bp->bp.at(index));
				setWaitEndProcess(false);
			}
		}
	}
}

// ================================ Breakpoint pending =================

void GdbBreakpoint::onBreakpointPending(int, QString s)
{
	QString n = findValue(s,"answerGdb");

	if( !n.isEmpty() )
	{
		//("^Breakpoint\\s+(\\d+)\\s+at\\s(\\w+):\\s+file\\s+([^,]+),\\s+line\\s+(\\d+)\\.(|\\s+\\(\\d+\\s\\w*\\))");
		QRegExp r = interpreterBreakpointPending->getAnswerRegExp();
		if(r.exactMatch(n))
		{
			QStringList l = r.capturedTexts();
			onBreakpointAdd(0,"fileName=\"" + l.at(2) + "\",line=\"" + l.at(3) + "\",answerGdb=\"Breakpoint " + l.at(1) + " at pending: file " + l.at(2) + ", line " + l.at(3) + ".");
		}
	}
}

// =============================== Breakpoint Conditionned ================
/*
(gdb) condition 1
Breakpoint 1 now unconditional.
(gdb) condition 1 qsdqsdqd
No symbol "qsdqsdqd" in current context.
(gdb) condition 1 3423FFSDF
Invalid number "3423FFSDF".
(gdb) condition 1 '""
Unmatched single quote.
(gdb)
*/

void GdbBreakpoint::toggleConditionnedBreakpoint(const QString & fileName, const int & index, const QString & condition)
{
	if(isWaitEndProcess())
		return;

	GdbCore::Parser()->changeAnswerInterpreter(interpreterConditionnedBreakpoint, 
		"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Conditionned\",fileName=\""+ fileName +"\",index=\""+ QString::number(index) +"\",condition=\"" + condition + "\",answerGdb=\"");
	GdbCore::Parser()->changeAnswerInterpreter(interpreterUnConditionnedBreakpoint, 
		"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-unConditionned\",fileName=\""+ fileName +"\",index=\""+ QString::number(index) +"\",condition=\"\",answerGdb=\"");

	if(condition.isEmpty() || condition == tr("none"))
	{
		GdbCore::Parser()->setNextCommand(name() ,"condition " + QString::number(index));
		GdbCore::Process()->sendRawData("condition " + QString::number(index));
	}
	else
	{
		GdbCore::Parser()->setNextCommand(name() ,"condition " + QString::number(index) + " " + condition);
		GdbCore::Process()->sendRawData("condition " + QString::number(index) + " " + condition);
	}
	setWaitEndProcess(true);
}

//

void GdbBreakpoint::onBreakpointConditionned(int, QString s)
{
	QString n = findValue(s,"fileName");

	if( !n.isEmpty() )
	{
		Breakpoint * bp = findByName(n);
		if(bp)
		{
			int index  = asBreakpointIndex(bp,findValue(s,"index").toInt());
			if(index != -1)
			{
				QString c = findValue(s,"condition");
				bp->bp[index].condition = c.isEmpty() ? tr("none") : c;
				bp->bp[index].type = 2;
//				emit onToggleBreakpointConditionned(*bp, bp->bp.at(index));
				emit  onToggleBreakpoint(*bp, bp->bp.at(index), true );
				setWaitEndProcess(false);
				mWidget->upDateData(breakpointList);
			}
		}
	}
}

//

void GdbBreakpoint::onBreakpointUnConditionned(int, QString s)
{
	QString n = findValue(s,"fileName");

	if( !n.isEmpty() )
	{
		Breakpoint * bp = findByName(n);
		if(bp)
		{
			int index  = asBreakpointIndex(bp,findValue(s,"index").toInt());
			if(index != -1)
			{
				QString c = findValue(s,"condition");
				bp->bp[index].condition = c.isEmpty() ? tr("none") : c;
				bp->bp[index].type = 1;
//				emit onToggleBreakpointConditionned(*bp, bp->bp.at(index));
				emit  onToggleBreakpoint(*bp, bp->bp.at(index), true );
				setWaitEndProcess(false);
				mWidget->upDateData(breakpointList);
			}
		}
	}
}
