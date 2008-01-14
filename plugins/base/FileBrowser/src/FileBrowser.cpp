/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : FileBrowser.cpp
** Date      : 2008-01-14T00:39:54
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
#include "FileBrowser.h"
#include "UIMain.h"
#include "pDockToolBar.h"
#include "pDockFileBrowser.h"
#include "FileBrowserSettings.h"
#include "MonkeyCore.h"

#include <QIcon>

FileBrowser::FileBrowser()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "File Browser" );
	mPluginInfos.Description = tr( "Plugin for browsing file outside the project" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Kopats Andei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = "FileBrowser";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

FileBrowser::~FileBrowser()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool FileBrowser::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// add dock to dock toolbar entry
		MonkeyCore::mainWindow()->dockToolBar( Qt::LeftToolBarArea )->addDock( pDockFileBrowser::instance(), infos().Caption, QIcon( ":/icons/browser.png" ) );
		// connect signals so we will be able to save/restore state of dock settings
		connect( pDockFileBrowser::instance(), SIGNAL( saveSettings() ), this, SLOT( saveSettings() ) );
		connect( pDockFileBrowser::instance(), SIGNAL( restoreSettings() ), this, SLOT( restoreSettings() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// it will remove itself from dock toolbar when deleted
		pDockFileBrowser::instance()->deleteLater();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

QWidget* FileBrowser::settingsWidget()
{ return new FileBrowserSettings(); }

void FileBrowser::saveSettings()
{
	setSettingsValue( "Path", pDockFileBrowser::instance()->currentPath() );
	setSettingsValue( "Wildcards", pDockFileBrowser::instance()->wildcards() );
}

void FileBrowser::restoreSettings()
{
	QString s = settingsValue( "Path" ).toString();
	if ( !s.isEmpty() )
		pDockFileBrowser::instance()->setCurrentPath( s );
	pDockFileBrowser::instance()->setWildcards( settingsValue( "Wildcards", QStringList() << "*~" << "*.o" << "*.pyc" << "*.bak" ).toStringList() );
}

Q_EXPORT_PLUGIN2( BaseFileBrowser, FileBrowser )
