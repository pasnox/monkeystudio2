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
#ifndef POPENEDFILEMODEL_H
#define POPENEDFILEMODEL_H

#include <MonkeyExport.h>

#include <QAbstractItemModel>
#include <QIcon>

class pWorkspace;
class pAbstractChild;

class Q_MONKEY_EXPORT pOpenedFileModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    enum SortMode
    {
        OpeningOrder,
        FileName,
        URL,
        Suffixes,
        Custom
    };
    
    pOpenedFileModel( pWorkspace* workspace );
    virtual ~pOpenedFileModel();
    
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& index ) const;
    virtual QStringList mimeTypes() const;
    virtual QMimeData* mimeData( const QModelIndexList& indexes ) const;
    virtual bool dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent );
    virtual Qt::DropActions supportedDropActions() const;
    
    pAbstractChild* document( const QModelIndex& index ) const;
    QModelIndex index( pAbstractChild* document ) const;
    
    pOpenedFileModel::SortMode sortMode() const;
    void setSortMode( pOpenedFileModel::SortMode mode );
    
    void setDocumentIcon( pAbstractChild* document, const QIcon& icon );
    void setDocumentToolTip( pAbstractChild* document, const QString& toolTip );

protected:
    pWorkspace* mWorkspace;
    pOpenedFileModel::SortMode mSortMode;
    QTimer* mSortDocumentsTimer;
    int mSortDocumentsTimeout;
    QList<pAbstractChild*> mDocuments;
    QMap<pAbstractChild*, QIcon> mDocumentsIcons;
    QMap<pAbstractChild*, QString> mDocumentsToolTips;
    QIcon mTransparentIcon;
    QIcon mModifiedIcon;
    
    void rebuildMapping( const QList<pAbstractChild*>& oldList, const QList<pAbstractChild*>& newList );
    void sortDocuments();
    void insertDocument( pAbstractChild* document, int index );

protected slots:
    void sortDocuments_timeout();
    void documentOpened( pAbstractChild* document );
    void documentModifiedChanged( pAbstractChild* document, bool modified );
    void documentClosed( pAbstractChild* document );

signals:
    void sortModeChanged( pOpenedFileModel::SortMode mode );
    void documentsSorted();
};

#endif // POPENEDFILEMODEL_H
