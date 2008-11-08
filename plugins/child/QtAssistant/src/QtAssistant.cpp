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
#include "QtAssistantChild.h"
#include "ui/QtAssistantDock.h"
#include "ui/UIQtAssistantSettings.h"
#include "3rdparty/preferencesdialog.h"

#include <UIMain.h>

#include <QDebug>

QtAssistant::QtAssistant()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Qt Assistant" );
	mPluginInfos.Description = tr( "This plugin embedded Qt Assistant" );
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
{
	if ( !isEnabled() )
		return 0;
	return new PreferencesDialog( mAssistantDock->helpEngine(), QApplication::activeWindow(), false );
}

bool QtAssistant::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		mAssistantDock = new QtAssistantDock;
		MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( mAssistantDock, infos().Caption, QIcon( ":/icons/assistant.png" ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		mAssistantDock->deleteLater();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	else
		return false;
	// return default value
	return true;
}

pAbstractChild* QtAssistant::openFile( const QString& /*filename*/, const QPoint& /*pos*/ )
{
	return 0;
}

Q_EXPORT_PLUGIN2( ChildQtAssistant, QtAssistant )
