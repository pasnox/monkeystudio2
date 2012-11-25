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
#include "UISettingsQMake.h"
#include "ui_UISettingsQMake.h"
#include "../QMake.h"
#include "../QtVersionManager.h"

#include <pGenericTableModel.h>
#include <shared/FileSystemModel.h>

#include <QPushButton>
#include <QDir>
#include <QDirModel>
#include <QCompleter>
#include <QMessageBox>
#include <QScrollBar>
#include <QFileDialog>
#include <QDebug>

UISettingsQMake::UISettingsQMake( QWidget* parent )
    : QWidget( parent ), ui( new Ui_UISettingsQMake )
{
    mBackground = QColor( "#A8DFA8" );
    mForeground = palette().brush( QPalette::Active, QPalette::Text );
    
    // set up dialog
    ui->setupUi( this );
    
    mQtManager = QMake::versionManager();
    mQtVersionsModel = new pGenericTableModel( this );
    mQtModulesModel = new pGenericTableModel( this );
    mQtConfigurationsModel = new pGenericTableModel( this );
    
    ui->lvQtVersions->setModel( mQtVersionsModel );
    ui->lvQtModules->setModel( mQtModulesModel );
    ui->lvQtConfigurations->setModel( mQtConfigurationsModel );
    
    connect( ui->lvQtVersions->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvQtVersions_selectionModel_selectionChanged( const QItemSelection&, const QItemSelection& ) ) );
    connect( ui->leQtVersionPath, SIGNAL( editingFinished() ), this, SLOT( updateMkSpecsEntries() ) );
    connect( ui->lvQtModules->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvQtModules_selectionModel_selectionChanged( const QItemSelection&, const QItemSelection& ) ) );
    connect( ui->lvQtConfigurations->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvQtConfigurations_selectionModel_selectionChanged( const QItemSelection&, const QItemSelection& ) ) );
    
    loadSettings();
    ui->lwPages->setCurrentRow( 0 );
    
    // completer of paths
    QCompleter* completer = new QCompleter( ui->leQtVersionPath );
    FileSystemModel* model = new FileSystemModel( completer );
    model->setRootPath( QString::null );
    model->setFilter( QDir::AllDirs | QDir::NoDotAndDotDot );
    completer->setModel( model );
    ui->leQtVersionPath->setCompleter( completer );
    
#ifdef Q_OS_MAC
    foreach ( QWidget* widget, findChildren<QWidget*>() ) {
        widget->setAttribute( Qt::WA_MacSmallSize );
    }
#endif
}

UISettingsQMake::~UISettingsQMake()
{
    delete ui;
}

void UISettingsQMake::on_lwPages_currentRowChanged( int row )
{
    QListWidgetItem* item = ui->lwPages->item( row );
    ui->lTitle->setText( item ? item->text() : QString::null );
    ui->lIcon->setPixmap( item ? item->icon().pixmap( QSize( 18, 18 ) ) : QPixmap() );
    ui->swPages->setCurrentIndex( row );
}

void UISettingsQMake::updateMkSpecsEntries( const QString& _currentMkSpec )
{
    const QString currentMkSpec = _currentMkSpec == "#null" ? ui->cbQtVersionQMakeSpec->currentText() : _currentMkSpec;
    const QDir mkspecsDir( QString( ui->leQtVersionPath->text() ).append( "/mkspecs" ) );
    QStringList mkspecs;
    
    if ( mkspecsDir.exists() ) {
        foreach ( const QFileInfo& fi, mkspecsDir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name ) ) {
            if ( fi.fileName() != "common" && fi.fileName() != "features" ) {
                mkspecs << fi.fileName();
            }
        }
    }
    
    if ( !currentMkSpec.isEmpty() && !mkspecs.contains( currentMkSpec ) ) {
        mkspecs << currentMkSpec;
    }
    
    mkspecs.sort();
    
    ui->cbQtVersionQMakeSpec->clear();
    ui->cbQtVersionQMakeSpec->addItems( mkspecs );
    ui->cbQtVersionQMakeSpec->setCurrentIndex( ui->cbQtVersionQMakeSpec->findText( currentMkSpec ) );
}

