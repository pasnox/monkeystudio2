/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : ProjectHeaders
** FileName  : ProjectHeaders.cpp
** Date      : 2007-11-04T01:45:50
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "ProjectHeaders.h"
#include "pMenuBar.h"
#include "UIProjectHeaders.h"

ProjectHeaders::ProjectHeaders()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Project Headers" );
	mPluginInfos.Description = tr( "Plugin for managing the license headers of your sources" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = "ProjectHeaders";
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.Enabled = false;
}

ProjectHeaders::~ProjectHeaders()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool ProjectHeaders::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// add dock to dock toolbar entry
		QAction* a = pMenuBar::instance()->action( "mEdit/aProjectHeaders" );
		connect( a, SIGNAL( triggered() ), this, SLOT( processLicensing() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	
	// return default value
	return true;
}

void ProjectHeaders::processLicensing()
{
	UIProjectHeaders d;
	d.exec();
}

Q_EXPORT_PLUGIN2( BaseProjectHeaders, ProjectHeaders )
