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

void BeaverDebugger::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "Beaver Debugger" );
	mPluginInfos.Description = tr( "Plugin for use Beaver Debugger together with MkS" );
	mPluginInfos.Author = "Andei Kopats aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iDebugger;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = false;
	mPluginInfos.Pixmap = QPixmap( ":/icons/beaverdbg.png" );
}

/*!
	Install plugin to the system
	\return Status code of action
	\retval true Successfull
	\retval false Some error ocurred
*/
bool BeaverDebugger::install()
{
	// create action
	MonkeyCore::menuBar()->menu( "mDebugger" )->menuAction()->setVisible( true ); // FIXME is it good?
	MonkeyCore::menuBar()->menu( "mDebugger" )->menuAction()->setEnabled( true ); // FIXME is it good?
	
	if (0) // FIXME debugger found
	{
		mWhyCannot = NULL;
	}
	else // debugger not found
	{
		mWhyCannot = MonkeyCore::menuBar()->action( "mDebugger/aWhyCannot",  
													tr( "Why can't I debug my app" ), 
													QIcon( ":/icons/beaverdbg.png" ), 
													"", // shortcut
													"Check Beaver Debugger status" );
	}
	connect( mWhyCannot, SIGNAL( triggered() ), this, SLOT( explainWhyCannot() ) );
	return true;
}

/*!
	Unnstall plugin from the system
	\return Status code of action
	\retval true Successfull
	\retval false Some error ocurred
*/
bool BeaverDebugger::uninstall()
{
	if (mWhyCannot)
		delete mWhyCannot;
	
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
	QString fullText;
	fullText += tr("Beaver debugger not found\n");
	fullText += tr("It might be not installed in the system, or path to it configured not correctly\n");
	fullText += tr("For install, download it from http://beaverdbg.googlecode.com and follow installation instructions\n");
	fullText += tr("If you use Linux distribution, Beaver Debugger might be included to it. Package name probably is 'beaverdbg'\n");
	fullText += "\n";
	fullText += tr("If Beaver Debugger is installed, but not found, go to \n");
	fullText += tr("Plugins->Manage->Debugger->Beaver Debugger->Configure...\n");
	fullText += tr("and configure path to it");
	pMonkeyStudio::information(tr("Beaver Debugger not found"), fullText, NULL);;
}

Q_EXPORT_PLUGIN2( BaseBeaverDebugger, BeaverDebugger )
