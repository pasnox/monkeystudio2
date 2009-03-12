#include "MkSShell.h"

#include <MkSShellConsole.h>
#include <MonkeyCore.h>
#include <UIMain.h>

class MkSShellDock : public pDockWidget
{
public:
	MkSShellDock( QWidget* parent = 0 )
		: pDockWidget( parent )
	{
		setWidget( new MkSShellConsole( this ) );
	}
};

MkSShell::MkSShell()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "MkS Shell" );
	mPluginInfos.Description = tr( "This plugin allow you to manually use the MkS Shell interpreter" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

MkSShell::~MkSShell()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool MkSShell::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		mDock = new MkSShellDock( MonkeyCore::mainWindow() );
		// add dock to dock toolbar entry
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addDock( mDock, infos().Caption, QIcon( pixmap() ) );
		// create menu action for the dock
		pActionsManager::setDefaultShortcut( mDock->toggleViewAction(), QKeySequence( "F6" ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		mDock->deleteLater();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

QPixmap MkSShell::pixmap() const
{ return QPixmap( ":/edit/icons/edit/search.png" ); }

Q_EXPORT_PLUGIN2( BaseMkSShell, MkSShell )
