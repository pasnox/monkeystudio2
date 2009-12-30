#include "SearchAndReplaceV2.h"
#include "SearchWidget.h"
#include "SearchResultsDock.h"

#include <MonkeyCore.h>
#include <pWorkspace.h>
#include <UIMain.h>

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
	mDock = new SearchResultsDock( mWidget->searchThread() );
	MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mDock, mDock->windowTitle(), mDock->windowIcon() );
	mDock->setVisible( false );
	mWidget->setResultsDock( mDock );
	return true;
}

bool SearchAndReplaceV2::uninstall()
{
	delete mDock;
	delete mWidget;
	return true;
}

Q_EXPORT_PLUGIN2( BaseSearchAndReplaceV2, SearchAndReplaceV2 )
