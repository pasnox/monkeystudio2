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
#include "PyQt.h"
#include "PyQtProjectItem.h"
#include "../XUP/src/gui/UIXUPEditor.h"

#include <MonkeyCore.h>
#include <UIMain.h>

#include <QDir>

PyQt::PyQt()
{
	mItem = 0;
}

void PyQt::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "PyQt Project" );
	mPluginInfos.Description = tr( "PyQt Project support for XUPManager" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Michon Aurelien aka aurelien <aurelien.french@gmail.com>";
	mPluginInfos.Type = BasePlugin::iXUP;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.FirstStartEnabled = true;
}

bool PyQt::setEnabled( bool enabled )
{
	if ( enabled && !isEnabled() )
	{
		// register pythonqt item
		mItem = new PyQtProjectItem;
		mItem->registerProjectType();

		// set plugin enabled
		stateAction()->setChecked( true );
	}
	else if ( !enabled && isEnabled() )
	{
		// unregister item, unregistering auto delete the item
		mItem->unRegisterProjectType();
		delete mItem;

		// set plugin disabled
		stateAction()->setChecked( false );
	}

	// return default value
	return true;
}

bool PyQt::editProject( XUPProjectItem* project )
{
	if ( !project )
	{
		return false;
	}

	return UIXUPEditor( project, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
}

Q_EXPORT_PLUGIN2( ProjectPyQt, PyQt )