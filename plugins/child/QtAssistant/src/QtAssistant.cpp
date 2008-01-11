#include "QtAssistant.h"
#include "pMonkeyStudio.h"
#include "UIMain.h"
#include "pDockToolBar.h"
#include "QtAssistantChild.h"
#include "MonkeyCore.h"

// assistant include
#include "config.h"

using namespace pMonkeyStudio;

QtAssistant::QtAssistant()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Qt Assistant" );
	mPluginInfos.Description = tr( "This plugin embbeded the official Qt Asssitant" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iChild;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
	// initialize assistant resource
	Q_INIT_RESOURCE( assistant );
}

QtAssistant::~QtAssistant()
{
	if ( isEnabled() )
		setEnabled( false );
}

QWidget* QtAssistant::settingsWidget()
{ return BasePlugin::settingsWidget(); }

bool QtAssistant::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// load configuration
		Config* conf = Config::loadConfig( QString() );
		if ( !conf )
			warning( tr( "Qt Assistant profile..." ), tr( "Can't load/create the default profile for Qt Assistant, aborting..." ) );
		// install dock
		MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( QtAssistantChild::instance()->dock(), infos().Caption, QIcon( ":/trolltech/assistant/images/assistant.png" ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		QtAssistantChild::cleanInstance();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

Q_EXPORT_PLUGIN2( ChildQtAssistant, QtAssistant )
