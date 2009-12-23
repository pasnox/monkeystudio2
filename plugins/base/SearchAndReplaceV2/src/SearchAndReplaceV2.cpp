#include "SearchAndReplaceV2.h"
#include "SearchWidget.h"

#include <MonkeyCore.h>
#include <pWorkspace.h>

void SearchAndReplaceV2::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "SearchAndReplaceV2" );
	mPluginInfos.Description = tr( "Search & Replace plugin" );
	mPluginInfos.Author = "Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>, Andrei KOPATS aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.FirstStartEnabled = false;
	mPluginInfos.HaveSettingsWidget = false;
	mPluginInfos.Pixmap = pIconManager::pixmap( "SearchAndReplaceV2.png", ":/icons" );
}

bool SearchAndReplaceV2::install()
{
	mWidget = new SearchWidget;
	MonkeyCore::workspace()->layout()->addWidget( mWidget );
	//mWidget->setVisible( false );
	return true;
}

bool SearchAndReplaceV2::uninstall()
{
	delete mWidget;
	return true;
}

Q_EXPORT_PLUGIN2( BaseSearchAndReplaceV2, SearchAndReplaceV2 )