void UISettingsQMake::loadSettings()
{
    // qt versions
    const QtVersionList versions = mQtManager->versions();
    
    mQtVersionsModel->setColumnCount( 1 );
    mQtVersionsModel->setRowCount( versions.count() );
    
    for ( int i = 0; i < versions.count(); i++ ) {
        const QtVersion& version = versions[ i ];
        const QModelIndex index = mQtVersionsModel->index( i, 0 );
        QFont font( index.data( Qt::FontRole ).value<QFont>() );
    
        font.setBold( version.Default );
        
        mQtVersionsModel->setData( index, version.Version, Qt::DisplayRole );
        mQtVersionsModel->setData( index, font.bold() ? font : QVariant(), Qt::FontRole );
        mQtVersionsModel->setData( index, QVariant::fromValue( version ), pGenericTableModel::ExtendedUserRole );
        
        if ( version.Default ) {
            mQtVersionsModel->setData( index, mBackground, Qt::BackgroundRole );
        }
    }
    
    lvQtVersions_selectionModel_selectionChanged( QItemSelection(), QItemSelection() );
    
    ui->pteQtVersionsHelp->appendHtml( 
        tr( "You can register one or more Qt Version to use in your Qt projects, so you can easily select the one to use in project settings.<br /><br />"
            "The colored item is the default Qt Version used. if there is no colored item, the default Qt Version used will be the first one available. You can explicitly set the default Qt Version selecting an item and clicking the set default button.<br /><br />"
            "To add a new Qt version, simply click the <b>Add a new Qt Version</b> button at top and fill needed fields.<br /><br />"
            "The minimum required fields are:<br />"
            "- <b>Version</b>: it define a human label across a Qt version.<br />"
            "- <b>Path</b>: it define the path where is located your Qt installation (the path from where you can see bin/qmake).<br /><br />"
            "You can get more help about fields reading there tooltips." )
    );
    
    goAtDocumentStart( ui->pteQtVersionsHelp );
    
    // qt modules
    const QtItemList modules = mQtManager->modules();
    
    mQtModulesModel->setColumnCount( 1 );
    mQtModulesModel->setRowCount( modules.count() );
    
    for ( int i = 0; i < modules.count(); i++ ) {
        const QtItem& item = modules[ i ];
        const QModelIndex index = mQtModulesModel->index( i, 0 );
        QFont font( index.data( Qt::FontRole ).value<QFont>() );
        
        font.setBold( item.Value.isEmpty() && item.Variable.isEmpty() );
        
        mQtModulesModel->setData( index, item.Text, Qt::DisplayRole );
        mQtModulesModel->setData( index, font, Qt::FontRole );
        mQtModulesModel->setData( index, font.bold() ? mBackground : QVariant(), Qt::BackgroundRole );
        mQtModulesModel->setData( index, font.bold() ? mForeground : QVariant(), Qt::ForegroundRole );
        mQtModulesModel->setData( index, QVariant::fromValue( item ), pGenericTableModel::ExtendedUserRole );
    }
    
    lvQtModules_selectionModel_selectionChanged( QItemSelection(), QItemSelection() );
    
    ui->pteQtModulesHelp->appendHtml( 
        tr( "You can register one or more Qt Modules for your Qt projects, so you can easily use them in the project settings dialog.<br />"
            "Qt Modules are components available by your Qt installation, like QtCore, GtGui...<br />"
            "This editor allow you to edit the available modules in case of by example a new Qt version is released and MkS did not yet support the new modules in the project settings.<br />"
            "A concrete example is the release of Qt 4.6.0 that has added QtMultimedia, you had notified that this module was not available in the project settings, so you can't use it.<br />"
            "By adding a new module by clicking <b>Add a new module</b> button, you can define the module caption and its associated value, this will make it available in the project settings !<br />"
            "The minimum required fields are <b>caption</b> and <b>value</b>, while <b>help</b> is an optional description of the module and will be shown as tooltip in the project settings.<br />"
            "Typically, the module value goes into the QT variable of your project file." )
    );
    
    goAtDocumentStart( ui->pteQtModulesHelp );
    
    // qt configuration
    const QtItemList configuration = mQtManager->configurations();
    
    mQtConfigurationsModel->setColumnCount( 1 );
    mQtConfigurationsModel->setRowCount( configuration.count() );
    
    for ( int i = 0; i < configuration.count(); i++ ) {
        const QtItem& item = configuration[ i ];
        const QModelIndex index = mQtConfigurationsModel->index( i, 0 );
        QFont font( index.data( Qt::FontRole ).value<QFont>() );
        
        font.setBold( item.Value.isEmpty() && item.Variable.isEmpty() );
        
        mQtConfigurationsModel->setData( index, item.Text, Qt::DisplayRole );
        mQtConfigurationsModel->setData( index, font, Qt::FontRole );
        mQtConfigurationsModel->setData( index, font.bold() ? mBackground : QVariant(), Qt::BackgroundRole );
        mQtConfigurationsModel->setData( index, font.bold() ? mForeground : QVariant(), Qt::ForegroundRole );
        mQtConfigurationsModel->setData( index, QVariant::fromValue( item ), pGenericTableModel::ExtendedUserRole );
    }
    
    lvQtConfigurations_selectionModel_selectionChanged( QItemSelection(), QItemSelection() );
    
    ui->pteQtConfigurationHelp->appendHtml( 
        tr( "Qt Configuration works like <b>Qt Modules</b> except that the content is shown in the <b>Others Modules</b> list and that values goes into the CONFIG variable of your project.<br /><br />"
            "Configurations having the word '<b>only</b>' as caption will be considerated as group separators and must have no value associated (they will be ignored)." )
    );
    
    goAtDocumentStart( ui->pteQtConfigurationHelp );
}

