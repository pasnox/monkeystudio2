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
#include "XUPOpenedProjectsModel.h"
#include "xupmanager/core/XUPProjectModel.h"
#include "xupmanager/core/XUPProjectItem.h"

XUPOpenedProjectsModel::XUPOpenedProjectsModel( QObject* parent )
    : QAbstractListModel( parent )
{
}

XUPOpenedProjectsModel::~XUPOpenedProjectsModel()
{
}

QVariant XUPOpenedProjectsModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() ) {
        return QVariant();
    }
    
    XUPProjectModel* model = mProjects.at( index.row() );
    XUPProjectItem* project = model->rootProject();
    
    switch ( role ) {
        case Qt::DecorationRole:
            return project->displayIcon();
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return project->displayText();
    }
    
    return QVariant();
}

int XUPOpenedProjectsModel::rowCount( const QModelIndex& parent ) const
{
    return parent == QModelIndex() ? mProjects.count() : 0;
}

void XUPOpenedProjectsModel::clear()
{
    if ( mProjects.isEmpty() ) {
        return;
    }
    
    const int count = rowCount();
    
    beginRemoveRows( QModelIndex(), 0, count -1 );
    mProjects.clear();
    endRemoveRows();
}

void XUPOpenedProjectsModel::addProject( XUPProjectModel* project )
{
    Q_ASSERT( project );
    
    if ( mProjects.contains( project ) ) {
        return;
    }
    
    const int row = rowCount();
    
    beginInsertRows( QModelIndex(), row, row );
    mProjects << project;
    connect( project, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( project_modified() ) );
    endInsertRows();
}

void XUPOpenedProjectsModel::removeProject( XUPProjectModel* project )
{
    Q_ASSERT( project );
    const int row = mProjects.indexOf( project );
    
    if ( row == -1 ) {
        return;
    }
    
    beginRemoveRows( QModelIndex(), row, row );
    mProjects.removeAt( row );
    endRemoveRows();
}

QList<XUPProjectModel*> XUPOpenedProjectsModel::projects() const
{
    return mProjects;
}

XUPProjectModel* XUPOpenedProjectsModel::project( int row ) const
{
    return mProjects.value( row, 0 );
}

int XUPOpenedProjectsModel::row( XUPProjectModel* project ) const
{
    return mProjects.indexOf( project );
}

void XUPOpenedProjectsModel::project_modified()
{
    emit dataChanged( index( 0, 0 ), index( rowCount() -1, 1 ) );
}
