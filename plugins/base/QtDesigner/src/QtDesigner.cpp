#include "QtDesigner.h"
#include "UIMain.h"
#include "pWorkspace.h"

#include "pMonkeyStudio.h"

#include "pDockToolBar.h"
#include "QtDesignerChild.h"

using namespace pMonkeyStudio;

QtDesigner::QtDesigner()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Qt Designer" );
	mPluginInfos.Description = tr( "This plugin embeds Qt Designer" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Luc Bruant aka fullmetalcoder <fullmetalcoder@hotmail.fr>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

QtDesigner::~QtDesigner()
{
	if ( isEnabled() )
		setEnabled( false );
}

QWidget* QtDesigner::settingsWidget()
{ return BasePlugin::settingsWidget(); }

bool QtDesigner::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// add designer to main window
		UIMain::instance()->workspace()->insertTab( 0, QtDesignerChild::instance(), QIcon( ":/icons/designer.png" ), tr( "Qt Designer" ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		QtDesignerChild::instance()->close();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

Q_EXPORT_PLUGIN2( BaseQtDesigner, QtDesigner )
