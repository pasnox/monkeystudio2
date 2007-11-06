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
