#include "QFileBrowser.h"
#include "UIMain.h"
#include "pDockToolBar.h"
#include "pFileBrowser.h"

#include <QIcon>

QFileBrowser::QFileBrowser()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "File Browser" );
	mPluginInfos.Description = tr( "Plugin for browsing file outside the project" );
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = "QFileBrowser";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

QFileBrowser::~QFileBrowser()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool QFileBrowser::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// add dock to dock toolbar entry
		UIMain::instance()->dockToolBar( Qt::LeftToolBarArea )->addDock( pFileBrowser::instance(), infos().Caption, QIcon( ":/icons/browser.png" ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// it will remove itself from dock toolbar when deleted
		pFileBrowser::instance()->deleteLater();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	
	// return default value
	return true;
}

Q_EXPORT_PLUGIN2( BaseQFileBrowser, QFileBrowser )
