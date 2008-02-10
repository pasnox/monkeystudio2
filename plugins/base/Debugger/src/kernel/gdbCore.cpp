#include "./kernel/gdbCore.h"
#include <QMetaType>

#define TIME_TICK	100

// one mutex for all class
static QMutex *mutexProcess = new QMutex();


GdbCore::GdbCore( GdbParser *p) : QThread(),
		bTargetLoaded(false), bTargetRunning(false),
		bTargetStopped(true), bTargetExited(true),
		bGdbStarted(false), bGdbFinished(true),
		mParser(NULL),stopProcess(false)
{
	qRegisterMetaType<QGdbMessageCore>( "QGdbMessageCore" );


//	stopProcess = false;
	statusProcess = PROCESS_TERMINED;
	// set parset
	mParser = p;
	// create QDockWidget main container
	mContainer = new QDockWidget();

	// connect 

//	connect(this ,SIGNAL(finished ( ) ), this , SLOT(onFiniched(  )));
//	connect(this ,SIGNAL(terminated ( ) ), this , SLOT(onFiniched(  )));

// Qt::DirectConnection
// ou
// Qt::BlockingQueuedConnection


#ifdef Q_OS_WIN
qDebug("connection windows");
	connect(this, SIGNAL( readyProcess(QGdbMessageCore)),this, SLOT(onReadyProcess(QGdbMessageCore)), Qt::BlockingQueuedConnection);
#endif
#ifdef Q_OS_UNIX
qDebug("connection unix");
	connect(this, SIGNAL( readyProcess(QGdbMessageCore)),this, SLOT(onReadyProcess(QGdbMessageCore)));
#endif	

	// start checking message Queue
	start();
}
//
GdbCore::~GdbCore()
{
}
//
void GdbCore::addInterpreter(QGdbInterpreter i)
{
	// save the current command
	mLastCmd = i.cmdRegExp();
	if( mParser )
	{
			mCurrentInterpreterIndex =   mParser->addInterpreter( name(),
				i.cmdGdb(),
				i.cmdRegExp(), 
				i.answerRegExp(),
				 "^info,interpreter=\"" + name()  +  "\",event=\"" + i.cmdName() + "\",answerExtention={" + i.answerExtention() + "},answerGdb=\""); 
	}
	else QMessageBox::warning(NULL, "Class GdbCore function addInterpreter", "WARNING Parser pointer is NULL !", QMessageBox::Ok );
}
//
void GdbCore::removeInterpreter()
{
	if(mParser)
	{
		if(mCurrentInterpreterIndex != INVALID_INTERPRETER) 
				mCurrentInterpreterIndex = mParser->removeInterpreter(mCurrentInterpreterIndex);
	}
	else QMessageBox::warning(NULL, "Class GdbCore function removeInterpreter", "WARNING Parser pointer is NULL !", QMessageBox::Ok );
}
//
// process want send data to all other process
void GdbCore::processPostMessage(int id, QByteArray data)
{
	emit sendPostMessage(id, "^notify,interpreter=\"none\",currentCmd=\"none\"," + data);
}
//
// process want send data to gdb
void GdbCore::processSendRawData(GdbCore *p, QByteArray data)
{
	emit sendRawData(p, data);
}
//
void GdbCore::onFinished( )
{
//	QMessageBox::warning(NULL,"termined","thread exit " + name());
}
//
void GdbCore::setStopProcess()
{
	stopProcess = true;
	terminate();

	if(!wait(5))
	{
		file.write("ERREUR THREAD NOT TERMINED\r\n");
		file.flush();
			QMessageBox::warning(NULL,"Information","Thread  " + name() + " not termined.");
	}
	else
	{	file.write("TERMINED THREAD\r\n");
		file.flush();
	}
	file.close();
}
//
// checking new message queue
void GdbCore::run()
{
	file.setFileName("./Thread_Core_" + name() + ".txt");
	file.open(QIODevice::WriteOnly);

	while(!stopProcess)
	{
		mutexProcess->lock();
		do
		{
file.write("have handle : "  + name().toLocal8Bit() + "\r\n");
file.flush();
			QGdbMessageCore inBox = getGdbMessage();
			if(inBox.id != MESSAGE_EMPTY) 
			{
				if(isGdbStarted() && isTargetLoaded() )
				{
file.write("u 1 : w 1 have msg\r\n");
file.flush();
					// lock processing
					mutexEndProcess.lock();
file.write("u 2 : w 2 emit\r\n");
file.flush();
					emit readyProcess(inBox);
file.write("u 3 : w 5 emit effectued\r\n");
file.flush();
					// wait until readyprocess is no finish
#ifdef Q_OS_UNIX	
//qDebug("wait unix");
					condition.wait(&mutexEndProcess);
#endif
//						statusProcess = process(inBox);
file.write("u 6 : w 6 exit\r\n");
file.flush();
					mutexEndProcess.unlock();
				}
			}
			usleep(TIME_TICK);
		}
		while(statusProcess != PROCESS_TERMINED);
file.write("leave handle\r\n");
file.flush();
		mutexProcess->unlock();
		usleep(TIME_TICK);
	}

	exit();

}
//
void GdbCore::onReadyProcess(QGdbMessageCore inBox)
{
file.write("u 4 : w 3 process...\r\n");
file.flush();

	statusProcess = process(inBox);
file.write("u 5 : w 4 termined and wakeOne\r\n");
file.flush();

#ifdef Q_OS_UNIX
//	qDebug("wakeOn unix");
	condition.wakeOne();
#endif
}

