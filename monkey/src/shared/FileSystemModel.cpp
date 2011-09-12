#include "shared/FileSystemModel.h"

#ifdef Q_OS_MAC
#include <QProcess>

QString FileSystemModel::MacOSXVolumes = "/Volumes";
#endif

FileSystemModel::FileSystemModel( QObject* parent )
    : QFileSystemModel( parent )
{
#ifdef Q_OS_MAC
    // Remove the hidden flag for the /Volumnes folder so all mount points are visible in the default (Q)FileSystemModel
    QProcess::startDetached( QString( "SetFile -a v %1" ).arg( FileSystemModel::MacOSXVolumes ) );
#endif
}

#ifdef Q_OS_MAC
QVariant FileSystemModel::data( const QModelIndex& index, int role ) const
{
    // return the 'My Computer' icon for the /Volumes folder
    if ( index.column() == 0 && filePath( index ) == FileSystemModel::MacOSXVolumes ) {
        switch ( role ) {
            case Qt::DecorationRole:
                return myComputer( role ); 
            default:
                break;
        }
    }
    
    return QFileSystemModel::data( index, role );
}
#endif
