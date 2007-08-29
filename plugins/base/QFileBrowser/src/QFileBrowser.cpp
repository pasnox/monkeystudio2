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
		// connect signals so we will be able to save/restore state of dock settings
		connect( pFileBrowser::instance(), SIGNAL( saveSettings() ), this, SLOT( saveSettings() ) );
		connect( pFileBrowser::instance(), SIGNAL( restoreSettings() ), this, SLOT( restoreSettings() ) );
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

void QFileBrowser::saveSettings()
{
	// save current drive and path
	setSettingsValue( "Drive", pFileBrowser::instance()->currentDrive() );
	setSettingsValue( "Path", pFileBrowser::instance()->currentPath() );
}

void QFileBrowser::restoreSettings()
{
	QString s;
	// restore last drive if available
	s = settingsValue( "Drive" ).toString();
	if ( !s.isEmpty() )
		pFileBrowser::instance()->setCurrentDrive( s );
	// restore last path if available
	s = settingsValue( "Path" ).toString();
	if ( !s.isEmpty() )
		pFileBrowser::instance()->setCurrentPath( s );
}

Q_EXPORT_PLUGIN2( BaseQFileBrowser, QFileBrowser )
