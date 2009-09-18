#include "PostIt.h"

#include "UIPostIt.h"
#include <MonkeyCore.h>
#include <UIMain.h>

BasePlugin::PluginInfos PostIt::infos() const
{
	PluginInfos pluginInfos;
	pluginInfos.Caption = tr( "PostIt" );
	pluginInfos.Description = tr( "Write and read notes easly !" );
	pluginInfos.Author = "Alexandre JORDAN <alexandre.jordan@gmail.com>";
	pluginInfos.Type = BasePlugin::iBase;
	pluginInfos.Name = PLUGIN_NAME;
	pluginInfos.Version = "0.0.3";
	pluginInfos.FirstStartEnabled = false;
	pluginInfos.Pixmap = pIconManager::pixmap( "knotes.png", ":/icons" );
	
	return pluginInfos;
}

bool PostIt::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create action
		QAction* a = MonkeyCore::menuBar()->action( "mTools/aPostIt", infos().Caption, infos().Pixmap, QString::null, infos().Description );
		// connections
		connect( a, SIGNAL( triggered() ), this, SLOT( action_triggered() ) );
		// set plugin enabled
		stateAction()->setChecked( true );
	}
	else if ( !b && isEnabled() )
	{
		// delete widget
		delete mPostIt;
		// delete action
		delete MonkeyCore::menuBar()->action( "mTools/aPostIt" );
		// set plugin disabled
		stateAction()->setChecked( false );
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
