/****************************************************************************
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
****************************************************************************/
#include "QtAssistant.h"
#include "ui/QtAssistantDock.h"
#include "ui/UIQtAssistantSettings.h"
#include "3rdparty/preferencesdialog.h"

#include <UIMain.h>

#include <QDebug>

void QtAssistant::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "Qt Assistant" );
	mPluginInfos.Description = tr( "This plugin embedded Qt Assistant" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iChild;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = true;
	mPluginInfos.Pixmap = pIconManager::pixmap( "assistant.png", ":/icons" );
}

QWidget* QtAssistant::settingsWidget()
{
	return new PreferencesDialog( mAssistantDock->helpEngine(), QApplication::activeWindow(), false );
}

bool QtAssistant::install()
{
	mAssistantDock = new QtAssistantDock;
	MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( mAssistantDock, infos().Caption, QIcon( ":/icons/assistant.png" ) );
	return true;
}

bool QtAssistant::uninstall()
{
	mAssistantDock->deleteLater();
	return true;
}

pAbstractChild* QtAssistant::createDocument( const QString& fileName )
{
	Q_UNUSED( fileName );
	return 0;
}

Q_EXPORT_PLUGIN2( ChildQtAssistant, QtAssistant )