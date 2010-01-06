#include "SearchAndReplace.h"
#include "SearchWidget.h"
#include "SearchResultsDock.h"

#include <MonkeyCore.h>
#include <pWorkspace.h>
#include <pAbstractChild.h>
#include <UIMain.h>

void SearchAndReplace::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "SearchAndReplace" );
	mPluginInfos.Description = tr( "Search & Replace plugin" );
	mPluginInfos.Author = "Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>, Andrei KOPATS aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = false;
	mPluginInfos.Pixmap = pIconManager::pixmap( "SearchAndReplace.png", ":/icons" );
}

bool SearchAndReplace::install()
{
	mWidget = new SearchWidget;
	MonkeyCore::workspace()->layout()->addWidget( mWidget );
	mWidget->setVisible( false );

	mDock = new SearchResultsDock( mWidget->searchThread() );
	MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mDock, mDock->windowTitle(), mDock->windowIcon() );
	mDock->setVisible( false );

	mWidget->setResultsDock( mDock );

	pMenuBar* mb = MonkeyCore::menuBar();
	QAction* action;

	mb->beginGroup( "mEdit/mSearchReplace" );
		action = mb->action( "aSearchFile" );
		connect( action, SIGNAL( triggered() ), this, SLOT( searchFile_triggered() ) );

		action = mb->action( "aReplaceFile", tr( "&Replace..." ), QIcon( ":/edit/icons/edit/replace.png" ), tr( "Ctrl+R" ), tr( "Replace in the current file..." ) );
		connect( action, SIGNAL( triggered() ), this, SLOT( replaceFile_triggered() ) );

		action = mb->action( "aSearchPrevious", tr( "Search Previous" ), QIcon( ":/edit/icons/edit/previous.png" ), tr( "Shift+F3" ), tr( "Search previous occurrence" ) );
		connect( action, SIGNAL( triggered() ), mWidget, SLOT( on_pbPrevious_clicked() ) );

		action = mb->action( "aSearchNext", tr( "Search Next" ), QIcon( ":/edit/icons/edit/next.png" ), tr( "F3" ), tr( "Search next occurrence" ) );
		connect( action, SIGNAL( triggered() ), mWidget, SLOT( on_pbNext_clicked() ) );
	mb->endGroup();

	return true;
}

bool SearchAndReplace::uninstall()
{
	pMenuBar* mb = MonkeyCore::menuBar();
	QAction* action;

	mb->beginGroup( "mEdit/mSearchReplace" );
		action = mb->action( "aSearchFile" );
		disconnect( action, SIGNAL( triggered() ), this, SLOT( searchFile_triggered() ) );

		action = mb->action( "aReplaceFile" );
		disconnect( action, SIGNAL( triggered() ), this, SLOT( replaceFile_triggered() ) );
		delete action;

		action = mb->action( "aSearchPrevious" );
		disconnect( action, SIGNAL( triggered() ), mWidget, SLOT( on_pbPrevious_clicked() ) );
		delete action;

		action = mb->action( "aSearchNext" );
		disconnect( action, SIGNAL( triggered() ), mWidget, SLOT( on_pbNext_clicked() ) );
		delete action;
	mb->endGroup();

	delete mDock;
	delete mWidget;

	return true;
}

void SearchAndReplace::searchFile_triggered()
{
	pAbstractChild* document = MonkeyCore::workspace()->currentDocument();

	if ( ( document && document->editor() ) || !document )
	{
		/*
		SearchAndReplace::Mode mode = mWidget->mode();
		
		if ( !(mode & SearchAndReplace::ModeFlagSearch) )
		{
			mode = SearchAndReplace::ModeSearch;
		}
		*/
		
		mWidget->setMode( SearchAndReplace::ModeSearch/*mode*/ );
	}
}

void SearchAndReplace::replaceFile_triggered()
{
	pAbstractChild* document = MonkeyCore::workspace()->currentDocument();

	if ( ( document && document->editor() ) || !document )
	{
		/*
		SearchAndReplace::Mode mode = mWidget->mode();
		
		if ( !(mode & SearchAndReplace::ModeFlagReplace) )
		{
			mode = SearchAndReplace::ModeReplace;
		}
		*/
		
		mWidget->setMode( SearchAndReplace::ModeReplace/*mode*/ );
	}
}

Q_EXPORT_PLUGIN2( BaseSearchAndReplace, SearchAndReplace )
