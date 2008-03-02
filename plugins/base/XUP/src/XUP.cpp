#include "XUP.h"
#include "QMakeXUPItem.h"
#include "UIXUPManager.h"
#include "UIXUPProjectEditor.h"

#include "MonkeyCore.h"
#include "UIMain.h"
#include "pDockToolBar.h"

XUP::XUP()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "XUP Manager / XUP Project Editor" );
	mPluginInfos.Description = tr( "Next Gen XML Based Projects Manager, it embeds the default project settings editor for XUP based projects and the XUP Project Manager" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iXUP;
	mPluginInfos.Name = "XUPManagerEditor";
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.Enabled = false;
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
		// register XUPItem classes
		mDockXUPManager->registerItem( new XUPItem );
		mDockXUPManager->registerItem( new QMakeXUPItem );
		// add dock to dockmanager
		MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( mDockXUPManager, infos().Caption, QIcon( ":/icons/console.png" ) );
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

bool XUP::editProject( XUPItem* project )
{
	if ( !project )
		return false;
	return UIXUPProjectEditor( project, MonkeyCore::mainWindow() ).exec();
}

Q_EXPORT_PLUGIN2( XUPXUP, XUP )
