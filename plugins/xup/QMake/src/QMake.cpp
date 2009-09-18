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
#include "QMake.h"
#include "QMakeProjectItem.h"
#include "UISettingsQMake.h"
#include "UISimpleQMakeEditor.h"

#include <MonkeyCore.h>
#include <UIMain.h>

#include <QDir>

QMake::QMake()
{
	mItem = 0;
}

BasePlugin::PluginInfos QMake::infos() const
{
	PluginInfos pluginInfos;
	pluginInfos.Caption = tr( "QMake Project" );
	pluginInfos.Description = tr( "QMake Project support for XUPManager" );
	pluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Roper Alexander aka minirop <minirop@peyj.com>";
	pluginInfos.Type = BasePlugin::iXUP;
	pluginInfos.Name = PLUGIN_NAME;
	pluginInfos.Version = "0.1.0";
	pluginInfos.FirstStartEnabled = true;
	pluginInfos.HaveSettingsWidget = true;
	
	return pluginInfos;
}

bool QMake::setEnabled( bool enabled )
{
	if ( enabled && !isEnabled() )
	{
		// register qmake item
		mItem = new QMakeProjectItem;
		mItem->registerProjectType();
		
		// set plugin enabled
		stateAction()->setChecked( true );
	}
	else if ( !enabled && isEnabled() )
	{
		// unregister qmake item, unregistering auto delete the item
		mItem->unRegisterProjectType();
		delete mItem;
		
		// set plugin disabled
		stateAction()->setChecked( false );
	}
	
	// return default value
	return true;
}

QWidget* QMake::settingsWidget()
{ return new UISettingsQMake(); }

bool QMake::editProject( XUPProjectItem* project )
{
	if ( !project )
	{
		return false;
	}
	
	return UISimpleQMakeEditor( project, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
}

Q_EXPORT_PLUGIN2( ProjectQMake, QMake )
