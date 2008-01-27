#include "./kernel/gdbBase.h"


GdbBase::GdbBase(QWidget *o) 
	: QDockWidget(o),				// on herite d'un dock
	 QGdbMessageQueu(),			
	 QGdbInterpreter()
{
	currentInterpreter = -1;
	Parser = NULL;

//	gdb_base_message::postGdbMessage(2,"");

}
//
GdbBase::~GdbBase()
{
}
//
void GdbBase::setParser(GdbParser *p)
{
	Parser = p;
}
//
/*void GdbBasePlugin::setEnableWatchDog(bool b)
{
	bWatchDog = b;
}*/
//
void GdbBase::addInterpreter(QBaseInterpreter i)
{
	mLastCmd = i.cmdRegExp;
	if(Parser )
	{
			currentInterpreter =   Parser->addInterpreter(name(),
	i.cmdRegExp.pattern(),
				i.cmdRegExp, 
				i.answerRegExp,
				 "^info,interpreter=\"" + name()  +  "\",event=\"" + i.cmdName + "\",answerExtention={" + i.answerExtention + "},answerGdb=\""); 
	}
	else qDebug("(Class GdbBasePlugin function addInterpreter) WARNING Parser pointer is NULL !");
}
//
void GdbBase::addInterpreter(QGdbInterpreter i)
{
	mLastCmd = i.cmdRegExp;
	if(Parser )
	{
			currentInterpreter =   Parser->addInterpreter(name(),
		i.cmdGdb,
		i.cmdRegExp, 
				i.answerRegExp,
				 "^info,interpreter=\"" + name()  +  "\",event=\"" + i.cmdName + "\",answerExtention={" + i.answerExtention + "},answerGdb=\""); 
	}
	else qDebug("(Class GdbBasePlugin function addInterpreter) WARNING Parser pointer is NULL !");
}
//
void GdbBase::removeInterpreter()
{
	if(Parser)
		currentInterpreter = Parser->removeInterpreter(currentInterpreter);
	else qDebug("(Class GdbBasePlugin function removeInterpreter) WARNING Parser pointer is NULL !");
}
//
//
void GdbBase::processPostMessage(int id, QByteArray data)
{
	emit sendMessage(id, "^notify,interpreter=\"none\",currentCmd=\"none\"," + data);
}
//
QList<QByteArray> GdbBase::extractBlock(QByteArray line)
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
				while(line.at(i) != '"') block.append(line.at(i++));
				blockList << block;
//qDebug(block);
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
				while(count!=-1);
				blockList << block;
//qDebug(block);
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
				while(count!=-1);
				blockList << block;
//qDebug(block);
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
QByteArray GdbBase::getParametre(QByteArray param, QByteArray line)
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

