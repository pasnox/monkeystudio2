/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : MessageBox.cpp
** Date      : 2008-01-14T00:39:59
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
#include "MessageBox.h"
#include "ui/MessageBoxDocks.h"
#include "ui/UIMessageBoxSettings.h"

#include <coremanager.h>
#include <maininterface.h>
#include <consolemanager.h>

#include <QIcon>
#include <QTabWidget>

MessageBox::MessageBox()
{
	mMessageBoxDocks = 0;
	// set plugin infos
	mPluginInfos.Caption = tr( "Message Box" );
	mPluginInfos.Description = tr( "Plugin for catching internal command console" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
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
		// create docks
		mMessageBoxDocks = new MessageBoxDocks( this );
		// add docks to main window
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mMessageBoxDocks->mBuildStep, mMessageBoxDocks->mBuildStep->windowTitle(), mMessageBoxDocks->mBuildStep->windowIcon() );
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mMessageBoxDocks->mOutput, mMessageBoxDocks->mOutput->windowTitle(), mMessageBoxDocks->mOutput->windowIcon() );
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mMessageBoxDocks->mCommand, mMessageBoxDocks->mCommand->windowTitle(), mMessageBoxDocks->mCommand->windowIcon() );
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mMessageBoxDocks->mSearchResult, mMessageBoxDocks->mSearchResult->windowTitle(), mMessageBoxDocks->mSearchResult->windowIcon() );
		// add actions to main window
		connect( MonkeyCore::menuBar()->action( "mView/aShowBuild", tr( "Show Build Steps" ), QIcon( ":/icons/tabbuild.png" ), tr( "F9" ) ), SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showBuild() ) );
		connect( MonkeyCore::menuBar()->action( "mView/aShowNextError", tr( "Show Next Error" ), QIcon( ":/icons/goto.png" ), "Shift+F9" ), SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showNextError() ) );
		connect( MonkeyCore::menuBar()->action( "mView/aShowOutput", tr( "Show Output" ), QIcon( ":/icons/taboutput.png" ) , "F10" ), SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showOutput() ) );
		connect( MonkeyCore::menuBar()->action( "mView/aShowCommands", tr( "Show Commands" ), QIcon( ":/icons/tablog.png" ), "F11" ), SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showLog() ) );
		connect( MonkeyCore::menuBar()->action( "mView/aShowSearchResults", tr( "Show Search Results" ), QIcon( ":/icons/tabsearch.png" ), "F12" ), SIGNAL( triggered() ), mMessageBoxDocks, SLOT( showSearchResults() ) );
		connect( MonkeyCore::consoleManager(), SIGNAL( started() ), this, SLOT( onConsoleStarted() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// disconnect
		disconnect( MonkeyCore::consoleManager(), SIGNAL( started() ), this, SLOT( onConsoleStarted() ) );
		// delete actions
		delete MonkeyCore::menuBar()->action( "mView/aShowBuild" );
		delete MonkeyCore::menuBar()->action( "mView/aShowNextError" );
		delete MonkeyCore::menuBar()->action( "mView/aShowOutput" );
		delete MonkeyCore::menuBar()->action( "mView/aShowCommands" );
		delete MonkeyCore::menuBar()->action( "mView/aShowSearchResults" );
		// delete docks
		delete mMessageBoxDocks;
		mMessageBoxDocks = 0;
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

QWidget* MessageBox::settingsWidget()
{ return new UIMessageBoxSettings( this ); }

void MessageBox::onConsoleStarted()
{
	if ( settingsValue( "ActivateDock", true ).toBool() )
	{
		UIMessageBoxSettings::Dock dock = (UIMessageBoxSettings::Dock)settingsValue( "ActivatedDock", UIMessageBoxSettings::Output ).toInt();
		switch ( dock )
		{
			case UIMessageBoxSettings::BuildStep:
				mMessageBoxDocks->mBuildStep->show();
				break;
			case UIMessageBoxSettings::Output:
				mMessageBoxDocks->mOutput->show();
				break;
			case UIMessageBoxSettings::Command:
				mMessageBoxDocks->mCommand->show();
				break;
		}
	}
}

Q_EXPORT_PLUGIN2( BaseMessageBox, MessageBox )
