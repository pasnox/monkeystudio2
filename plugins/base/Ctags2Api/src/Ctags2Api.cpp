#include "Ctags2Api.h"
#include "pMenuBar.h"
#include "UICtags2Api.h"

Ctags2Api::Ctags2Api()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Api File Generator" );
	mPluginInfos.Description = tr( "This plugin allow to generate api file using ctags." );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

Ctags2Api::~Ctags2Api()
{
	if ( isEnabled() )
		setEnabled( false );
}

QWidget* Ctags2Api::settingsWidget()
{ return BasePlugin::settingsWidget(); }

bool Ctags2Api::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create action
		QAction* a = pMenuBar::instance()->action( "mEdit/aCtags2Api",  tr( "Api File Generator" ), QIcon( ":/icons/Ctags2Api.png" ), tr( "Ctrl+Alt+G" ), mPluginInfos.Description );
		connect( a, SIGNAL( triggered() ), this, SLOT( UICtags2Api_show() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// delete action
		delete pMenuBar::instance()->action( "mEdit/aCtags2Api" );
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	
	// return default value
	return true;
}

void Ctags2Api::UICtags2Api_show()
{
	UICtags2Api w;
	w.exec();
}

Q_EXPORT_PLUGIN2( BaseCtags2Api, Ctags2Api )
