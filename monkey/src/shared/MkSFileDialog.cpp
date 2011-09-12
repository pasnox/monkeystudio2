#include "shared/MkSFileDialog.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "xupmanager/core/ProjectTypesIndex.h"
#include "pMonkeyStudio.h"
#include "coremanager/MonkeyCore.h"
#include "workspace/pFileManager.h"
#include "xupmanager/gui/XUPProjectManager.h"
#include "settingsmanager/Settings.h"

#include <QComboBox>
#include <QDebug>

MkSFileDialog::MkSFileDialog( QWidget* parent, const QString& caption, const QString& directory, const QString& filter, bool textCodecEnabled )
    : pFileDialog( parent, caption, directory, filter, textCodecEnabled, false )
{
}

void MkSFileDialog::setRecentNameFilter( const QString& name )
{
    if ( !name.isEmpty() ) {
        selectNameFilter( name );
    }
}

QString MkSFileDialog::recentNameFilter() const
{
    return selectedNameFilter();
}

pFileDialogResult MkSFileDialog::getOpenFileName( bool useRecents, QWidget* parent, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, QString* selectedFilter, Options options )
{
    pFileDialogResult result;
    MkSFileDialog fd( parent );
    setOpenFileNameDialog( &fd, caption, dir, filter, enabledTextCodec, false, selectedFilter, options );
    fd.setTextCodec( pMonkeyStudio::defaultCodec() );
    
    if ( useRecents ) {
        if ( !filter.isEmpty() ) {
            fd.setRecentNameFilter( MonkeyCore::settings()->value( "Recents/FileFilter" ).toString() );
        }
    }
    
    if ( fd.exec() == QDialog::Accepted ) {
        if ( useRecents ) {
            if ( !filter.isEmpty() ) {
                MonkeyCore::settings()->setValue( "Recents/FileFilter", fd.selectedNameFilter() );
            }
        }
        
        if ( selectedFilter ) {
            *selectedFilter = fd.selectedNameFilter();
        }
        
        result[ "filename" ] = fd.selectedFiles().value( 0 );
        result[ "codec" ] = fd.textCodec();
    }
    
    return result;
}

pFileDialogResult MkSFileDialog::getOpenFileNames( bool useRecents, QWidget* parent, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, QString* selectedFilter, Options options )
{
    pFileDialogResult result;
    MkSFileDialog fd( parent );
    setOpenFileNamesDialog( &fd, caption, dir, filter, enabledTextCodec, false, selectedFilter, options );
    fd.setTextCodec( pMonkeyStudio::defaultCodec() );
    
    if ( useRecents ) {
        if ( !filter.isEmpty() ) {
            fd.setRecentNameFilter( MonkeyCore::settings()->value( "Recents/FileFilter" ).toString() );
        }
    }
    
    if ( fd.exec() == QDialog::Accepted ) {
        if ( useRecents ) {
            if ( !filter.isEmpty() ) {
                MonkeyCore::settings()->setValue( "Recents/FileFilter", fd.selectedNameFilter() );
            }
        }
        
        if ( selectedFilter ) {
            *selectedFilter = fd.selectedNameFilter();
        }
        
        result[ "filenames" ] = fd.selectedFiles();
        result[ "codec" ] = fd.textCodec();
    }
    
    return result;
}

pFileDialogResult MkSFileDialog::getSaveFileName( bool useRecents, QWidget* parent, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, QString* selectedFilter, Options options )
{
    pFileDialogResult result;
    MkSFileDialog fd( parent );
    setSaveFileNameDialog( &fd, caption, dir, filter, enabledTextCodec, selectedFilter, options );
    fd.setTextCodec( pMonkeyStudio::defaultCodec() );
    
    if ( useRecents ) {
        if ( !filter.isEmpty() ) {
            fd.setRecentNameFilter( MonkeyCore::settings()->value( "Recents/FileFilter" ).toString() );
        }
    }
    
    if ( fd.exec() == QDialog::Accepted ) {
        if ( useRecents ) {
            if ( !filter.isEmpty() ) {
                MonkeyCore::settings()->setValue( "Recents/FileFilter", fd.selectedNameFilter() );
            }
        }
        
        if ( selectedFilter ) {
            *selectedFilter = fd.selectedNameFilter();
        }
        
        result[ "filename" ] = fd.selectedFiles().value( 0 );
        result[ "codec" ] = fd.textCodec();
    }
    
    return result;
}

pFileDialogResult MkSFileDialog::getExistingDirectory( bool useRecents, QWidget* parent, const QString& caption, const QString& dir, bool enabledTextCodec, Options options )
{
    pFileDialogResult result;
    MkSFileDialog fd( parent );
    setOpenFileNameDialog( &fd, caption, dir, QString::null, enabledTextCodec, false, 0, options );
    fd.setFileMode( QFileDialog::Directory );
    fd.setTextCodec( pMonkeyStudio::defaultCodec() );
    
    #warning Does we need recents handling here ?
    Q_UNUSED( useRecents );
    
    /*if ( useRecents ) {
        if ( !filter.isEmpty() ) {
            fd.setRecentNameFilter( MonkeyCore::settings()->value( "Recents/FileFilter" ).toString() );
        }
    }*/
    
    if ( fd.exec() == QDialog::Accepted ) {
        /*if ( useRecents ) {
            if ( !filter.isEmpty() ) {
                MonkeyCore::settings()->setValue( "Recents/FileFilter", fd.selectedNameFilter() );
            }
        }*/
        
        result[ "filename" ] = fd.selectedFiles().value( 0 );
        result[ "codec" ] = fd.textCodec();
    }
    
    return result;
}

pFileDialogResult MkSFileDialog::getNewEditorFile( QWidget* parent )
{
    const XUPProjectItem* curProject = MonkeyCore::projectsManager()->currentProject();
    const QString caption = tr( "New File Name..." );
    const QString filter = pMonkeyStudio::availableFilesFilter();
    const bool enabledTextCodec = true;
    const QString codec = curProject ? curProject->codec() : pMonkeyStudio::defaultCodec();
    QString path = MonkeyCore::fileManager()->currentDocumentFile();
    pFileDialogResult result;
    
    if ( path.isEmpty() && curProject ) {
        path = curProject->path();
    }
    
    MkSFileDialog fd( parent );
    setSaveFileNameDialog( &fd, caption, path, filter, enabledTextCodec, 0, 0 );
    fd.setTextCodec( codec );
    
    if ( fd.exec() == QDialog::Accepted ) {
        result[ "filename" ] = fd.selectedFiles().value( 0 );
        result[ "codec" ] = fd.textCodec();
    }
    
    return result;
}