void UISettingsQMake::goAtDocumentStart( QPlainTextEdit* pte )
{
    QTextCursor cursor = pte->textCursor();
    cursor.movePosition( QTextCursor::Start );
    pte->setTextCursor( cursor );
}

// qt versions

void UISettingsQMake::setQtVersion( const QModelIndex& index )
{
    if ( !index.isValid() ) {
        return;
    }
    
    QtVersion version = mQtVersionsModel->data( index, pGenericTableModel::ExtendedUserRole ).value<QtVersion>();
    
    version.Version = ui->leQtVersionVersion->text();
    version.Path = ui->leQtVersionPath->text();
    version.QMakeSpec = ui->cbQtVersionQMakeSpec->currentText();
    version.QMakeParameters = ui->leQtVersionQMakeParameters->text();
    version.HasQt4Suffix = ui->cbQtVersionHasSuffix->isChecked();
    
    mQtVersionsModel->setData( index, version.Version, Qt::DisplayRole );
    mQtVersionsModel->setData( index, QVariant::fromValue( version ), pGenericTableModel::ExtendedUserRole );
}

void UISettingsQMake::getQtVersion( const QModelIndex& index )
{
    const QtVersion version = mQtVersionsModel->data( index, pGenericTableModel::ExtendedUserRole ).value<QtVersion>();
    
    ui->leQtVersionVersion->setText( version.Version );
    ui->leQtVersionPath->setText( version.Path );
    updateMkSpecsEntries( version.QMakeSpec );
    ui->leQtVersionQMakeParameters->setText( version.QMakeParameters );
    ui->cbQtVersionHasSuffix->setChecked( version.HasQt4Suffix );
    
    ui->wQtVersion->setEnabled( index.isValid() );
}

void UISettingsQMake::updateQtVersionState()
{
    const QModelIndex index = ui->lvQtVersions->selectionModel()->selectedIndexes().value( 0 );
    const int count = mQtVersionsModel->rowCount( index.parent() );
    
    ui->tbRemoveQtVersion->setEnabled( index.isValid() );
    ui->tbClearQtVersions->setEnabled( count > 0 );
    ui->tbUpQtVersion->setEnabled( index.isValid() && index.row() > 0 && count > 1 );
    ui->tbDownQtVersion->setEnabled( index.isValid() && index.row() < count -1 && count > 1 );
    ui->tbDefaultQtVersion->setEnabled( index.isValid() );
}

void UISettingsQMake::lvQtVersions_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
    const QModelIndex oldIndex = deselected.indexes().value( 0 );
    const QModelIndex newIndex = selected.indexes().value( 0 );
    
    setQtVersion( oldIndex );
    getQtVersion( newIndex );
    updateQtVersionState();
}

void UISettingsQMake::on_tbAddQtVersion_clicked()
{
    const int count = mQtVersionsModel->rowCount();
    mQtVersionsModel->insertRow( count );
    const QModelIndex index = mQtVersionsModel->index( count, 0 );
    
    if ( index.isValid() ) {
        const QtVersion version( tr( "New Qt version" ) );
        mQtVersionsModel->setData( index, version.Version, Qt::DisplayRole );
        mQtVersionsModel->setData( index, QVariant::fromValue( version ), pGenericTableModel::ExtendedUserRole );
        ui->lvQtVersions->setCurrentIndex( index );
        ui->lvQtVersions->scrollTo( index, QAbstractItemView::EnsureVisible );
    }
}

