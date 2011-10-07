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
#ifndef QCTAGSSENSEFILESMODEL_H
#define QCTAGSSENSEFILESMODEL_H

#include "qCtagsSenseExport.h"

#include <QAbstractItemModel>

class qCtagsSenseSQL;
class qCtagsSenseFilesThread;
class QStringList;

class QCTAGSSENSE_EXPORT qCtagsSenseFilesModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    qCtagsSenseFilesModel( qCtagsSenseSQL* parent );
    virtual ~qCtagsSenseFilesModel();
    
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& index ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
    
    QString fileName( int id ) const;
    int indexOf( const QString& fileName ) const;
    bool isRunning() const;

public slots:
    void refresh( const QString& language );

protected:
    qCtagsSenseFilesThread* mThread;
    qCtagsSenseSQL* mSQL;
    QStringList* mFiles;

protected slots:
    void queryFinished( const QStringList& files );

signals:
    void ready();
};

#endif // QCTAGSSENSEFILESMODEL_H
