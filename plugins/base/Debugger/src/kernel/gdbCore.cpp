#include "./kernel/gdbCore.h"
#include <QMetaType>


// one mutex for all class
static QMutex *mutexProcess = new QMutex();


GdbCore::GdbCore( GdbParser *p) : QThread(),
		bTargetLoaded(false), bTargetRunning(false),
		bTargetStopped(true), bTargetExited(true),
		bGdbStarted(false), bGdbFinished(true),
		mParser(NULL)
{
	qRegisterMetaType<QGdbMessageCore>( "QGdbMessageCore" );

// debuggin
	file.setFileName("./GdbCore_Thread_Main.txt");
	file.open(QIODevice::WriteOnly);

	statusProcess = PROCESS_TERMINED;
	// set parset
	mParser = p;
	// create QDockWidget main container
	mContainer = new QDockWidget();
	// connect 
	connect(this, SIGNAL( readyProcess(QGdbMessageCore)),this, SLOT(onReadyProcess(QGdbMessageCore)), Qt::BlockingQueuedConnection);
	// start checking message Queue
	start();
}
//
GdbCore::~GdbCore()
{
	terminate();
	delete mContainer;
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
// checking new message queue
void GdbCore::run()
{
	while(true)
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
file.write("1 1 have msg\r\n");
file.flush();
					// lock processing
					mutexEndProcess.lock();
file.write("2 2 emit\r\n");
file.flush();
					emit readyProcess(inBox);
file.write("3 5 emit effectued\r\n");
file.flush();
					// wait until readyprocess is no finish					
//#ifdef Q_OW_WIN	
		//		condition.wait(&mutexEndProcess);
//#endif
//						statusProcess = process(inBox);
file.write("6 6 exit\r\n");
file.flush();
					mutexEndProcess.unlock();
				}
			}
			usleep(100);
		}
		while(statusProcess != PROCESS_TERMINED);
file.write("leave handle\r\n");
file.flush();
		mutexProcess->unlock();
		usleep(100);
	}
}
//
void GdbCore::onReadyProcess(QGdbMessageCore inBox)
{
file.write("4 3 process...\r\n");
file.flush();

	statusProcess = process(inBox);
file.write("5 4 termined and wakeOne\r\n");
file.flush();

//#ifdef Q_OS_WIN
//	condition.wakeOne();
//#endif
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