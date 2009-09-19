#include "PostIt.h"

#include "UIPostIt.h"
#include <MonkeyCore.h>
#include <UIMain.h>

void PostIt::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "PostIt" );
	mPluginInfos.Description = tr( "Write and read notes easly !" );
	mPluginInfos.Author = "Alexandre JORDAN <alexandre.jordan@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.0.3";
	mPluginInfos.FirstStartEnabled = false;
	mPluginInfos.Pixmap = pIconManager::pixmap( "knotes.png", ":/icons" );
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