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
#ifndef SEARCHRESULTSMODEL_H
#define SEARCHRESULTSMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QPoint>
#include <QStringList>
#include <QDir>

class SearchThread;

class SearchResultsModel : public QAbstractItemModel
{
    Q_OBJECT
    friend class SearchWidget;
    
public:
    struct Result
    {
        Result( const QString& _fileName = QString::null, const QString& _capture = QString::null,
            const QPoint& _position = QPoint(), int _offset = -1, int _length = 0, bool _checkable = false,
            Qt::CheckState _checkState = Qt::Unchecked, bool _enabled = true, const QStringList& _capturedTexts = QStringList() )
        {
            fileName = _fileName;
            capture = _capture;
            position = _position;
            offset = _offset;
            length = _length;
            checkable = _checkable;
            checkState = _checkState;
            enabled = _enabled;
            capturedTexts = _capturedTexts;
        }
        
        bool operator==( const SearchResultsModel::Result& other ) const
        {
            return fileName == other.fileName &&
                //capture == other.capture &&
                position == other.position &&
                offset == other.offset &&
                length == other.length &&
                /*checkable == other.checkable &&
                checkState == other.checkState &&
                enabled == other.enabled*/
                capturedTexts == other.capturedTexts;
        }
        
        QString fileName;
        QString capture;
        QPoint position;
        int offset;
        int length;
        bool checkable;
        Qt::CheckState checkState;
        bool enabled;
        QStringList capturedTexts;
    };
    
    typedef QList<SearchResultsModel::Result*> ResultList;
    
    enum CustomRoles
    {
        EnabledRole = Qt::UserRole
    };
    
    SearchResultsModel( SearchThread* searchThread, QObject* parent = 0 );
    
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& index ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    
    QModelIndex index( SearchResultsModel::Result* result ) const;
    SearchResultsModel::Result* result( const QModelIndex& index ) const;
    
    const QList<SearchResultsModel::ResultList>& results() const;

protected:
    int mRowCount;
    QDir mSearchDir;
    mutable QHash<QString, SearchResultsModel::Result*> mParents; // fileName, result
    mutable SearchResultsModel::ResultList mParentsList; // ordered parents
    mutable QList<SearchResultsModel::ResultList> mResults; // parents children
    SearchThread* mSearchThread;

public slots:
    void clear();

protected slots:
    void thread_reset();
    void thread_resultsAvailable( const QString& fileName, const SearchResultsModel::ResultList& results );
    void thread_resultsHandled( const QString& fileName, const SearchResultsModel::ResultList& results );

signals:
    void firstResultsAvailable();
};

#endif // SEARCHRESULTSMODEL_H
