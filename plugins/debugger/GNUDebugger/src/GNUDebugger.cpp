#include "GNUDebugger.h"
#include "./ui/UIGNUDebuggerSetting.h"

#include <maininterface.h>

#include <QIcon>

GNUDebugger::GNUDebugger()
{
	aSeparator = 0;
	// set plugin infos
	mPluginInfos.Caption = tr( "GNU Debugger" );
	mPluginInfos.Description = tr( "Plugin for Gdb intergration\n\nBe sure you haved Gdb version > 6.x and you haved this in your path" );
	mPluginInfos.Author = "Pinon Yannick aka Xiantia  <private mail>";
	mPluginInfos.Type = BasePlugin::iDebugger;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.2.0";
	mPluginInfos.Enabled = false;
}

GNUDebugger::~GNUDebugger()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool GNUDebugger::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
//		QMessageBox::warning(NULL,tr("Sorry ..."), "enable plugin");
		// get instance
		 mDockGNUDebugger = DockGNUDebugger::instance();
		// add dock to dock toolbar entry
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mDockGNUDebugger, infos().Caption, QIcon( pixmap() ) );
		// add actions to main window
		actionList["aLoadTarget"] = MonkeyCore::menuBar()->action( "mDebugger/aSelectTarget", tr( "Load target" ), QIcon( ":/icons/file.png" ) );
		actionList["aRestart"] = MonkeyCore::menuBar()->action( "mDebugger/aStartorrestart", tr( "Start or restart target" ), QIcon( ":/icons/update.png" ) );
		actionList["aContinue"] = MonkeyCore::menuBar()->action( "mDebugger/aContinue", tr( "Continue to next breakpoint" ), QIcon( ":/icons/play.png" ) );
		actionList["aStepOver"] = MonkeyCore::menuBar()->action( "mDebugger/aStepover", tr( "Step over" ), QIcon( ":/icons/stepover.png" ) );
		actionList["aStepInto"] = MonkeyCore::menuBar()->action( "mDebugger/aStepinto", tr( "Step into" ), QIcon( ":/icons/stepinto.png" ) );
		actionList["aStop"] = MonkeyCore::menuBar()->action( "mDebugger/aStop", tr( "Stop" ), QIcon( ":/icons/stop.png" ) );
		actionList["aExitGdb"] = MonkeyCore::menuBar()->action( "mDebugger/aQuit", tr( "Quit debug mode" ), QIcon( ":/icons/close.png" ) );
		// connections
		connect( actionList["aLoadTarget"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onLoadTarget() ) );
		connect( actionList["aRestart"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onRun() ) );
		connect( actionList["aContinue"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onContinue() ) );
		connect( actionList["aStepOver"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onStepOver() ) );
		connect( actionList["aStepInto"] , SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onStepInto() ) );
		connect( actionList["aStop"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onStop() ) );
		connect( actionList["aExitGdb"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onExit() ) );
		// init action disable
		actionList["aRestart"]->setEnabled( false );
		actionList["aContinue"]->setEnabled( false );
		actionList["aStepOver"]->setEnabled( false );
		actionList["aStepInto"]->setEnabled( false );
		actionList["aStop"]->setEnabled( false );
		actionList["aExitGdb"]->setEnabled( false );
		// separator and add icon to toolbar
		aSeparator = MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction();
		foreach ( QAction* a, actionList )
			MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( a );
		// set action to gdbControl
		// enable / disable action
		mDockGNUDebugger->gdbControl()->setAction( &actionList );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		
//		QMessageBox::warning(NULL,tr("Sorry ..."), "desible plugin");
		// delete actions
		foreach ( QAction* a, actionList )
			delete MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( a );
//		qDeleteAll( actionList );
		delete aSeparator;
		actionList.clear();
		// delete dock
		delete mDockGNUDebugger;
//		DockGNUDebugger::instance()->deleteLater();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

QPixmap GNUDebugger::pixmap() const
{ return QPixmap( ":/icons/debugger.png" ); }

void GNUDebugger::saveSettings()
{
}

void GNUDebugger::restoreSettings()
{
}

QWidget* GNUDebugger::settingsWidget()
{ return new UIGNUDebuggerSetting; }

Q_EXPORT_PLUGIN2( BaseGNUDebugger, GNUDebugger )
