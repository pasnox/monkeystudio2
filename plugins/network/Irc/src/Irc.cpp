

#include "Irc.h"

#include <MonkeyCore.h>
#include <UIMain.h>
#include <pConsoleManager.h>

#include <QIcon>
#include <QTabWidget>

BasePlugin::PluginInfos Irc::infos() const
{
	PluginInfos pluginInfos;
	pluginInfos.Caption = tr( "Chat Irc" );
	pluginInfos.Description = tr( "Plugin Irc for Monkey studio live" );
	pluginInfos.Author = "Pinon yannick aka Xiantia <private mail>";
	pluginInfos.Type = BasePlugin::iBase;
	pluginInfos.Name = PLUGIN_NAME;
	pluginInfos.Version = "1.0.0";
	pluginInfos.FirstStartEnabled = false;
	pluginInfos.Pixmap = QPixmap( ":/icons/irc.png" );
	
	return pluginInfos;
}

bool Irc::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create docks
		mIrcDock = IrcDock::instance();
		// add docks to main window
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mIrcDock, infos().Caption, QIcon( infos().Pixmap ) );
		// add actions to main window
//		connect( MonkeyCore::menuBar()->action( "mView/aShowBuild", tr( "Show Build Steps" ), QIcon( ":/icons/tabbuild.png" ), tr( "F9" ) ), SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showBuild() ) );
//		connect( MonkeyCore::menuBar()->action( "mView/aShowNextError", tr( "Show Next Error" ), QIcon( ":/icons/goto.png" ), "Shift+F9" ), SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showNextError() ) );
//		connect( MonkeyCore::menuBar()->action( "mView/aShowOutput", tr( "Show Output" ), QIcon( ":/icons/taboutput.png" ) , "F10" ), SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showOutput() ) );
//		connect( MonkeyCore::menuBar()->action( "mView/aShowCommands", tr( "Show Commands" ), QIcon( ":/icons/tablog.png" ), "F11" ), SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showLog() ) );
//		connect( MonkeyCore::menuBar()->action( "mView/aShowSearchResults", tr( "Show Search Results" ), QIcon( ":/icons/tabsearch.png" ), "F12" ), SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showSearchResults() ) );
//		connect( MonkeyCore::consoleManager(), SIGNAL( started() ), this, SLOT( onConsoleStarted() ) );
		// set plugin enabled
		stateAction()->setChecked( true );
	}
	else if ( !b && isEnabled() )
	{
		// disconnect
//		disconnect( MonkeyCore::consoleManager(), SIGNAL( started() ), this, SLOT( onConsoleStarted() ) );
		// delete actions
//		delete MonkeyCore::menuBar()->action( "mView/aShowBuild" );
//		delete MonkeyCore::menuBar()->action( "mView/aShowNextError" );
//		delete MonkeyCore::menuBar()->action( "mView/aShowOutput" );
//		delete MonkeyCore::menuBar()->action( "mView/aShowCommands" );
//		delete MonkeyCore::menuBar()->action( "mView/aShowSearchResults" );
		// delete docks
		delete mIrcDock;
		// set plugin disabled
		stateAction()->setChecked( false );
	}
	// return default value
	return true;
}

Q_EXPORT_PLUGIN2( BaseIrc, Irc )
