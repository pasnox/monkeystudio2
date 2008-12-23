#include "PostIt.h"

#include "UIPostIt.h"
#include <MonkeyCore.h>
#include <UIMain.h>

PostIt::PostIt()
{
    // set plugin infos
    mPluginInfos.Caption = tr( "PostIt" );
    mPluginInfos.Description = tr( "Write and read notes easly !" );
    mPluginInfos.Author = "Alexandre JORDAN <alexandre.jordan@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBase;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "0.0.2";
    mPluginInfos.Enabled = false;
}

PostIt::~PostIt()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool PostIt::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create action
		QAction* a = MonkeyCore::menuBar()->action( "mTools/aPostIt", infos().Caption, pixmap(), QString::null, mPluginInfos.Description );
		// connections
		connect( a, SIGNAL( triggered() ), this, SLOT( action_triggered() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// delete widget
		delete mPostIt;
		// delete action
		delete MonkeyCore::menuBar()->action( "mTools/aPostIt" );
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

void PostIt::action_triggered()
{
	if ( !mPostIt )
		mPostIt = new UIPostIt( MonkeyCore::mainWindow() );
	mPostIt->setVisible( !mPostIt->isVisible() );
}

Q_EXPORT_PLUGIN2( BasePostIt, PostIt )
