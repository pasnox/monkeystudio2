/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
