#include "GNUDebugger.h"
#include "./ui/UIGNUDebuggerSetting.h"

#include <maininterface.h>
#include <coremanager.h>
#include <settingsmanager.h>
#include <monkey.h>
#include <queuedstatusbar.h>

#include <QIcon>
 
GNUDebugger::GNUDebugger()
{
	aSeparator = 0;
	// set plugin infos
	mPluginInfos.Caption = tr( "GNU Debugger" );
	mPluginInfos.Description = tr( "Plugin for Gdb intergration" );
	mPluginInfos.Author = "Pinon Yannick aka Xiantia  <private mail>";
	mPluginInfos.Type = BasePlugin::iDebugger;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.3.2";
	mPluginInfos.Enabled = false;

	// get the new instance of GdbPattermFile
	patternFile = GdbPatternFile::instance(this);
	GdbSetting::instance(this);
	GdbSetting::instance()->load();

	if(GdbSetting::instance()->getPathParseFile().isEmpty())
	{
		// load pattern
		QStringList pluginsPath = MonkeyCore::settings()->value( "Plugins/Path" ).toStringList();
		QFileInfoList files;
		// get all files in plugins path nammed 'gdbparsing.txt'
		QDir pluginsDir;
		for ( int i = 0; i < pluginsPath.count(); i++ )
		{
			QString path = pluginsPath.at( i );
			if ( QFileInfo( path ).isRelative() )
				path = QDir::cleanPath( QApplication::applicationDirPath() +"/" + path );
			pluginsDir.setPath( path );
			files << pMonkeyStudio::getFiles( pluginsDir, QString( "gdbparsing.txt" ), true );
		}

		
		if ( files.isEmpty())
			MonkeyCore::statusBar()->appendMessage( tr( "gdbparsing.txt not found. Debugger can not work ! " ) + GdbSetting::instance()->getPathParseFile(), 5000 ,QPixmap(), QBrush(QColor(255,80,80)));
		else
		{
			GdbSetting::instance()->setPathParseFile( files.first().absoluteFilePath());
			patternFile->load( GdbSetting::instance()->getPathParseFile() );
//			MonkeyCore::statusBar()->appendMessage( tr( "GdbPatternFile initializing sucess full" ), 2500 ,QPixmap(), QBrush(QColor(120,250,100)));
		}
	}
	else
	{
		// load txt file if possible, else warn user in status bar
		if ( ! patternFile->load( GdbSetting::instance()->getPathParseFile() ) )
			MonkeyCore::statusBar()->appendMessage( tr( "gdbparsing.txt not found. Debugger can not work ! " ) + GdbSetting::instance()->getPathParseFile(), 5000 ,QPixmap(), QBrush(QColor(255,80,80)));
//		else 	MonkeyCore::statusBar()->appendMessage( tr( "GdbPatternFile initializing sucess full" ), 2500 ,QPixmap(), QBrush(QColor(120,250,100)));

	}
}


GNUDebugger::~GNUDebugger()
{
	GdbSetting::instance()->save();

	if ( isEnabled() )
		setEnabled( false );
}

bool GNUDebugger::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
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
		actionList["aExitGdb"] = MonkeyCore::menuBar()->action( "mDebugger/aQuit", tr( "Quit debug mode" ), QIcon( ":/icons/close.png" ) );
		// connections
		connect( actionList["aLoadTarget"], SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionLoadTarget() ) );
		connect( actionList["aRestart"], SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionRestart() ) );
		connect( actionList["aContinue"], SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionContinue() ) );
		connect( actionList["aStepOver"], SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionStepOver() ) );
		connect( actionList["aStepInto"] , SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionStepInto() ) );
		connect( actionList["aExitGdb"], SIGNAL( triggered() ), mDockGNUDebugger, SLOT( onActionExit() ) );

		// init action disable
		actionList["aRestart"]->setEnabled( false );
		actionList["aContinue"]->setEnabled( false );
		actionList["aStepOver"]->setEnabled( false );
		actionList["aStepInto"]->setEnabled( false );
		actionList["aExitGdb"]->setEnabled( false );

		// separator and add icon to toolbar
		aSeparator = MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction();
		foreach ( QAction* a, actionList )
			MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( a );

		// set action to gdbControl
		// enable / disable action
		mDockGNUDebugger->setAction( &actionList );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// delete actions
		qDeleteAll( actionList );
		delete aSeparator;
		actionList.clear();
		// delete dock
		delete mDockGNUDebugger;
		//DockGNUDebugger::instance()->deleteLater(); <- not work, more slow
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

QPixmap GNUDebugger::pixmap() const
{ return QPixmap( ":/icons/debugger.png" ); }

void GNUDebugger::saveSettings()
{}

void GNUDebugger::restoreSettings()
{}

QWidget* GNUDebugger::settingsWidget()
{
	return new UIGNUDebuggerSetting;
}
// DebuggerPlugin
Q_EXPORT_PLUGIN2( BaseGNUDebugger, GNUDebugger )
