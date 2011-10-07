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
#include "UISettingsClassBrowser.h"
#include "ui_UISettingsClassBrowser.h"
#include "ClassBrowser.h"

#include <QFileDialog>

UISettingsClassBrowser::UISettingsClassBrowser( ClassBrowser* plugin, QWidget* parent )
    : QWidget( parent ), mPlugin( plugin ), ui( new Ui_UISettingsClassBrowser )
{
    qCtagsSenseProperties properties = mPlugin->properties();
    
    ui->setupUi( this );
    
    ui->cbIntegrationMode->addItem( tr( "Dock" ), ClassBrowser::imDock );
    ui->cbIntegrationMode->addItem( tr( "Combo" ), ClassBrowser::imCombo );
    ui->cbIntegrationMode->addItem( tr( "Both" ), ClassBrowser::imBoth );
    ui->cbIntegrationMode->setCurrentIndex( ui->cbIntegrationMode->findData( plugin->integrationMode() ) );
    
    ui->gbUseDBFileName->setChecked( properties.UsePhysicalDatabase );
    ui->leDBFileName->setText( properties.DatabaseFileName );
    ui->pePaths->setValues( properties.SystemPaths );
    ui->sleSuffixes->setValues( properties.FilteredSuffixes );
}

UISettingsClassBrowser::~UISettingsClassBrowser()
{
    delete ui;
}

void UISettingsClassBrowser::on_tbDBFileName_clicked()
{
    const QString fn = QFileDialog::getSaveFileName( this, tr( "Select a filename to use for the temporary database" ), ui->leDBFileName->text() );
    
    if ( !fn.isNull() ) {
        ui->leDBFileName->setText( fn );
    }
}

void UISettingsClassBrowser::on_dbbButtons_clicked( QAbstractButton* button )
{
    Q_UNUSED( button );
    qCtagsSenseProperties properties;
    properties.SystemPaths = ui->pePaths->values();
    properties.FilteredSuffixes = ui->sleSuffixes->values();
    properties.UsePhysicalDatabase = ui->gbUseDBFileName->isChecked();
    properties.DatabaseFileName = ui->leDBFileName->text();
    
    mPlugin->setIntegrationMode( (ClassBrowser::IntegrationMode)ui->cbIntegrationMode->itemData( ui->cbIntegrationMode->currentIndex() ).toInt() );
    mPlugin->setProperties( properties );
}
