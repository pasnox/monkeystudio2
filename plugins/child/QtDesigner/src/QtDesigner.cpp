/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QtDesigner.cpp
** Date      : 2007-11-04T22:53:21
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
