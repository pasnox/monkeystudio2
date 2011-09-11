#include "$Plugin Name$.h"

pCommand $Plugin Name$::defaultCommand() const
{
	// return your default command
	return pCommand();
}

pCommand::List $Plugin Name$::defaultCommands() const
{
	// return the defaults user commands
	return pCommand::List();
}

void $Plugin Name$::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "$Plugin Name$" );
	mPluginInfos.Description = tr( "$Description$" );
	mPluginInfos.Author = "$Authors$";
	mPluginInfos.Type = BasePlugin::iCLITool;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.FirstStartEnabled = false;
	mPluginInfos.HaveSettingsWidget = false;
	mPluginInfos.Pixmap = pIconManager::pixmap( "$Plugin Name$.png", ":/icons" );
}

bool $Plugin Name$::install()
{
	// place your init code here
	return false;
}

bool $Plugin Name$::uninstall()
{
	// place your deinit code here
	return false;
}

Q_EXPORT_PLUGIN2( Interpreter$Plugin Name$, $Plugin Name$ )
