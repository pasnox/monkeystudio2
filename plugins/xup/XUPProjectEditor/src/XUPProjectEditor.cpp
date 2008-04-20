#include "XUPProjectEditor.h"

#include <coremanager.h>
#include <maininterface.h>
#include <xupmanager.h>

XUPProjectEditor::XUPProjectEditor()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "XUP Project Editor" );
	mPluginInfos.Description = tr( "Default and Generic Project Settings Editor for XUPManager" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iXUP;
	mPluginInfos.Name = "XUPProjectEditor";
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.Enabled = false;
}

XUPProjectEditor::~XUPProjectEditor()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool XUPProjectEditor::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

bool XUPProjectEditor::editProject( XUPItem* project )
{
	if ( !project )
		return false;
	return UIXUPProjectEditor( project, MonkeyCore::mainWindow() ).exec();
}

Q_EXPORT_PLUGIN2( XUPXUPProjectEditor, XUPProjectEditor )
