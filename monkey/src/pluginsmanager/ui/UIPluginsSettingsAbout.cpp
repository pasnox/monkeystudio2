#include "pluginsmanager/ui/UIPluginsSettingsAbout.h"
#include "pluginsmanager/BasePlugin.h"

#include <QStringListModel>

UIPluginsSettingsAbout::UIPluginsSettingsAbout( BasePlugin* plugin, QWidget* p )
	: QDialog( p )
{
	Q_ASSERT( plugin );
	const BasePlugin::PluginInfos infos = plugin->infos();
	
	// setup dialog
	setupUi( this );
	setWindowTitle( windowTitle().arg( infos.Caption ) );
	
	if ( !infos.Pixmap.isNull() ) {
		lIcon->setPixmap( infos.Pixmap );
	}
	
	lIcon->setPixmap( lIcon->pixmap()->scaled( QSize( 48, 48 ), Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
	lCaption->setText( infos.Caption );
	lVersion->setText( infos.Version );
	lLicense->setText( infos.License.isEmpty() ? tr( "GNU General Public License" ) : infos.License );
	lDescription->setText( infos.Description );
	
	lvAuthors->setModel( new QStringListModel( trimmedSplitedString( infos.Author ) ) );
	lvTypes->setModel( new QStringListModel( trimmedSplitedString( plugin->completeTypeToString( infos.Type ) ) ) );
	lvLanguages->setModel( new QStringListModel( infos.Languages ) );
	lvDependencies->setModel( new QStringListModel( infos.dependencies ) );
}

QStringList UIPluginsSettingsAbout::trimmedSplitedString( const QString& string ) const
{
	QStringList entries = string.split( "," );
	
	for ( int i = 0; i < entries.count(); i++ ) {
		QString& entry = entries[ i ];
		entry = entry.trimmed();
	}
	
	return entries;
}
