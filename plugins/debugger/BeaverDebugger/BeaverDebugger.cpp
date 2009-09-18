/****************************************************************************
**
** Authors   : Andrei KOPATS aka hlamer <hlamer@tut.by>
** Project   : Beaver Debugger plugin
** FileName  : BeaverDebugger.h
** Date      : 
** License   : GPL
** Comment   : 
** Home Page : http://www.monkeystudio.org
**
	Copyright (C) 2005 - 2008  Andrei KOPATS & The Monkey Studio Team

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
/*!
	\file BeaverDebugger.h
	\date 2008-01-14T00:40:08
	\author Filipe AZEVEDO, Andrei KOPATS
	\brief Header file for BeaverDebugger plugin
*/

#include "BeaverDebugger.h"

#include <QIcon>

#include "pMonkeyStudio.h"

BasePlugin::PluginInfos BeaverDebugger::infos() const
{
	PluginInfos pluginInfos;
	pluginInfos.Caption = tr( "Beaver Debugger" );
	pluginInfos.Description = tr( "Plugin for use Beaver Debugger together with MkS" );
	pluginInfos.Author = "Andei Kopats aka hlamer <hlamer@tut.by>";
	pluginInfos.Type = BasePlugin::iDebugger;
	pluginInfos.Name = PLUGIN_NAME;
	pluginInfos.Version = "1.0.0";
	pluginInfos.FirstStartEnabled = true;
	pluginInfos.HaveSettingsWidget = false;
	pluginInfos.Pixmap = QPixmap( ":/icons/beaverdbg.png" );
	
	return pluginInfos;
}

/*!
	Install/uninstall plugin from system
	\param b Flag of action a - install; b - uninstall
	\return Status code of action
	\retval true Successfull
	\retval false Some error ocurred
*/
bool BeaverDebugger::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create action
		MonkeyCore::menuBar()->menu( "mDebugger" )->menuAction()->setVisible( true ); // FIXME is it good?
		MonkeyCore::menuBar()->menu( "mDebugger" )->menuAction()->setEnabled( true ); // FIXME is it good?
		QAction* a = MonkeyCore::menuBar()->action( "mDebugger/aWhyCannot",  
													tr( "Why can't I debug my app" ), 
													QIcon( ":/icons/beaverdbg.png" ), 
													"", // shortcut
													"Check Beaver Debugger status" );
		connect( a, SIGNAL( triggered() ), this, SLOT( explainWhyCannot() ) );
		// set plugin enabled
		stateAction()->setChecked( true );

	}
	else if ( !b && isEnabled() )
	{
		delete MonkeyCore::menuBar()->action( "mDebugger/aWhyCannot");
		
		stateAction()->setChecked( false );
	}
	// return default value
	return true;
}

/*!
	Get settings widget of plugin
	\return Pointer to created settings widget for plugin
*/
QWidget* BeaverDebugger::settingsWidget()
{ return NULL; }

void BeaverDebugger::explainWhyCannot() const
{
	pMonkeyStudio::information("Beaver Debugger", "Beaver Debugger not found", NULL );
}

Q_EXPORT_PLUGIN2( BaseBeaverDebugger, BeaverDebugger )
