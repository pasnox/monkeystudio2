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
#include <QMessageBox>
#include <QFileInfo>
#include <QTextCodec>


GdbBreakpoint::GdbBreakpoint(QObject * parent) : GdbCore(parent)
{
	// new connexion for auto-call onBreakpointAdd()
	Connect = new GdbConnectTemplate<GdbBreakpoint>;

	setEnabled(true);
	setWaitEndProcess(false);


	/*
		create new parser :

		cRegExpCmd = "b main.cpp:12"
		aRegExp = "Breakpoint 1 at 0x437bdf: file src/addon/gdbBackTrace.cpp, line 15."
		or
		aRegExp = "Breakpoint 2 at 0x437d2b: file src/addon/gdbBackTrace.cpp, line 22. (2 locations)"
	*/

	interpreterAddBreakpoint = GdbCore::Parser()->addInterpreter(
		//"Breakpoint-add",
		//"b",
		QRegExp("^b\\s.*:\\d+$"),
		QRegExp("^Breakpoint\\s+\\d+\\s+at\\s\\w+:\\s+file\\s+[^,]+,\\s+line\\s+\\d+\\.(|\\s+\\(\\d+\\s\\w*\\))"),
		"");

	// connect interpreter to function
	Connect->add(this, interpreterAddBreakpoint, &GdbBreakpoint::onBreakpointAdd);


	/*
		create new parser :

		cRegExpCmd = "delete 1"
		aRegExp = "(gdb) "
	*/
	interpreterDelBreakpoint = GdbCore::Parser()->addInterpreter(
		//"Breakpoint-delete",
		//"delete ",
		QRegExp("^delete\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-delete\",answerGdb=\"");

	Connect->add(this, interpreterDelBreakpoint, &GdbBreakpoint::onBreakpointDelete);

	/*
		create new parser :

		cRegExpCmd = "enable 1"
		aRegExp = "(gdb) "
	*/
	interpreterEnabledBreakpoint = GdbCore::Parser()->addInterpreter(
		//"Breakpoint-Enabled",
		//"enable",
		QRegExp("^enable\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"");

	Connect->add(this, interpreterEnabledBreakpoint, &GdbBreakpoint::onBreakpointEnabled);

	/*
		create new parser :

		cRegExpCmd = "disable 1"
		aRegExp = "(gdb) "
	*/
	interpreterDisabledBreakpoint = GdbCore::Parser()->addInterpreter(
		//"Breakpoint-Disable",
		//"disable",
		QRegExp("^disable\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"");

	Connect->add(this, interpreterDisabledBreakpoint, &GdbBreakpoint::onBreakpointDisabled);

	mWidget = UIGdbBreakpoint::self();
	connect( mWidget, SIGNAL(enabledBreakpoint(QString, int, bool)), this, SLOT(toggleEnabledBreakpoint(QString, int, bool)));
}

//

GdbBreakpoint::~GdbBreakpoint()
{
	removeAllBreakpoint();
	delete Connect;
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

void GdbBreakpoint::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect->call( i, id, s);
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

void GdbBreakpoint::gdbStarted()
{
	removeAllBreakpoint();
	breakpointList.clear();
	setWaitEndProcess(false);
	desableBreakpointHit();
	mWidget->upDateData(breakpointList);
}

//

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

//

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
					// clear to editor
					toggleBreakpoint(bp->fileName, b.line);
				else
				{
						// clear current breakpoint
//						emit onToggleBreakpoint(bp->fileName, b.line, false);
						emit onToggleBreakpoint(*bp, b, false);
						bp->bp[i].line = line;
						// move breakpoint
//						emit onToggleBreakpoint(bp->fileName, bp->bp.at(i).line, true);
						emit onToggleBreakpoint(*bp, bp->bp.at(i) , true);
				}
			}

			// check if breakpoint hit fior show icon under TreeView
			if(b.index == index) bp->bp[i].hit = true;
			else bp->bp[i].hit = false;
		
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
			GdbCore::Parser()->setNextCommand("delete " + QString::number(bp->bp.at(index).index));
			GdbCore::Parser()->changeAnswerInterpreter(interpreterDelBreakpoint, 
				"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Delete\",fileName=\""+ fileName +"\",line=\""+ QString::number(line) +"\",answerGdb=\"");
			GdbCore::Process()->sendRawData("delete " +  QString::number(bp->bp.at(index).index));

			setWaitEndProcess(true);
		}
		else // add
		{
			GdbCore::Parser()->setNextCommand("b " + fileName + ":" + QString::number(line));
			GdbCore::Parser()->changeAnswerInterpreter(interpreterAddBreakpoint, 
				"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Add\",fileName=\""+ fileName +"\",line=\""+ QString::number(line) +"\",answerGdb=\"");
			GdbCore::Process()->sendRawData("b \"" + fileName + "\":" + QString::number(line));

			setWaitEndProcess(true);
		}
	}
	else // pas de fichier source donc add bp
	{
		GdbCore::Parser()->setNextCommand("b " + fileName + ":" + QString::number(line));
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
		QRegExp r("^Breakpoint\\s+(\\d+)\\s+at\\s(\\w+):\\s+file\\s+([^,]+),\\s+line\\s+(\\d+)\\.(|\\s+\\(\\d+\\s\\w*\\))");

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
				p.condition = "No implemented";
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
				p.condition = "No implemented";

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
				QMessageBox::warning(NULL,"Critical erreur","Repport this bug : Delete breakpoint but no have this line !");
		}
		else
			QMessageBox::warning(NULL,"Critical erreur","Repport this bug : Delete breakpoint but no have this file !");
	
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

void GdbBreakpoint::toggleEnabledBreakpoint(QString fileName, int index, bool b)
{
	if(b)
	{
		GdbCore::Parser()->changeAnswerInterpreter(interpreterEnabledBreakpoint, 
			"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Enabled\",fileName=\""+ fileName +"\",index=\""+ QString::number(index) +"\",enabled=\"true\",answerGdb=\"");
		GdbCore::Parser()->setNextCommand("enable " + QString::number(index));
		GdbCore::Process()->sendRawData("enable " + QString::number(index));
	}
	else
	{
		GdbCore::Parser()->changeAnswerInterpreter(interpreterDisabledBreakpoint, 
			"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Disable\",fileName=\""+ fileName +"\",index=\""+ QString::number(index) +"\",enabled=\"false\",answerGdb=\"");
		GdbCore::Parser()->setNextCommand("disable " + QString::number(index));
		GdbCore::Process()->sendRawData("disable " + QString::number(index));
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
				emit onToggleBreakpointEnabled(*bp, bp->bp.at(index) );
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
				emit onToggleBreakpointEnabled(*bp, bp->bp.at(index));
			}
		
		}
	}
}