void UISettingsQMake::on_tbRemoveQtVersion_clicked()
{
    const QModelIndex index = ui->lvQtVersions->selectionModel()->selectedIndexes().value( 0 );
    mQtVersionsModel->removeRow( index.row() );
}

void UISettingsQMake::on_tbClearQtVersions_clicked()
{
    mQtVersionsModel->setRowCount( 0 );
    updateQtVersionState();
}

void UISettingsQMake::on_tbUpQtVersion_clicked()
{
    const QModelIndex index = ui->lvQtVersions->selectionModel()->selectedIndexes().value( 0 );
    mQtVersionsModel->swapRows( index.row(), index.row() -1 );
    updateQtVersionState();
}

void UISettingsQMake::on_tbDownQtVersion_clicked()
{
    const QModelIndex index = ui->lvQtVersions->selectionModel()->selectedIndexes().value( 0 );
    mQtVersionsModel->swapRows( index.row(), index.row() +1 );
    updateQtVersionState();
}

void UISettingsQMake::on_tbDefaultQtVersion_clicked()
{
    const QModelIndex index = ui->lvQtVersions->selectionModel()->selectedIndexes().value( 0 );
    QFont font( index.data( Qt::FontRole ).value<QFont>() );
    
    font.setBold( true );
    
    for ( int i = 0; i < mQtVersionsModel->rowCount(); i++ ) {
        const QModelIndex idx = mQtVersionsModel->index( i, 0 );
        mQtVersionsModel->setData( idx, idx == index ? font : QVariant(), Qt::FontRole );
        mQtVersionsModel->setData( idx, idx == index ? mBackground : QVariant(), Qt::BackgroundRole );
    }
}

void UISettingsQMake::on_tbQtVersionPath_clicked()
{
    const QString s = QFileDialog::getExistingDirectory( window(), tr( "Locate your qt installation directory" ), ui->leQtVersionPath->text() );
    
    if ( !s.isEmpty() ) {
        ui->leQtVersionPath->setText( s );
    }
}

void UISettingsQMake::on_tbQtVersionQMakeSpec_clicked()
{
    const QString s = QFileDialog::getExistingDirectory( window(), tr( "Locate the mk spec folder to use" ), ui->leQtVersionPath->text() );
    
    if ( !s.isEmpty() ) {
        if ( ui->cbQtVersionQMakeSpec->findText( s ) == -1 ) {
            ui->cbQtVersionQMakeSpec->addItem( s );
        }
        
        ui->cbQtVersionQMakeSpec->setCurrentIndex( ui->cbQtVersionQMakeSpec->findText( s ) );
    }
}

// qt modules

void UISettingsQMake::setQtModule( const QModelIndex& index )
{
    if ( !index.isValid() ) {
        return;
    }
    
    QtItem item = mQtModulesModel->data( index, pGenericTableModel::ExtendedUserRole ).value<QtItem>();
    QFont font( index.data( Qt::FontRole ).value<QFont>() );
    
    item.Text = ui->leCaptionQtModule->text();
    item.Value = ui->leValueQtModule->text();
    item.Variable = ui->leVariableQtModule->text();
    item.Help = ui->pteHelpQtModule->toPlainText();
    
    font.setBold( item.Value.isEmpty() && item.Variable.isEmpty() );
    
    mQtModulesModel->setData( index, item.Text, Qt::DisplayRole );
    mQtModulesModel->setData( index, font.bold() ? font : QVariant(), Qt::FontRole );
    mQtModulesModel->setData( index, font.bold() ? mBackground : QVariant(), Qt::BackgroundRole );
    mQtModulesModel->setData( index, font.bold() ? mForeground : QVariant(), Qt::ForegroundRole );
    mQtModulesModel->setData( index, QVariant::fromValue( item ), pGenericTableModel::ExtendedUserRole );
}

void UISettingsQMake::getQtModule( const QModelIndex& index )
{
    const QtItem item = mQtModulesModel->data( index, pGenericTableModel::ExtendedUserRole ).value<QtItem>();
    
    ui->leCaptionQtModule->setText( item.Text );
    ui->leValueQtModule->setText( item.Value );
    ui->leVariableQtModule->setText( item.Variable );
    ui->pteHelpQtModule->setPlainText( item.Help );
    
    ui->wQtModule->setEnabled( index.isValid() );
}

