/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : ProjectHeaders.cpp
** Date      : 2008-01-14T00:40:14
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
#include "ProjectHeaders.h"
#include "MonkeyCore.h"
#include "pMenuBar.h"
#include "UIProjectHeaders.h"
#include "pMonkeyStudio.h"

using namespace pMonkeyStudio;

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
		QAction* a = MonkeyCore::menuBar()->action( "mEdit/aProjectHeaders", tr( "Project Licensing..." ), QIcon( ":/icons/licensing.png" ) );
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

void ProjectHeaders::setTemplatesHeaderRegExp( const QString& l, const QString& s )
{ setSettingsValue( QString( "RegExp/" ).append( l ), s ); }

QString ProjectHeaders::templatesHeaderRegExp( const QString& l ) const
{
	QString s = settingsValue( QString( "RegExp/" ).append( l ), QString() ).toString();
	return s.isEmpty() ? defaultTemplatesHeaderRegExp( l ) : s;
}

QString ProjectHeaders::defaultTemplatesHeaderRegExp( const QString& l ) const
{
	QString s;
	
	if ( l == "C++" )
		s = "^(/\\*[^$]*\\*/)";
	else if ( l == "HTML" )
		s = "^(<\\!--[^$]*-->)";
	
	// default
	return s;
}

void ProjectHeaders::setTemplatesHeader( const QString& l, const QString& s )
{ setSettingsValue( QString( "Licensing/" ).append( l ), s ); }

QString ProjectHeaders::templatesHeader( const QString& l ) const
{
	QString s = settingsValue( QString( "Licensing/" ).append( l ), QString() ).toString();
	return s.isEmpty() ? defaultTemplatesHeader( l ) : s;
}

QString ProjectHeaders::defaultTemplatesHeader( const QString& l ) const
{
	QString s;
	
	if ( l == "C++" )
		s = "/****************************************************************************\n"
			"**\n"
			"** 		Created using $editor_version_string$\n"
			"** Authors    : $authors$\n"
			"** Project   : $projectname$\n"
			"** FileName  : $filename$\n"
			"** Date      : $date$\n"
			"** License   : $license$\n"
			"** Comment   : $comment$\n"
			"** Home Page : $homepage$\n"
			"**\n"
			"** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE\n"
			"** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n"
			"**\n"
			"****************************************************************************/\n";
	else if ( l == "HTML" )
		s = "<!--\n"
			"/****************************************************************************\n"
			"**\n"
			"** 		Created using $editor_version_string$\n"
			"** Authors    : $authors$\n"
			"** Project   : $projectname$\n"
			"** FileName  : $filename$\n"
			"** Date      : $date$\n"
			"** License   : $license$\n"
			"** Comment   : $comment$\n"
			"** Home Page : $homepage$\n"
			"**\n"
			"** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE\n"
			"** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n"
			"**\n"
			"****************************************************************************/\n"
			"-->\n";
	
	// default
	return s;
}

void ProjectHeaders::processLicensing()
{ UIProjectHeaders( 0, this ).exec(); }

Q_EXPORT_PLUGIN2( BaseProjectHeaders, ProjectHeaders )
