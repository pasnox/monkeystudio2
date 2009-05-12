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
#ifndef QCTAGSSENSESEARCHMODEL_H
#define QCTAGSSENSESEARCHMODEL_H

#include "qCtagsSenseExport.h"
#include "qCtagsSense.h"

#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPixmap>

class qCtagsSenseSearchThread;

typedef QMap<QString, qCtagsSenseEntryList> SearchMapEntries;

class QCTAGSSENSE_EXPORT qCtagsSenseSearchModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	enum Types { FileName, Entry };
	enum Roles { TypeRole = Qt::UserRole, DataRole };
	
	qCtagsSenseSearchModel( qCtagsSenseSQL* parent );
	virtual ~qCtagsSenseSearchModel();
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	
	QModelIndex index( const QString& fileName ) const;
	
	static bool caseInsensitiveFileNameLessThan( const QString& s1, const QString& s2 );

public slots:
	void clear();
	void refresh( const QString& search );

protected:
	qCtagsSenseSearchThread* mThread;
	qCtagsSenseSQL* mSQL;
	SearchMapEntries* mEntries;
	QHash<QString, int> mCacheKeys;
	int mCacheCount;

protected slots:
	void queryFinished( SearchMapEntries* entries );

signals:
	void ready();
	void searching( bool searching );
};

#endif // QCTAGSSENSESEARCHMODEL_H
