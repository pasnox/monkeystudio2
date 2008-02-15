/****************************************************************************
** Authors   : Mashin Engeny aka evgenM (author), Andrei Kopats (integration)
** FileName  : FindInFiles.h
** License   : GPL
** Comment   : FindInFiles plugin
** Home Page : http://www.monkeystudio.org
*/

#include "FindInFiles.h"
#include "MonkeyCore.h"
#include "UIMain.h"
#include "pMenuBar.h"
#include "pDockToolBar.h"
#include "DockFindInFiles.h"

#include <QIcon>
#include <QTabWidget>

FindInFiles::FindInFiles()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Find In Files" );
	mPluginInfos.Description = tr( "Plugin for searching text in folder/project" );
	mPluginInfos.Author = "Evgeny Mashin (author), Kopats Andrei (integration)";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = "FindInFiles";
	mPluginInfos.Version = "0.0.1";
	mPluginInfos.Enabled = false;
}

FindInFiles::~FindInFiles()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool FindInFiles::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// add dock to dock toolbar entry
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( DockFindInFiles::instance(), infos().Caption, QIcon( ":/icons/console.png" ) );
		// connect signals so we will be able to save/restore state of dock settings
		connect( MonkeyCore::menuBar()->action( "mEdit/aFindInFiles", tr( "Search in files" ), QIcon( ":/icons/tabbuild.png" ), tr( "F10" ) ), SIGNAL( triggered() ), DockFindInFiles::instance(), SLOT( show() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		delete MonkeyCore::menuBar()->action( "mEdit/aFindInFiles" );
		// it will remove itself from dock toolbar when deleted
		DockFindInFiles::instance()->deleteLater();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

Q_EXPORT_PLUGIN2( BaseFindInFiles, FindInFiles )
