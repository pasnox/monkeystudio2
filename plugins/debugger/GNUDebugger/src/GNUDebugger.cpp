#include "GNUDebugger.h"
#include "DockGNUDebugger.h"

#include <maininterface.h>

#include <QIcon>

GNUDebugger::GNUDebugger()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "GNU Debugger V1.1.0 new 8829" );
	mPluginInfos.Description = tr( "Plugin for Gdb intergration\n\nBe sure you haved Gdb version > 6.x and you haved this in your path" );
	mPluginInfos.Author = "Pinon Yannick aka Xiantia  <private mail>";
	mPluginInfos.Type = BasePlugin::iDebugger;
	mPluginInfos.Name = "Debugger";
	mPluginInfos.Version = "1.1.0";
	mPluginInfos.Enabled = false;
}

GNUDebugger::~GNUDebugger()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool GNUDebugger::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// add dock to dock toolbar entry
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( DockGNUDebugger::instance(), infos().Caption, QIcon( ":/icons/debugger.png" ) );
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		DockGNUDebugger::instance()->deleteLater();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

void GNUDebugger::saveSettings()
{
}

void GNUDebugger::restoreSettings()
{
}

Q_EXPORT_PLUGIN2( BaseGNUDebugger, GNUDebugger )
