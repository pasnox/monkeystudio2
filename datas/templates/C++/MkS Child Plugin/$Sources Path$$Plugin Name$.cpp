#include "$Plugin Name$.h"

pAbstractChild* $Plugin Name$::createDocument( const QString& fileName )
{
    // return your own widget documentd
    return 0;
}

void $Plugin Name$::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "$Plugin Name$" );
    mPluginInfos.Description = tr( "$Description$" );
    mPluginInfos.Author = "$Authors$";
    mPluginInfos.Type = BasePlugin::iChild;
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

Q_EXPORT_PLUGIN2( Child$Plugin Name$, $Plugin Name$ )
