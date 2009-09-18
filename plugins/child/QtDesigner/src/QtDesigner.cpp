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
#include "QtDesigner.h"
#include "QtDesignerManager.h"
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
	mPluginInfos.FirstStartEnabled = true;
}

QtDesigner::~QtDesigner()
{
	if ( isEnabled() )
	{
		setEnabled( false );
	}
}

QWidget* QtDesigner::settingsWidget()
{
	return BasePlugin::settingsWidget();
}

bool QtDesigner::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// set usable suffixes
		mSuffixes[ tr( "Qt Forms" ) ] = QStringList( "*.ui" );
		// create designer
		mDesignerManager = new QtDesignerManager( this );
		// set plugin enabled
		stateAction()->setChecked( true );
	}
	else if ( !b && isEnabled() )
	{
		// clear suffixes
		mSuffixes.clear();
		// clear designer instance
		delete mDesignerManager;
		// set plugin disabled
		stateAction()->setChecked( false );
	}
	// return default value
	return true;
}

pAbstractChild* QtDesigner::createDocument( const QString& fileName )
{
	if ( canOpen( fileName ) )
	{
		return new QtDesignerChild( mDesignerManager );
	}
	
	return 0;
}

Q_EXPORT_PLUGIN2( BaseQtDesigner, QtDesigner )
