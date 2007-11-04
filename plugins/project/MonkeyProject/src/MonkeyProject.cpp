/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : MonkeyProject.cpp
** Date      : 2007-11-04T22:53:34
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "MonkeyProject.h"
#include "MonkeyProjectItem.h"

#include <QDir>

MonkeyProject::MonkeyProject()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "MonkeyProject" );
	mPluginInfos.Description = tr( "Plugin for managing projects without any project file" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iProject;
	mPluginInfos.Name = "MonkeyProject";
	mPluginInfos.Version = "0.0.1";
	mPluginInfos.Enabled = false;
	mSuffixes["MonkeyProject"] = QStringList("*.monkey");
}

ProjectItem* MonkeyProject::getProjectItem( const QString& s )
{
	// don t open project if plugin is not enabled
	if ( !isEnabled() || !QDir::match( mSuffixes[tr( "MonkeyProject" )], s ) )
		return 0;
	
	// create project item
	ProjectItem* it = new MonkeyProjectItem( ProjectItem::ProjectType );
	
	// set project filename
	it->setValue( s );
	
	// set item plugin
	it->setPlugin( this );
	
	// return item
	return it;
}

bool MonkeyProject::setEnabled (bool en)
{
	mPluginInfos.Enabled = en;
	return true;
};

Q_EXPORT_PLUGIN2( ProjectMonkeyProject, MonkeyProject )
