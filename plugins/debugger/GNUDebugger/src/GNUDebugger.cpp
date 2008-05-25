#include "GNUDebugger.h"

#include "./ui/UIGNUDebuggerSetting.h"

#include <maininterface.h>

#include <QIcon>

GNUDebugger::GNUDebugger()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Debugger" ); // fix //tr( "GNU Debugger V1.1.0 new 8829" );
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
		mDockGNUDebugger = DockGNUDebugger::instance();
		
		// add dock to dock toolbar entry
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mDockGNUDebugger, 
			infos().Caption, QIcon( ":/icons/debugger.png" ) );

		// enable debugger menu
		MonkeyCore::menuBar()->menu( "mDebugger" )->setEnabled(true);

		// add actions to main window
		actionList["aLoadTarget"] = MonkeyCore::menuBar()->action( "mDebugger/aSelectTarget", tr( "Load target" ),
			QIcon( ":/icons/file.png" ), tr( "F6" ) ); 
		connect(actionList["aLoadTarget"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onLoadTarget() ) );
		
		actionList["aRestart"] = MonkeyCore::menuBar()->action( "mDebugger/aStartorrestart", tr( "Start or restart target" ), 
			QIcon( ":/icons/update.png" ), tr( "F7" ) );
		connect(actionList["aRestart"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onRun() ) );
		
		actionList["aContinue"] = MonkeyCore::menuBar()->action( "mDebugger/aContinue", tr( "Continue to next breakpoint" ), 
			QIcon( ":/icons/play.png" ), tr( "F8" ) );
		connect( actionList["aContinue"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onContinue() ) );
		
		actionList["aStepOver"] = MonkeyCore::menuBar()->action( "mDebugger/aStepover", tr( "Step over" ), 
			QIcon( ":/icons/stepover.png" ), tr( "F8" ) );
		connect( actionList["aStepOver"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onStepOver() ) );

		actionList["aStepInto"] = MonkeyCore::menuBar()->action( "mDebugger/aStepinto", tr( "Step into" ), 
			QIcon( ":/icons/stepinto.png" ), tr( "F9" ) );
		connect(actionList["aStepInto"] , SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onStepInto() ) );
		
		actionList["aStop"] = MonkeyCore::menuBar()->action( "mDebugger/aStop", tr( "Stop" ), 
			QIcon( ":/icons/stop.png" ), tr( "F10" ) );
		connect( actionList["aStop"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onStop() ) );
		
		actionList["aExitGdb"] = MonkeyCore::menuBar()->action( "mDebugger/aQuit", tr( "Quit debug mode" ), 
			QIcon( ":/icons/close.png" ), tr( "F11" ) );
		connect( actionList["aExitGdb"], SIGNAL( triggered() ), mDockGNUDebugger->gdbControl(), SLOT( onExit() ) );

		// init action disable
		actionList["aRestart"]->setEnabled(false);
		actionList["aContinue"]->setEnabled(false);
		actionList["aStepOver"]->setEnabled(false);
		actionList["aStepInto"]->setEnabled(false);
		actionList["aStop"]->setEnabled(false);
		actionList["aExitGdb"]->setEnabled(false);


		// separator and add icon to toolbar
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction();
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( actionList["aLoadTarget"]);
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( actionList["aRestart"]);
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( actionList["aContinue"]);
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( actionList["aStepOver"]);
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( actionList["aStepInto"]);
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( actionList["aStop"]);
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( actionList["aExitGdb"]);

		// set action to gdbControl
		// enable / disable action
		mDockGNUDebugger->gdbControl()->setAction(&actionList);

		mPluginInfos.Enabled = true;

	}
	else if ( !b && isEnabled() )
	{
		// delete actions
		delete MonkeyCore::menuBar()->action( "mDebugger/aSelectTarget" );
		delete MonkeyCore::menuBar()->action( "mDebugger/aStartorrestart" );
		delete MonkeyCore::menuBar()->action( "mDebugger/aContinue" );
		delete MonkeyCore::menuBar()->action( "mDebugger/aStepinto" );
		delete MonkeyCore::menuBar()->action( "mDebugger/aStepover" );
		delete MonkeyCore::menuBar()->action( "mDebugger/aStop" );
		delete MonkeyCore::menuBar()->action( "mDebugger/aQuit" );

		// disable debugger menu
		MonkeyCore::menuBar()->menu( "mDebugger" )->setEnabled(false);

		DockGNUDebugger::instance()->deleteLater();

		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

void GNUDebugger::saveSettings()
{
}

void GNUDebugger::restoreSettings()
{
}

QWidget* GNUDebugger::settingsWidget()
{ return  new UIGNUDebuggerSetting; }

Q_EXPORT_PLUGIN2( BaseGNUDebugger, GNUDebugger )
