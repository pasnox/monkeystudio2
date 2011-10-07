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