void UISettingsQMake::updateQtModuleState()
{
    const QModelIndex index = ui->lvQtModules->selectionModel()->selectedIndexes().value( 0 );
    const int count = mQtModulesModel->rowCount( index.parent() );
    
    ui->tbRemoveQtModule->setEnabled( index.isValid() );
    ui->tbClearQtModules->setEnabled( count > 0 );
    ui->tbUpQtModule->setEnabled( index.isValid() && index.row() > 0 && count > 1 );
    ui->tbDownQtModule->setEnabled( index.isValid() && index.row() < count -1 && count > 1 );
}

void UISettingsQMake::lvQtModules_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
    const QModelIndex oldIndex = deselected.indexes().value( 0 );
    const QModelIndex newIndex = selected.indexes().value( 0 );
    
    setQtModule( oldIndex );
    getQtModule( newIndex );
    updateQtModuleState();
}

void UISettingsQMake::on_tbAddQtModule_clicked()
{
    const int count = mQtModulesModel->rowCount();
    mQtModulesModel->insertRow( count );
    const QModelIndex index = mQtModulesModel->index( count, 0 );
    
    if ( index.isValid() ) {
        const QtItem item( tr( "New Qt module" ) );
        mQtModulesModel->setData( index, item.Text, Qt::DisplayRole );
        mQtModulesModel->setData( index, QVariant::fromValue( item ), pGenericTableModel::ExtendedUserRole );
        ui->lvQtModules->setCurrentIndex( index );
        ui->lvQtModules->scrollTo( index, QAbstractItemView::EnsureVisible );
    }
}

void UISettingsQMake::on_tbRemoveQtModule_clicked()
{
    const QModelIndex index = ui->lvQtModules->selectionModel()->selectedIndexes().value( 0 );
    mQtModulesModel->removeRow( index.row() );
}

void UISettingsQMake::on_tbClearQtModules_clicked()
{
    mQtModulesModel->setRowCount( 0 );
    updateQtModuleState();
}

void UISettingsQMake::on_tbUpQtModule_clicked()
{
    const QModelIndex index = ui->lvQtModules->selectionModel()->selectedIndexes().value( 0 );
    mQtModulesModel->swapRows( index.row(), index.row() -1 );
    updateQtModuleState();
}

void UISettingsQMake::on_tbDownQtModule_clicked()
{
    const QModelIndex index = ui->lvQtModules->selectionModel()->selectedIndexes().value( 0 );
    mQtModulesModel->swapRows( index.row(), index.row() +1 );
    updateQtModuleState();
}

// qt configuration

void UISettingsQMake::setQtConfiguration( const QModelIndex& index )
{
    if ( !index.isValid() ) {
        return;
    }
    
    QtItem item = mQtConfigurationsModel->data( index, pGenericTableModel::ExtendedUserRole ).value<QtItem>();
    QFont font( index.data( Qt::FontRole ).value<QFont>() );
    
    item.Text = ui->leCaptionQtConfiguration->text();
    item.Value = ui->leValueQtConfiguration->text();
    item.Variable = ui->leVariableQtConfiguration->text();
    item.Help = ui->pteHelpQtConfiguration->toPlainText();
    
    font.setBold( item.Value.isEmpty() && item.Variable.isEmpty() );
    
    mQtConfigurationsModel->setData( index, item.Text, Qt::DisplayRole );
    mQtConfigurationsModel->setData( index, font.bold() ? font : QVariant(), Qt::FontRole );
    mQtConfigurationsModel->setData( index, font.bold() ? mBackground : QVariant(), Qt::BackgroundRole );
    mQtConfigurationsModel->setData( index, font.bold() ? mForeground : QVariant(), Qt::ForegroundRole );
    mQtConfigurationsModel->setData( index, QVariant::fromValue( item ), pGenericTableModel::ExtendedUserRole );
}

void UISettingsQMake::getQtConfiguration( const QModelIndex& index )
{
    const QtItem item = mQtConfigurationsModel->data( index, pGenericTableModel::ExtendedUserRole ).value<QtItem>();
    
    ui->leCaptionQtConfiguration->setText( item.Text );
    ui->leValueQtConfiguration->setText( item.Value );
    ui->leVariableQtConfiguration->setText( item.Variable );
    ui->pteHelpQtConfiguration->setPlainText( item.Help );
    
    ui->wQtConfiguration->setEnabled( index.isValid() );
}

void UISettingsQMake::updateQtConfigurationState()
{
    const QModelIndex index = ui->lvQtConfigurations->selectionModel()->selectedIndexes().value( 0 );
    const int count = mQtConfigurationsModel->rowCount( index.parent() );
    
    ui->tbRemoveQtConfiguration->setEnabled( index.isValid() );
    ui->tbClearQtConfigurations->setEnabled( count > 0 );
    ui->tbUpQtConfiguration->setEnabled( index.isValid() && index.row() > 0 && count > 1 );
    ui->tbDownQtConfiguration->setEnabled( index.isValid() && index.row() < count -1 && count > 1 );
}

