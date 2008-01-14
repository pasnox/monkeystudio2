/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Child Plugins
** FileName  : QtDesigner.cpp
** Date      : 2008-01-14T00:57:16
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
#include "QtDesigner.h"
#include "QtDesignerChild.h"

QtDesigner::QtDesigner()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Qt Designer" );
	mPluginInfos.Description = tr( "This plugin embeds Qt Designer" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Luc Bruant aka fullmetalcoder <fullmetalcoder@hotmail.fr>";
	mPluginInfos.Type = BasePlugin::iChild;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

QtDesigner::~QtDesigner()
{
	if ( isEnabled() )
		setEnabled( false );
}

QWidget* QtDesigner::settingsWidget()
{ return BasePlugin::settingsWidget(); }

bool QtDesigner::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// set usable suffixes
		mSuffixes[tr( "Qt Forms" )] = QStringList( "*.ui" );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// clear suffixes
		mSuffixes.clear();
		// clear designer instance
		QtDesignerChild::cleanInstance();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

pAbstractChild* QtDesigner::openFile( const QString& s, const QPoint& )
{
	QtDesignerChild::instance()->openFile( s );
	return QtDesignerChild::instance();
}

Q_EXPORT_PLUGIN2( BaseQtDesigner, QtDesigner )
