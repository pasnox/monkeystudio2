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
#ifndef PLOCALEMODEL_H
#define PLOCALEMODEL_H

#include "MonkeyExport.h"

#include <QAbstractItemModel>
#include <QStringList>

class Q_MONKEY_EXPORT pLocaleModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    pLocaleModel( QObject* parent = 0 );
    virtual ~pLocaleModel();
    
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& child ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    
    QModelIndex localeToIndex( const QString& locale ) const;
    QString indexToLocale( const QModelIndex& index ) const;
    
    bool isCheckable() const;
    void setCheckable( bool checkable );
    
    QStringList checkedLocales() const;
    void setCheckedLocales( const QStringList& locales, bool checked = true );
    
    static QString localeDisplayText( const QString& name );
    
protected:
    typedef QHash<int, QVariant> IntVariantMap;
    mutable QStringList mLocales; // top level locales
    mutable QHash<QString, QStringList> mChildrenLocales; // children locales of a top level locale
    QHash<QString, pLocaleModel::IntVariantMap> mData;
    bool mIsCheckable;
    
    void populate();
    void debug( const QModelIndex& root = QModelIndex(), int prof = 0 ) const;
};

#endif // PLOCALEMODEL_H
