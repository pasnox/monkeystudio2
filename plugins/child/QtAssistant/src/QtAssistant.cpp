/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Child Plugins
** FileName  : QtAssistant.cpp
** Date      : 2008-01-14T00:57:10
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
#include "QtAssistant.h"
#include "QtAssistantChild.h"
#include "QtAssistantConfig.h"
#include "ui/UIQtAssistantSettings.h"

#include <monkey.h>
#include <maininterface.h>
#include <queuedstatusbar.h>

QtAssistant::QtAssistant()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Qt Assistant" );
	mPluginInfos.Description = tr( "This plugin embbeded the official Qt Asssitant" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iChild;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

QtAssistant::~QtAssistant()
{
	if ( isEnabled() )
		setEnabled( false );
}

QWidget* QtAssistant::settingsWidget()
{ return new UIQtAssistantSettings( this ); }

bool QtAssistant::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// load configuration
		Config* conf = QtAssistantConfig::instance( this );
		if ( !conf )
		{
			MonkeyCore::statusBar()->appendMessage( tr( "Can't load/create the default profile for Qt Assistant, aborting..." ), 2500 );
			return false;
		}
		// install dock
		MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( QtAssistantChild::instance()->dock(), infos().Caption, QIcon( ":/trolltech/assistant/images/assistant.png" ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		QtAssistantChild::cleanInstance();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

Q_EXPORT_PLUGIN2( ChildQtAssistant, QtAssistant )
