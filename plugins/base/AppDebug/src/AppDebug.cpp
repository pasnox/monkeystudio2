#include "AppDebug.h"
#include "DebugDockWidget.h"

#include <coremanager/MonkeyCore.h>
#include <maininterface/UIMain.h>
#include <pDockToolBar.h>

QWeakPointer<DebugDockWidget> AppDebug::mDock;

void AppDebug::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "Application Debug View" );
	mPluginInfos.Description = tr( "A plugin that help to debug Mks itself" );
	mPluginInfos.Author = "Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.FirstStartEnabled = false;
	mPluginInfos.HaveSettingsWidget = false;
	mPluginInfos.Pixmap = pIconManager::pixmap( "AppDebug.png", ":/icons" );
}

bool AppDebug::install()
{
	mDock = new DebugDockWidget;
	qInstallMsgHandler( AppDebug::qtMessageHandler );
	MonkeyCore::mainWindow()->dockToolBar( Qt::LeftToolBarArea )->addDock( mDock.data(), infos().Caption, pIconManager::icon( "AppDebug.png", ":/icons" ) );
	
	return true;
}

bool AppDebug::uninstall()
{
	qInstallMsgHandler( 0 );
	mDock.data()->deleteLater();
	
	return true;
}

void AppDebug::qtMessageHandler( QtMsgType type, const char* msg )
{
	if ( mDock ) {
		mDock.data()->qtMessageHandler( type, msg );
	}
}

Q_EXPORT_PLUGIN2( BaseAppDebug, AppDebug )
