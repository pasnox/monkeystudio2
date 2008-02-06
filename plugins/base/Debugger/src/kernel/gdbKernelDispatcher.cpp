

/*
	Class GdbKernelDispatcher
*/

#include "./kernel/gdbKernelDispatcher.h"


GdbKernelDispatcher::GdbKernelDispatcher( GdbParser *p) : GdbCore(p)
{
	mWidget = new QTextEdit();

	getContainer()->setWidget(mWidget);
	getContainer()->setWindowTitle(name());
}
//
GdbKernelDispatcher::~GdbKernelDispatcher()
{
}
//
QString GdbKernelDispatcher::name()
{
	return "GdbKernelDispatcher";
}
//
/*QWidget * GdbPluginManager::widget()
{
	return (QWidget*) mWidget;
}
//
void GdbPluginManager::setupDockWidget(QMainWindow *mw)
{
	setWidget(widget());
	setWindowTitle(name());
	setAllowedAreas(Qt::AllDockWidgetAreas);
	setFeatures (QDockWidget::DockWidgetMovable |QDockWidget::DockWidgetFloatable);
	mw->addDockWidget(Qt::RightDockWidgetArea, this);
}
*///
void GdbKernelDispatcher::add(GdbCore * p)
{
	listPlugin <<  p;
	connect(p, SIGNAL( sendRawData(GdbCore *, QByteArray)), this ,SLOT(onSendRawData( GdbCore * , QByteArray)));
	connect(p, SIGNAL( sendPostMessage(int, QByteArray)), this ,SLOT(onPostMessage( int , QByteArray)));
	mWidget->append("Pluging add  " + p->name() + " ... OK");
}


// process version 3
int GdbKernelDispatcher::process(QGdbMessageCore m)
{

	// posting message to all plugin
	for(int i=0; i<listPlugin.count(); i++)
	{
		listPlugin.at(i)->postGdbMessage(m);
	}


/*
	// process loop
	for( int iCurrentPlugin=0 ; iCurrentPlugin<listPlugin.count(); iCurrentPlugin++)
	{
//		if(listPlugin.at(iCurrentPlugin) != pluginSender)
		{


			QGdbMessage m = listPlugin.at(iCurrentPlugin)->getGdbMessage();
qDebug(" process plugin loop " + listPlugin.at(iCurrentPlugin)->name().toLocal8Bit() + " current ID is " + QByteArray::number(iCurrentPlugin) + " data " + m.msg);
 
			while(m.id != -2)	
			{
				mWidget->append("process plugin loop " + listPlugin.at(iCurrentPlugin)->name() + " current ID is " + QString::number(iCurrentPlugin) + " data " + m.msg);

				switch(listPlugin.at(iCurrentPlugin)->process(m.id,m.msg))
				{
					case PROCESS_TERMINED :{ mWidget->append("-> Termined"); break;}
					case PROCESS_WAITING : 
						{ 
							mWidget->append("-> waiting"); 
							if(listPlugin.at(iCurrentPlugin)->getGdbMessageCount()) 
								break;
							else
								return 0 ;
						}

					case PROCESS_SEND_DATA :{ mWidget->append("-> Send Data") ; return 0 ;}
					default : qDebug("(GdbPluginManager) plugin process return  unknow int");
				}
				m = listPlugin.at(iCurrentPlugin)->getGdbMessage();
			}
		}
	}*/
	return 0;
}
//
int GdbKernelDispatcher::processError(QGdbMessageCore m)
{
	// TODO
	m=m;
	return PROCESS_TERMINED;
}
//
void GdbKernelDispatcher::processExit()
{
}
//
void GdbKernelDispatcher::onPostMessage( int id, QByteArray data)
{
	QGdbMessageCore m;
	m.id = id;
	m.msg = data;

	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->postGdbMessage(m);
}
//
void GdbKernelDispatcher::onSendRawData( GdbCore *plug, QByteArray data)
{
//	plugSender = plug;
	emit sendRawData(this, data);
}
//
void GdbKernelDispatcher::gdbStarted()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->gdbStarted();
}
//
void GdbKernelDispatcher::gdbFinished()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->gdbFinished();
}
//
void GdbKernelDispatcher::targetLoaded()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->targetLoaded();
}
//
void GdbKernelDispatcher::targetRunning()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->targetRunning();
}
//
void GdbKernelDispatcher::targetStopped()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->targetStopped();
}
//
void GdbKernelDispatcher::targetExited()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->targetExited();
}
