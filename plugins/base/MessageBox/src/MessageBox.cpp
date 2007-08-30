#include "MessageBox.h"
#include "UIMain.h"
#include "pDockToolBar.h"
#include "pDockMessageBox.h"

#include <QIcon>

MessageBox::MessageBox()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Message Box" );
	mPluginInfos.Description = tr( "Plugin for catching internal command console" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = "MessageBox";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

MessageBox::~MessageBox()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool MessageBox::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// add dock to dock toolbar entry
		UIMain::instance()->dockToolBar( Qt::LeftToolBarArea )->addDock( pDockMessageBox::instance(), infos().Caption, QIcon( ":/icons/messagebox.png" ) );
		// connect signals so we will be able to save/restore state of dock settings
		connect( pDockMessageBox::instance(), SIGNAL( saveSettings() ), this, SLOT( saveSettings() ) );
		connect( pDockMessageBox::instance(), SIGNAL( restoreSettings() ), this, SLOT( restoreSettings() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// it will remove itself from dock toolbar when deleted
		pDockMessageBox::instance()->deleteLater();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	
	// return default value
	return true;
}

void MessageBox::saveSettings()
{
	// save current drive and path
	/*
	setSettingsValue( "Drive", pDockMessageBox::instance()->currentDrive() );
	setSettingsValue( "Path", pDockMessageBox::instance()->currentPath() );
	*/
}

void MessageBox::restoreSettings()
{
	/*
	QString s;
	// restore last drive if available
	s = settingsValue( "Drive" ).toString();
	if ( !s.isEmpty() )
		pDockMessageBox::instance()->setCurrentDrive( s );
	// restore last path if available
	s = settingsValue( "Path" ).toString();
	if ( !s.isEmpty() )
		pDockMessageBox::instance()->setCurrentPath( s );
	*/
}

Q_EXPORT_PLUGIN2( BaseMessageBox, MessageBox )