void GdbCore::postGdbMessage(QGdbMessageCore m)
{
		mutexMessage.lock();
		mMsgQueue << m;
		mutexMessage.unlock();
}
//
QGdbMessageCore GdbCore::getGdbMessage()
{
	mutexMessage.lock();
	QGdbMessageCore m;
	m.id = MESSAGE_EMPTY;

	if(mMsgQueue.count())
	{
		m = mMsgQueue.at(0);
		mMsgQueue.removeAt(0);
	 }
	mutexMessage.unlock();
	return m;
}
//
QList<QByteArray> GdbCore::extractBlock(QByteArray line)
{
	QList<QByteArray> blockList;
	QByteArray block;
	for(int i=0; i <line.length(); i++)
	{
		switch(line.at(i))
		{
			case ',' :  blockList << block; block.clear() ; break;
			case '"' : // forme reason="breakpoint-hit",
			{
				i++; // saute le premier "
				while(line.at(i) != '"' && i < line.size()) block.append(line.at(i++));
				blockList << block;
				block.clear();
				break;
			}
			case '{' : //  frame={addr="0x08048564", func="main"}	
			{
				i++; // saute le premier {
				int count=0;
				do
				{
					if(line.at(i) == '{') count++;
					else if(line.at(i) == '}') count--;
						else block.append(line.at(i));
					i++;
				}	
				while(count!=-1 && i<line.size());
				blockList << block;
				block.clear();
				break;
			}
			case '[' : // 	args=[{name="argc",value="1"},{name="argv",value="0xbfc4d4d4"}]
			{
				i++; // saute le premier [
				int count=0;
				do
				{
					if(line.at(i) == '[') count++;
					else if(line.at(i) == ']') count--;
							else block.append(line.at(i));
					i++;
				}	
				while(count!=-1 && i<line.size());
				blockList << block;
				block.clear();
				break;
			}
			default :
				block.append(line.at(i));
		}// end swtich
	}// end for
	return blockList;
}
// test ok
QByteArray GdbCore::getParametre(QByteArray param, QByteArray line)
{
	QList<QByteArray > listParam = extractBlock(line);

	for(int i=0; i< listParam.count(); i++)
	{
		if(	listParam.at(i).startsWith(param ))
		{
			QByteArray arg = listParam.at(i).right(  listParam.at(i).length() - listParam.at(i).indexOf('=') - 1);
			return arg;
		}
	}
	return QByteArray();
}
