#include "XUPProjectEditor.h"

#include "MonkeyCore.h"
#include "UIMain.h"

#include <QDialog>
#include <QFileInfo>

XUPProjectEditor::XUPProjectEditor()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "XUP Project Editor" );
	mPluginInfos.Description = tr( "This is the default project settings editor for XUP based projects." );
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
	
	QDialog d( MonkeyCore::mainWindow() );
	d.setWindowTitle( tr( "Project Settings : %1" ).arg( QFileInfo( project->projectFilePath() ).completeBaseName() ) );
	return d.exec();
}

Q_EXPORT_PLUGIN2( XUPXUPProjectEditor, XUPProjectEditor )
