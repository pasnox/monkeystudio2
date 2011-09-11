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
#include <xupmanager/core/XUPProjectItemHelper.h>

#include <coremanager/MonkeyCore.h>

void PyQt::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "PyQt Project" );
	mPluginInfos.Description = tr( "PyQt Project support for XUPManager" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase | BasePlugin::iXUP;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = false;
	mPluginInfos.dependencies << "Python";
}

bool PyQt::install()
{
	mFilters = DocumentFilterMap( ":/pyqtitems" );
	int weight = 0;
	
	mFilters[ "PROJECT" ].weight = weight++;
	mFilters[ "PROJECT" ].label = tr( "PyQt Project" );
	mFilters[ "PROJECT" ].icon = "project.png";
	mFilters[ "PROJECT" ].type = DocumentFilter::Project;
	mFilters[ "PROJECT" ].filters << "*.xpyqt";
	
	mFilters[ "FORMS" ].weight = weight++;
	mFilters[ "FORMS" ].label = tr( "Qt Forms" );
	mFilters[ "FORMS" ].icon = "forms.png";
	mFilters[ "FORMS" ].type = DocumentFilter::File;
	mFilters[ "FORMS" ].filters << "*.ui";
	mFilters[ "FORMS" ].filtered = true;
	
	mFilters[ "PYTHON_FILES" ].weight = weight++;
	mFilters[ "PYTHON_FILES" ].label = tr( "Python Sources" );
	mFilters[ "PYTHON_FILES" ].icon = "python.png";
	mFilters[ "PYTHON_FILES" ].type = DocumentFilter::File;
	mFilters[ "PYTHON_FILES" ].filters << "*.py*";
	mFilters[ "PYTHON_FILES" ].filtered = true;
	
	MonkeyCore::projectTypesIndex()->registerType( PLUGIN_NAME, &PyQtProjectItem::staticMetaObject, mFilters );
	return true;
}

bool PyQt::uninstall()
{
	MonkeyCore::projectTypesIndex()->unRegisterType( PLUGIN_NAME );
	mFilters.clear();
	return true;
}

Q_EXPORT_PLUGIN2( ProjectPyQt, PyQt )
