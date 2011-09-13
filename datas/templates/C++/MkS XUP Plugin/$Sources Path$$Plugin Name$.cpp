#include "$Plugin Name$.h"

void $Plugin Name$::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "$Plugin Name$" );
    mPluginInfos.Description = tr( "$Description$" );
    mPluginInfos.Author = "$Authors$";
    mPluginInfos.Type = BasePlugin::iXUP;
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

Q_EXPORT_PLUGIN2( XUP$Plugin Name$, $Plugin Name$ )