void UISettingsQMake::lvQtConfigurations_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
    const QModelIndex oldIndex = deselected.indexes().value( 0 );
    const QModelIndex newIndex = selected.indexes().value( 0 );
    
    setQtConfiguration( oldIndex );
    getQtConfiguration( newIndex );
    updateQtConfigurationState();
}

void UISettingsQMake::on_tbAddQtConfiguration_clicked()
{
    const int count = mQtConfigurationsModel->rowCount();
    mQtConfigurationsModel->insertRow( count );
    const QModelIndex index = mQtConfigurationsModel->index( count, 0 );
    
    if ( index.isValid() ) {
        const QtItem item( tr( "New Qt configuration" ) );
        mQtConfigurationsModel->setData( index, item.Text, Qt::DisplayRole );
        mQtConfigurationsModel->setData( index, QVariant::fromValue( item ), pGenericTableModel::ExtendedUserRole );
        ui->lvQtConfigurations->setCurrentIndex( index );
        ui->lvQtConfigurations->scrollTo( index, QAbstractItemView::EnsureVisible );
    }
}

void UISettingsQMake::on_tbRemoveQtConfiguration_clicked()
{
    const QModelIndex index = ui->lvQtConfigurations->selectionModel()->selectedIndexes().value( 0 );
    mQtConfigurationsModel->removeRow( index.row() );
}

void UISettingsQMake::on_tbClearQtConfigurations_clicked()
{
    mQtConfigurationsModel->setRowCount( 0 );
    updateQtConfigurationState();
}

void UISettingsQMake::on_tbUpQtConfiguration_clicked()
{
    const QModelIndex index = ui->lvQtConfigurations->selectionModel()->selectedIndexes().value( 0 );
    mQtConfigurationsModel->swapRows( index.row(), index.row() -1 );
    updateQtConfigurationState();
}

void UISettingsQMake::on_tbDownQtConfiguration_clicked()
{
    const QModelIndex index = ui->lvQtConfigurations->selectionModel()->selectedIndexes().value( 0 );
    mQtConfigurationsModel->swapRows( index.row(), index.row() +1 );
    updateQtConfigurationState();
}

void UISettingsQMake::on_dbbButtons_clicked( QAbstractButton* button )
{
    Q_UNUSED( button );
    
    // valid current data
    setQtVersion( ui->lvQtVersions->selectionModel()->selectedIndexes().value( 0 ) );
    setQtModule( ui->lvQtModules->selectionModel()->selectedIndexes().value( 0 ) );
    setQtConfiguration( ui->lvQtConfigurations->selectionModel()->selectedIndexes().value( 0 ) );
    
    // save qt versions
    QtVersionList versions;
    
    for ( int i = 0; i < mQtVersionsModel->rowCount(); i++ ) {
        const QModelIndex index = mQtVersionsModel->index( i, 0 );
        const QtVersion version = index.data( pGenericTableModel::ExtendedUserRole ).value<QtVersion>();
        
        if ( !version.isValid() ) {
            ui->lvQtVersions->setCurrentIndex( index );
            QMessageBox::warning( window(), tr( "Error..." ), tr( "A Qt Version is not valid and has been selected, please correct it and save again." ) );
            ui->lwPages->setCurrentRow( 0 );
            ui->lvQtVersions->setFocus();
            return;
        }
        
        versions << version;
    }
    
    mQtManager->setVersions( versions );
    
    // save modules
    QtItemList modules;
    
    for ( int i = 0; i < mQtModulesModel->rowCount(); i++ ) {
        const QModelIndex index = mQtModulesModel->index( i, 0 );
        const QtItem item = index.data( pGenericTableModel::ExtendedUserRole ).value<QtItem>();
        modules << item;
    }
    
    mQtManager->setModules( modules );
    
    // save configurations
    QtItemList configurations;
    
    for ( int i = 0; i < mQtConfigurationsModel->rowCount(); i++ ) {
        const QModelIndex index = mQtConfigurationsModel->index( i, 0 );
        const QtItem item = index.data( pGenericTableModel::ExtendedUserRole ).value<QtItem>();
        configurations << item;
    }
    
    mQtManager->setConfigurations( configurations );
    
    // save content on disk
    mQtManager->sync();
}
