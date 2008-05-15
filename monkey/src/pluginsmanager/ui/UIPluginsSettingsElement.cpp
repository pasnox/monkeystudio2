#include "UIPluginsSettingsElement.h"
#include "../BasePlugin.h"

UIPluginsSettingsElement::UIPluginsSettingsElement( BasePlugin* plugin, QWidget* p )
	: QWidget( p )
{
	Q_ASSERT( plugin );
	mPlugin = plugin;
	// setup dialog
	setupUi( this );
	BasePlugin::PluginInfos infos = mPlugin->infos();
	//lIcon->setPixmap();
	lTitle->setText( infos.Caption );
	lDescription->setText( infos.Description );
}
