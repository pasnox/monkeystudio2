#include "MainEditor.h"
#include "ui_MainEditor.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "xupmanager/core/XUPProjectItemHelper.h"
#include "xupmanager/core/XUPDynamicFolderItem.h"

#include <QFileDialog>

MainEditor::MainEditor( QWidget* parent )
    : XUPPageEditor( parent )
{
    ui = new Ui_MainEditor;
    ui->setupUi( this );
    mProject = 0;
}

MainEditor::~MainEditor()
{
    delete ui;
}

void MainEditor::setup( XUPProjectItem* project )
{
    mProject = project;
    
    const XUPDynamicFolderSettings folder = XUPProjectItemHelper::projectDynamicFolderSettings( project );

    ui->leProjectName->setText( mProject->attribute( "name" ) );
    ui->gbDynamicFolder->setChecked( folder.Active );
    ui->leDynamicFolder->setText( folder.AbsolutePath );
    ui->gbDynamicFilesPatterns->setValues( folder.FilesPatterns );
    updateMainFileComboBox( XUPProjectItemHelper::projectSettingsValue( mProject, "MAIN_FILE" ) );
}

void MainEditor::finalize()
{
    const XUPDynamicFolderSettings oldSettings = XUPProjectItemHelper::projectDynamicFolderSettings( mProject );
    XUPDynamicFolderSettings settings;
    settings.Active = ui->gbDynamicFolder->isChecked();
    settings.AbsolutePath = ui->leDynamicFolder->text();
    settings.FilesPatterns = ui->gbDynamicFilesPatterns->values();
    
    if ( oldSettings.AbsolutePath != settings.AbsolutePath || oldSettings.FilesPatterns != settings.FilesPatterns ) {
        XUPItem* itemFolder = XUPProjectItemHelper::projectDynamicFolderItem( mProject, false );
        
        if ( itemFolder ) {
            itemFolder->parent()->removeChild( itemFolder );
        }
    }
    
    mProject->setAttribute( "name", ui->leProjectName->text() );
    XUPProjectItemHelper::setProjectSettingsValue( mProject, "MAIN_FILE", ui->cbMainFile->currentText() );
    XUPProjectItemHelper::setProjectDynamicFolderSettings( mProject, settings );
    
    if ( settings.Active && !settings.AbsolutePath.isEmpty() && QFile::exists( settings.AbsolutePath ) ) {
        XUPDynamicFolderItem* dynamicFolderItem = XUPProjectItemHelper::projectDynamicFolderItem( mProject, true );
        dynamicFolderItem->setRootPath( settings.AbsolutePath, settings.FilesPatterns );
    }
}

void MainEditor::updateMainFileComboBox( const QString& selectFile )
{
    ui->cbMainFile->clear();
    
    const QStringList sources = mProject->sourceFiles();
    QMap<QString, QString> files;
    
    // quick insensitive sort
    foreach ( const QString& file, sources ) {
        const QString fileName = mProject->relativeFilePath( file );
        files[ fileName.toLower() ] = fileName;
    }
    
    ui->cbMainFile->addItems( files.values() );
    const int index = ui->cbMainFile->findText( mProject->relativeFilePath( selectFile ) );
    ui->cbMainFile->setCurrentIndex( index );
}

void MainEditor::on_tbDynamicFolder_clicked()
{
    QString path = ui->leDynamicFolder->text();
    path = QFileDialog::getExistingDirectory( this, tr( "Select the folder to monitor" ), path );
    
    if ( path.isEmpty() ) {
        return;
    }
    
    ui->leDynamicFolder->setText( path );
}
