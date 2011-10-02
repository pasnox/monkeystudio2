#include "QMakeTranslationsEditor.h"
#include "ui_QMakeTranslationsEditor.h"

#include <xupmanager/core/XUPProjectItem.h>
#include <xupmanager/core/XUPProjectItemHelper.h>
#include <pLocaleModel.h>

#include <QFileDialog>
#include <QDebug>

#define QMakeTranslationsEditorDefaultName "$${TARGET}"
#define QMakeTranslationsEditorDefaultDirectory "translations"

QMakeTranslationsEditor::QMakeTranslationsEditor( QWidget* parent )
	: XUPPageEditor( parent )
{
    mProject = 0;
    mModel = new pLocaleModel( this );
    mModel->setCheckable( true );
    
	ui = new Ui_QMakeTranslationsEditor;
	ui->setupUi( this );
    ui->tvLocales->setModel( mModel );
}

QMakeTranslationsEditor::~QMakeTranslationsEditor()
{
	delete ui;
}

void QMakeTranslationsEditor::setup( XUPProjectItem* project )
{
    mProject = project;
    const QString baseName = XUPProjectItemHelper::projectSettingsValue( mProject, "TRANSLATIONS_BASENAME", QMakeTranslationsEditorDefaultName );
    const QString directory = XUPProjectItemHelper::projectSettingsValue( mProject, "TRANSLATIONS_DIRECTORY", QMakeTranslationsEditorDefaultDirectory );
    const XUPItemList translations = mProject->getVariables( mProject, "TRANSLATIONS" );
    
    ui->leBaseName->setText( baseName );
    ui->leDirectory->setText( mProject->relativeFilePath( directory ) );
    
    foreach ( const XUPItem* translation, translations ) {
        foreach( XUPItem* item, translation->childrenList() ) {
            if ( item->type() == XUPItem::File ) {
                mCurrentTranslations[ mProject->filePath( item->content() ) ] = item;
            }
        }
    }
    
    QStringList locales;
    
    foreach ( const QString& filePath, mCurrentTranslations.keys() ) {
        const QFileInfo fi( filePath );
        const QString baseName = fi.baseName();
        const int count = baseName.count( "_" );
        
        for ( int i = 0; i < count; i++ ) {
            const QLocale locale( baseName.section( '_', i +1 ) );
            
            if ( locale.language() != QLocale::C ) {
                locales << locale.name();
                break; // for
            }
        }
    }
    
    mModel->setCheckedLocales( locales );
}

void QMakeTranslationsEditor::finalize()
{
    XUPProjectItemHelper::setProjectSettingsValue( mProject, "TRANSLATIONS_BASENAME", ui->leBaseName->text() );
    XUPProjectItemHelper::setProjectSettingsValue( mProject, "TRANSLATIONS_DIRECTORY", mProject->relativeFilePath( ui->leDirectory->text() ) );
    
    const QStringList locales = mModel->checkedLocales();
    QStringList translations;
    
    foreach ( const QString& locale, locales ) {
        translations << QString( "%1/%2_%3.ts" ).arg( ui->leDirectory->text() ).arg( ui->leBaseName->text() ).arg( locale );
    }
    
    foreach ( const QString& filePath, translations ) {
        mCurrentTranslations.remove( filePath );
    }
    
    mProject->addFiles( translations );
    
    qWarning() << "Adding" << translations;
    
    /*foreach ( XUPItem* item, mCurrentTranslations.values() ) {
        mProject->removeValue( item, false );
    }*/
}

void QMakeTranslationsEditor::on_tbDirectory_clicked()
{
    const QString defaultDirectory = QString( "%1/%2" ).arg( mProject->path() ).arg( QMakeTranslationsEditorDefaultDirectory );
    QString path = ui->leDirectory->text().isEmpty() ? defaultDirectory : mProject->filePath( ui->leDirectory->text() );
    path = QFileDialog::getExistingDirectory( this, tr( "Choose a target path for your translations" ), path );
    
    if ( !path.isEmpty() ) {
        ui->leDirectory->setText( mProject->relativeFilePath( path ) );
    }
}
