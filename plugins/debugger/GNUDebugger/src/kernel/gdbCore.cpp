#include "gdbCore.h"
#include <QMetaType>

#define TIME_TICK	50000

// one mutex for all class
static QMutex *mutexProcess = new QMutex();


GdbCore::GdbCore( GdbParser *p) : QThread(),
		bTargetLoaded(false), bTargetRunning(false),
		bTargetStopped(true), bTargetExited(true),
		bGdbStarted(false), bGdbFinished(true),
		mParser(NULL),stopProcess(false)
{
	qRegisterMetaType<QGdbMessageCore>( "QGdbMessageCore" );


	statusProcess = PROCESS_TERMINED;
	// set parset
	mParser = p;
	// create QDockWidget main container
//	mContainer = new QDockWidget();
//	mContainer = new QWidget();
	connect(this, SIGNAL( readyProcess(QGdbMessageCore)),this, SLOT(onReadyProcess(QGdbMessageCore)), Qt::BlockingQueuedConnection);
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
	// thread finished
}
//
void GdbCore::setStopProcess()
{
	stopProcess = true;
	//terminate();

	if(!wait(1000)) // wait 1 seconde
	{
//		file.write("ERREUR THREAD NOT TERMINED\r\n");
//		file.flush();
			QMessageBox::warning(NULL,"Information","Thread  " + name() + " not termined correctly.");
	}
	else
	{
//		file.write("TERMINED THREAD\r\n");
//		file.flush();
	}
//	file.close();
}
//
// checking new message queue
void GdbCore::run()
{
//	file.setFileName("./plugins/GNUdbg/log/Thread_Core_" + name() + ".txt");
//	file.open(QIODevice::WriteOnly);

	while(!stopProcess)
	{
		if(mutexProcess->tryLock())
//		mutexProcess->lock();
		{
			do
			{
				QGdbMessageCore inBox = getGdbMessage();
				if(inBox.id != MESSAGE_EMPTY) 
				{
					if(isGdbStarted() && isTargetLoaded() )
					{	
//file.write("u 1 : w 1 have msg\r\n");
//file.write("u 2 : w 2 emit\r\n");
//file.flush();
					do{	
						emit readyProcess(inBox);
						inBox = getGdbMessage();
					}while(inBox.id != MESSAGE_EMPTY);
					
//file.write("u 3 : w 5 emit effectued\r\n");
//file.write("u 6 : w 6 exit\r\n");
//file.flush();
					}
				}
				else usleep(TIME_TICK);
			}
			while(statusProcess != PROCESS_TERMINED);
			mutexProcess->unlock();
		}
		usleep(TIME_TICK);
	}
//	file.write("EXITED THREAD ... \r\n");
//	file.flush();

	exit();
}
//
void GdbCore::onReadyProcess(QGdbMessageCore inBox)
{
//file.write("u 4 : w 3 process...\r\n");
//file.flush();

	statusProcess = process(inBox);

//file.write("u 5 : w 4 termined and wakeOne\r\n");
//file.flush();
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
