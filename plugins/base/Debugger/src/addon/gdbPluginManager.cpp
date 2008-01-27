
/*
	Class GdbPluginManager
*/

#include "./addon/gdbPluginManager.h"


GdbPluginManager::GdbPluginManager(QWidget *p) : GdbBase(p)
{
	mWidget = new QTextEdit(this);
}
//
GdbPluginManager::~GdbPluginManager()
{
}
//
QString GdbPluginManager::name()
{
	return "GdbPluginManager";
}
//
QWidget * GdbPluginManager::widget()
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
//
void GdbPluginManager::addPlugin(QObject * p)
{
	listPlugin << (GdbBase * ) p;
	connect(p, SIGNAL( sendRawData(GdbBase *, QByteArray)), this ,SLOT(onSendRawData( GdbBase * , QByteArray)));
	connect(p, SIGNAL( sendMessage(int, QByteArray)), this ,SLOT(onPosteGdbMessage( int , QByteArray)));
	mWidget->append("Pluging add  " + ((GdbBase * )p)->name() + " ... OK");
}


// process version 3
int GdbPluginManager::process(int id,QByteArray data)
{
	// posting message to all plugin
	for(int i=0; i<listPlugin.count(); i++)
	{
		listPlugin.at(i)->postGdbMessage(id, data);
	}


	// process loop
	for( int iCurrentPlugin=0 ; iCurrentPlugin<listPlugin.count(); iCurrentPlugin++)
	{
//		if(listPlugin.at(iCurrentPlugin) != pluginSender)
		{
			QGdbMessage m = listPlugin.at(iCurrentPlugin)->getGdbMessage();
 
			while(m.id != -2)	
			{
				mWidget->append("process plugin loop " + listPlugin.at(iCurrentPlugin)->name() + " current ID is " + QString::number(iCurrentPlugin) + " data " + m.msg);

				switch(listPlugin.at(iCurrentPlugin)->process(m.id,m.msg))
				{
					case PROCESS_TERMINED :{ mWidget->append("-> Termined");/* next plug */ /*iCurrentPlugin++*/; break;}
					case PROCESS_WAITING : 
						{ 
							mWidget->append("-> waiting");/* wait data */ ; 
							if(listPlugin.at(iCurrentPlugin)->getGdbMessageCount()) 
								break;
							else
								return 0 ;
						}

					case PROCESS_SEND_DATA :{ mWidget->append("-> Send Data");/* wait data */ ; return 0 ;}
					default : qDebug("(GdbPluginManager) plugin process return  unknow int");
				}
				m = listPlugin.at(iCurrentPlugin)->getGdbMessage();
			}
		}
	}
	return 0;
}
//
int GdbPluginManager::processError(int id, QByteArray data)
{
	// TODO
 	id = id;
	data = data;
	return PROCESS_TERMINED;
}
//
void GdbPluginManager::processExit()
{
}
//
void GdbPluginManager::onPosteGdbMessage( int id, QByteArray data)
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->postGdbMessage(id, data);
}
//
void GdbPluginManager::onSendRawData( GdbBase *plug, QByteArray data)
{
	emit sendRawData(plug, data);
}
//
void GdbPluginManager::gdbStarted()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->gdbStarted();
}
//
void GdbPluginManager::gdbFinished()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->gdbFinished();
}
//
void GdbPluginManager::targetLoaded()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->targetLoaded();
}
//
void GdbPluginManager::targetRunning()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->targetRunning();
}
//
void GdbPluginManager::targetStopped()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->targetStopped();
}
//
void GdbPluginManager::targetExited()
{
	for(int i=0; i < listPlugin.count(); i++)
		listPlugin.at(i)->targetExited();
}
