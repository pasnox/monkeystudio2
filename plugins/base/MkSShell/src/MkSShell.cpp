#include "MkSShell.h"

#include <MkSShellConsole.h>
#include <MonkeyCore.h>
#include <UIMain.h>

class MkSShellDock : public pDockWidget
{
public:
	MkSShellDock( QWidget* parent = 0 )
		: pDockWidget( parent )
	{
		setWidget( new MkSShellConsole( this ) );
	}
};

BasePlugin::PluginInfos MkSShell::infos() const
{
	PluginInfos pluginInfos;
	pluginInfos.Caption = tr( "MkS Shell" );
	pluginInfos.Description = tr( "This plugin allow you to manually use the MkS Shell interpreter" );
	pluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	pluginInfos.Type = BasePlugin::iBase;
	pluginInfos.Name = PLUGIN_NAME;
	pluginInfos.Version = "1.0.0";
	pluginInfos.FirstStartEnabled = false;
	pluginInfos.Pixmap = pIconManager::pixmap( "konsole.png", ":/icons" );
	
	return pluginInfos;
}

bool MkSShell::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		mDock = new MkSShellDock( MonkeyCore::mainWindow() );
		// add dock to dock toolbar entry
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addDock( mDock, infos().Caption, QIcon( infos().Pixmap ) );
		// create menu action for the dock
		pActionsManager::setDefaultShortcut( mDock->toggleViewAction(), QKeySequence( "F6" ) );
		// set plugin enabled
		stateAction()->setChecked( true );
	}
	else if ( !b && isEnabled() )
	{
		mDock->deleteLater();
		// set plugin disabled
		stateAction()->setChecked( false );
	}
	// return default value
	return true;
}

Q_EXPORT_PLUGIN2( BaseMkSShell, MkSShell )
