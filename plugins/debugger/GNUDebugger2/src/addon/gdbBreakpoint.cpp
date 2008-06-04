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


GdbBreakpoint::GdbBreakpoint(QObject * parent, QPointer<GdbParser> pa , QPointer<GdbProcess> pr) : GdbCore(parent, pa, pr)
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

	interpreterAddBreakpoint = GdbCore::Parser()->addInterpreter("Breakpoint-add",
		"b",
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
	interpreterDelBreakpoint = GdbCore::Parser()->addInterpreter("Breakpoint-delete",
		"delete ",
		QRegExp("^delete\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-delete\",answerGdb=\"");

	Connect->add(this, interpreterDelBreakpoint, &GdbBreakpoint::onBreakpointDelete);

	mWidget = new QTextEdit();
}


GdbBreakpoint::~GdbBreakpoint()
{
	removeAllBreakpoint();
	delete Connect;
	delete mWidget;
}

QString GdbBreakpoint::name()
{
	return "GdbBreakpoint";
}


QPointer<QWidget> GdbBreakpoint::widget()
{
	return (QPointer<QWidget>)( mWidget );
}

void GdbBreakpoint::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect->call( i, id, s);
}


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

void GdbBreakpoint::removeBreakpoint(Breakpoint * bp)
{
	int i = breakpointList.indexOf(bp);
	if(i != -1) 
	{
		delete breakpointList.at(i);
		breakpointList.removeAt(i);
	}
}

void GdbBreakpoint::removeAllBreakpoint()
{
	foreach(Breakpoint * bp, breakpointList)
		delete bp;
}

void GdbBreakpoint::gdbFinished()
{
	removeAllBreakpoint();
	breakpointList.clear();
	setWaitEndProcess(false);
}

void GdbBreakpoint::gdbStarted()
{
	removeAllBreakpoint();
	breakpointList.clear();
	setWaitEndProcess(false);
}

void GdbBreakpoint::targetRunning(const int & , const QString & )
{
}

void GdbBreakpoint::targetStopped(const int & , const QString & s)
{
	/*
		reason : breakpoint-hit
		reason : end-stepping-range (step over or step into)
		Breakpoint 1, main (argc=1, argv=0xbfe6fc74) at src/main.cpp:21
		Breakpoint 1, main (argc=604801882, argv=0x8b240489) at src/main.cpp:13
	*/

	mWidget->append("*** target stopped ***");

	QString answer = findValue(s, "answerGdb");
	QRegExp r("^Breakpoint\\s+(\\d+),\\s*.*at\\s+(.*):(\\d+)");

	if(r.exactMatch(answer))
	{
		QStringList l = r.capturedTexts();
		breakpointMoved(l.at(2), l.at(3).toInt(), l.at(1).toInt());
	}
}


void GdbBreakpoint::breakpointMoved(const QString & fileName, const int & line, const int & index)
{
	Breakpoint * bp = findByName(fileName);
	if(bp)
	{

		for(int i=0; i< bp->bp.count() ; i++)
		{
			BaseBreakpoint b = bp->bp.at(i);
			// line under gdb as changed
			if(b.index == index && b.line != line)
			{
				int r  = asBreakpointAtLine(bp,line);
				if(r != -1) 
				{
					// clear to editor
					toggleBreakpoint(bp->fileName, b.line);
					
//					QMessageBox::warning(NULL,"breakpoint","break deja present");
				}
				else
				{
						mWidget->append("Gdb as moved breakpoint " + QString::number(b.line) + " to " + QString::number(line));

						// clear current breakpoint
						emit onToggleBreakpoint(bp->fileName, b.line, false);
						bp->bp[i].line = line;
						// move breakpoint
						emit onToggleBreakpoint(bp->fileName, bp->bp.at(i).line, true);
				}
			}
		}
	}
}

