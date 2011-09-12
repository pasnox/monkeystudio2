/****************************************************************************
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
#ifndef QCTAGSSENSELANGUAGESMODEL_H
#define QCTAGSSENSELANGUAGESMODEL_H

#include "qCtagsSenseExport.h"

#include <QAbstractItemModel>
#include <QStringList>

class qCtagsSenseSQL;
class qCtagsSenseLanguagesThread;

class QCTAGSSENSE_EXPORT qCtagsSenseLanguagesModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    qCtagsSenseLanguagesModel( qCtagsSenseSQL* parent );
    virtual ~qCtagsSenseLanguagesModel();
    
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& index ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
    
    QString language( int id ) const;
    int indexOf( const QString& language ) const;
    bool isRunning() const;
    
public slots:
    void refresh();
    
protected:
    qCtagsSenseLanguagesThread* mThread;
    qCtagsSenseSQL* mSQL;
    QStringList* mLanguages;

protected slots:
    void queryFinished( const QStringList& languages );

signals:
    void ready();
};

#endif // QCTAGSSENSELANGUAGESMODEL_H
