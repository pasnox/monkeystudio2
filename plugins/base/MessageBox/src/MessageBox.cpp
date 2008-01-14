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
#include "MonkeyCore.h"
#include "UIMain.h"
#include "pMenuBar.h"
#include "pDockToolBar.h"
#include "pDockMessageBox.h"

#include <QIcon>
#include <QTabWidget>

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
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( pDockMessageBox::instance(), infos().Caption, QIcon( ":/icons/console.png" ) );
		// connect signals so we will be able to save/restore state of dock settings
		connect( MonkeyCore::menuBar()->action( "mView/aShowBuild", tr( "Show Build" ), QIcon( ":/icons/tabbuild.png" ), tr( "F10" ) ), SIGNAL( triggered() ), pDockMessageBox::instance(), SLOT( showBuild() ) );
		connect( MonkeyCore::menuBar()->action( "mView/aShowOutput", tr( "Show Output" ), QIcon( ":/icons/taboutput.png" ) , tr( "F11" )), SIGNAL( triggered() ), pDockMessageBox::instance(), SLOT( showOutput() ) );
		connect( MonkeyCore::menuBar()->action( "mView/aShowLog", tr( "Show Log" ), QIcon( ":/icons/tablog.png" ), tr( "F12" ) ), SIGNAL( triggered() ), pDockMessageBox::instance(), SLOT( showLog() ) );
		connect( MonkeyCore::menuBar()->action( "mView/aShowNextError", tr( "Show Next Error" ), QIcon( ":/icons/goto.png" ), "F9" ), SIGNAL( triggered() ), pDockMessageBox::instance(), SLOT( showNextError() ) );
		connect( pDockMessageBox::instance(), SIGNAL( saveSettings() ), this, SLOT( saveSettings() ) );
		connect( pDockMessageBox::instance(), SIGNAL( restoreSettings() ), this, SLOT( restoreSettings() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		delete MonkeyCore::menuBar()->action( "mView/aShowBuild" );
		delete MonkeyCore::menuBar()->action( "mView/aShowOutput" );
		delete MonkeyCore::menuBar()->action( "mView/aShowLog" );
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
	// save current tab index
	setSettingsValue( "CurrentIndex", pDockMessageBox::instance()->twMessageBox->currentIndex() );
}

void MessageBox::restoreSettings()
{
	// restore tab index
	pDockMessageBox::instance()->twMessageBox->setCurrentIndex( settingsValue( "CurrentIndex", 0 ).toInt() );
}

Q_EXPORT_PLUGIN2( BaseMessageBox, MessageBox )
