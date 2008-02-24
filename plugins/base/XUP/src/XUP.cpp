#include "XUP.h"
#include "UIXUPManager.h"

#include "MonkeyCore.h"
#include "UIMain.h"
#include "pDockToolBar.h"

XUP::XUP()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "XUP Manager" );
	mPluginInfos.Description = tr( "Next Gen XML Based Projects Manager" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = "XUP";
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.Enabled = false;
	//Q_INIT_RESOURCE( XUPManager );
}

XUP::~XUP()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool XUP::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create dock manager
		mDockXUPManager = new UIXUPManager;
		// add dock to dockmanager
		MonkeyCore::mainWindow()->dockToolBar( Qt::LeftToolBarArea )->addDock( mDockXUPManager, infos().Caption, QIcon( ":/icons/console.png" ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// delete dock
		delete mDockXUPManager;
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	
	// return default value
	return true;
}

Q_EXPORT_PLUGIN2( BaseXUP, XUP )