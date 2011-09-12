#include "QMakeMainEditor.h"
#include "ui_QMakeMainEditor.h"
#include "../QMake.h"
#include "../QtVersionManager.h"

#include <xupmanager/core/XUPProjectItem.h>
#include <xupmanager/core/XUPProjectItemHelper.h>

#include <QFileDialog>

QMakeMainEditor::QMakeMainEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent )
    : XUPPageEditor( parent ), mPositiveValues( positive ), mNegativeValues( negative )
{
    mProject = 0;
    
    ui = new Ui_QMakeMainEditor;
    ui->setupUi( this );
    ui->bgType->setId( ui->rbSolution, QMakeMainEditor::Solution );
    ui->bgType->setId( ui->rbApplication, QMakeMainEditor::Application );
    ui->bgType->setId( ui->rbStaticLibrary, QMakeMainEditor::StaticLibrary );
    ui->bgType->setId( ui->rbSharedLibrary, QMakeMainEditor::SharedLibrary );
    ui->bgType->setId( ui->rbQtPlugin, QMakeMainEditor::QtPlugin );
    ui->bgType->setId( ui->rbQtDesignerPlugin, QMakeMainEditor::QtDesignerPlugin );
    
    connect( ui->bgType, SIGNAL( buttonClicked( int ) ), this, SLOT( projectTypeChanged() ) );
}

QMakeMainEditor::~QMakeMainEditor()
{
    delete ui;
}

void QMakeMainEditor::setup( XUPProjectItem* project )
{
    mProject = project;
    
    const QString projectTemplate = mPositiveValues.value( "TEMPLATE" ).join( " " );
    const QStringList config = mPositiveValues.value( "CONFIG" );
    const QtVersionManager* qmanager = QMake::versionManager();
    const QtVersion qversion = qmanager->version( XUPProjectItemHelper::projectSettingsValue( mProject, "QT_VERSION" ) );
    
    if ( projectTemplate == "app" || projectTemplate == "vcapp" ) {
        ui->bgType->button( QMakeMainEditor::Application )->setChecked( true );
    }
    else if ( projectTemplate == "subdirs" || projectTemplate == "vcsubdirs" ) {
        ui->bgType->button( QMakeMainEditor::Solution )->setChecked( true );
    }
    else if ( projectTemplate == "lib" || projectTemplate == "vclib" ) {
        if ( config.contains( "designer" ) ) {
            ui->bgType->button( QMakeMainEditor::QtDesignerPlugin )->setChecked( true );
        }
        else if ( config.contains( "plugin" ) ) {
            ui->bgType->button( QMakeMainEditor::QtPlugin )->setChecked( true );
        }
        else if ( config.contains( "shared" ) || config.contains( "dll" ) ) {
            ui->bgType->button( QMakeMainEditor::SharedLibrary )->setChecked( true );
        }
        else if ( config.contains( "static" ) || config.contains( "staticlib" ) ) {
            ui->bgType->button( QMakeMainEditor::StaticLibrary )->setChecked( true );
        }
    }
    
    //if ( !isSolution() ) {
        ui->leProjectName->setText( mPositiveValues.value( "TARGET" ).join( " " ) );
        
        switch ( ui->bgType->checkedId() ) {
            case QMakeMainEditor::Application:
            case QMakeMainEditor::StaticLibrary:
                ui->leProjectTarget->setText( mPositiveValues.value( "DESTDIR" ).join( " " ) );
                break;
            case QMakeMainEditor::SharedLibrary:
                ui->leProjectTarget->setText( mPositiveValues.value( "DLLDESTDIR" ).join( " " ) );
                break;
            default:
                break;
        }
    //}
    
    foreach ( const QtVersion& qv, qmanager->versions() ) {
        QListWidgetItem* it = new QListWidgetItem( qv.Version, ui->lwQtVersion );
        it->setData( Qt::UserRole, QVariant::fromValue( qv ) );
        
        if ( qv.Default ) {
            it->setBackground( QColor( 249, 228, 227 ) );
        }
        
        if ( qv == qversion ) {
            it->setSelected( true );
        }
    }
    
    projectTypeChanged();
    
    if ( project != project->topLevelProject() ) {
        ui->gbQtVersion->setDisabled( true );
    }
}

void QMakeMainEditor::finalize()
{
    QListWidgetItem* qversionItem = ui->lwQtVersion->selectedItems().value( 0 );
    
    switch ( ui->bgType->checkedId() ) {
        case QMakeMainEditor::Solution:
            mPositiveValues[ "TEMPLATE" ] << "subdirs";
            break;
        case QMakeMainEditor::Application:
            mPositiveValues[ "TEMPLATE" ] << "app";
            break;
        case QMakeMainEditor::StaticLibrary:
            mPositiveValues[ "TEMPLATE" ] << "lib";
            mPositiveValues[ "CONFIG" ] << "static";
            break;
        case QMakeMainEditor::SharedLibrary:
            mPositiveValues[ "TEMPLATE" ] << "lib";
            mPositiveValues[ "CONFIG" ] << "shared";
            break;
        case QMakeMainEditor::QtPlugin:
            mPositiveValues[ "TEMPLATE" ] << "lib";
            mPositiveValues[ "CONFIG" ] << "plugin";
            break;
        case QMakeMainEditor::QtDesignerPlugin:
            mPositiveValues[ "TEMPLATE" ] << "lib";
            mPositiveValues[ "CONFIG" ] << "plugin" << "designer";
            break;
    }
    
    //if ( !isSolution() ) {
        if ( !ui->leProjectName->text().isEmpty() ) {
            mPositiveValues[ "TARGET" ] << ui->leProjectName->text();
        }
        
        switch ( ui->bgType->checkedId() ) {
            case QMakeMainEditor::Application:
            case QMakeMainEditor::StaticLibrary:
                if ( !ui->leProjectTarget->text().isEmpty() ) {
                    mPositiveValues[ "DESTDIR" ] << ui->leProjectTarget->text();
                }
                break;
            case QMakeMainEditor::SharedLibrary:
                if ( !ui->leProjectTarget->text().isEmpty() ) {
                    mPositiveValues[ "DLLDESTDIR" ] << ui->leProjectTarget->text();
                }
                break;
            default:
                break;
        }
    //}
    
    if ( ui->gbQtVersion->isEnabled() && qversionItem ) {
        const QtVersion qversion = qversionItem->data( Qt::UserRole ).value<QtVersion>();
        XUPProjectItemHelper::setProjectSettingsValue( mProject, "QT_VERSION", qversion.Version );
    }
}

bool QMakeMainEditor::isSolution() const
{
    return ui->bgType->checkedId() == QMakeMainEditor::Solution;
}

void QMakeMainEditor::projectTypeChanged()
{
    bool isSolution = this->isSolution();
    ui->leProjectName->setDisabled( isSolution );
    ui->leProjectTarget->setDisabled( isSolution );
    ui->tbProjectTarget->setDisabled( isSolution );
}

void QMakeMainEditor::on_tbProjectTarget_clicked()
{
    QString path = ui->leProjectTarget->text().isEmpty() ? mProject->path() : mProject->filePath( ui->leProjectTarget->text() );
    path = QFileDialog::getExistingDirectory( this, tr( "Choose a target path for your project" ), path );
    
    if ( !path.isEmpty() ) {
        ui->leProjectTarget->setText( mProject->relativeFilePath( path ) );
    }
}