//main function
void GdbBreakpoint::toggleBreakpoint(const QString & fileName, const int & line)
{
	
	if(isWaitEndProcess()) 
		return;

	Breakpoint *  bp = findByName(fileName);

	mWidget->append("*** user toggle breakpoint ***");
	
	if(bp)
	{
		mWidget->append("file found");
		// file source exist
		// have already bp in this line ?
		int index  = asBreakpointAtLine(bp,line);
		if(index != -1)
		{
			mWidget->append("line found (delete)");
		
			GdbCore::Parser()->setNextCommand("delete " + QString::number(bp->bp.at(index).index));
			GdbCore::Parser()->changeAnswerInterpreter(interpreterDelBreakpoint, 
				"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Delete\",fileName=\""+ fileName +"\",line=\""+ QString::number(line) +"\",answerGdb=\"");
			GdbCore::Process()->sendRawData("delete " +  QByteArray::number(bp->bp.at(index).index));

			mWidget->append("send -> delete " +  QString::number(bp->bp.at(index).index) );
			setWaitEndProcess(true);
		}
		else // add
		{
			mWidget->append("line no found (add)");
		
			GdbCore::Parser()->setNextCommand("b " + fileName + ":" + QString::number(line));
			GdbCore::Parser()->changeAnswerInterpreter(interpreterAddBreakpoint, 
				"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Add\",fileName=\""+ fileName +"\",line=\""+ QString::number(line) +"\",answerGdb=\"");
			GdbCore::Process()->sendRawData("b " + fileName.toLocal8Bit() + ":" + QByteArray::number(line));

			mWidget->append("send -> b " + fileName + ":" + QString::number(line) );
			setWaitEndProcess(true);
		}
	}
	else // pas de fichier source donc add bp
	{
		mWidget->append("file no found (add first)");
		
		GdbCore::Parser()->setNextCommand("b " + fileName + ":" + QString::number(line));
		GdbCore::Parser()->changeAnswerInterpreter(interpreterAddBreakpoint, 
			"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Add\",fileName=\""+ fileName +"\",line=\""+ QString::number(line) +"\",answerGdb=\"");
		GdbCore::Process()->sendRawData("b " + fileName.toLocal8Bit() + ":" + QByteArray::number(line));

		mWidget->append("send -> b " + fileName + ":" + QString::number(line) );

		setWaitEndProcess(true);
	}
}



void GdbBreakpoint::onBreakpointAdd( int , QString s)
{

	mWidget->append("gdb -> " + s);

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
				p.line = findValue(s,"line").toInt();
				p.index = l.at(1).toInt();
				p.type = 0;
				p.enable = 1;
				bp->bp << p;

				setWaitEndProcess(false);
				emit  onToggleBreakpoint(bp->fileName, p.line, true );

				mWidget->append("-> breakpoint add sucess full");

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
				p.line = findValue(s,"line").toInt();
				p.index = l.at(1).toInt();
				p.type = 0;
				p.enable = 1;
				b->bp << p;
				breakpointList << b;

				setWaitEndProcess(false);
				emit  onToggleBreakpoint(b->fileName, p.line, true );

				mWidget->append("-> breakpoint add sucess full");
			}
		}
	}
}


void GdbBreakpoint::onBreakpointDelete( int , QString s)
{
	mWidget->append("gdb -> " + s);
	QString n = findValue(s,"fileName");

	if( !n.isEmpty())
	{
		Breakpoint * bp = findByName(n);
		if(bp)
		{
			int index  = asBreakpointAtLine(bp,findValue(s,"line").toInt());
			if(index != -1)
			{
				emit  onToggleBreakpoint(bp->fileName, bp->bp.at(index).line, false );
				bp->bp.removeAt(index);	
				setWaitEndProcess(false);

				mWidget->append("-> breakpoint deleted sucess full");
			}
			else
				QMessageBox::warning(NULL,"Critical erreur","Repport this bug : Delete breakpoint but no have this line !");
		}
		else
			QMessageBox::warning(NULL,"Critical erreur","Repport this bug : Delete breakpoint but no have this file !");
	
		if(bp && bp->bp.count() == 0)
			removeBreakpoint(bp);
	}
}


void GdbBreakpoint::onRequestBreakpoint(const QString & fileName)
{
	mWidget->append("*** Request breakpoint for : " + fileName);

	Breakpoint *bp = findByName(fileName);
	if(bp)
	{
		foreach(BaseBreakpoint b , bp->bp)
		{
			emit onToggleBreakpoint(bp->fileName, b.line, true);
		}
	}
}
