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
#include "UIPluginsSettingsElement.h"
#include "pluginsmanager/ui/UIPluginsSettingsAbout.h"
#include "pluginsmanager/BasePlugin.h"
#include "coremanager/MonkeyCore.h"
#include "settingsmanager/Settings.h"

#include <QDesktopWidget>

UIPluginsSettingsElement::UIPluginsSettingsElement( BasePlugin* plugin, QWidget* p )
    : QWidget( p )
{
    Q_ASSERT( plugin );
    mPlugin = plugin;
    BasePlugin::PluginInfos infos = mPlugin->infos();
    
    // setup dialog
    setupUi( this );
    cbEnabled->setChecked( mPlugin->isEnabled() );
    
    if ( !mPlugin->infos().Pixmap.isNull() )
    {
        lIcon->setPixmap( mPlugin->infos().Pixmap.scaledToWidth( lIcon->maximumWidth(), Qt::SmoothTransformation ) );
    }
    
    lIcon->setEnabled( cbEnabled->isChecked() );
    lTitle->setText( infos.Caption );
    lDescription->setText( infos.Description );
    pbSettings->setVisible( mPlugin->infos().HaveSettingsWidget );
    cbNeverEnable->setChecked( mPlugin->neverEnable() );
}

BasePlugin* UIPluginsSettingsElement::plugin() const
{ return mPlugin; }

void UIPluginsSettingsElement::on_cbEnabled_toggled( bool checked )
{
    if ( checked )
        cbNeverEnable->setChecked( false );
    lIcon->setEnabled( checked );
    mPlugin->setEnabled( checked );
    MonkeyCore::settings()->setValue( QString( "Plugins/%1" ).arg( mPlugin->infos().Name ), checked );
}

void UIPluginsSettingsElement::on_pbSettings_clicked()
{
    QWidget* window = qApp->activeWindow();
    QWidget* widget = mPlugin->settingsWidget();
    
#ifdef Q_OS_MAC
#if QT_VERSION >= 0x040500
    widget->setParent( window, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint );
#else
    widget->setParent( window, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowSystemMenuHint );
#endif
#else
    widget->setParent( window, Qt::Dialog );
#endif
    widget->setWindowModality( Qt::ApplicationModal );
    widget->setAttribute( Qt::WA_DeleteOnClose );
    widget->setWindowIcon( mPlugin->infos().Pixmap );
    widget->setWindowTitle( tr( "Settings - %1" ).arg( mPlugin->infos().Caption ) );
    
    widget->show();
}

void UIPluginsSettingsElement::on_pbAbout_clicked()
{
    UIPluginsSettingsAbout( mPlugin, qApp->activeWindow() ).exec();
}

void UIPluginsSettingsElement::on_cbNeverEnable_toggled( bool checked )
{
    if ( checked )
        cbEnabled->setChecked( false );
    mPlugin->setNeverEnable( checked );
}
