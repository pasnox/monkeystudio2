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
#ifndef QCTAGSSENSEMEMBERSMODEL_H
#define QCTAGSSENSEMEMBERSMODEL_H

#include "qCtagsSenseExport.h"
#include "qCtagsSense.h"

#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPixmap>

class qCtagsSenseMembersThread;

class QCTAGSSENSE_EXPORT qCtagsSenseMembersModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	qCtagsSenseMembersModel( qCtagsSenseSQL* parent );
	virtual ~qCtagsSenseMembersModel();
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;

public slots:
	void refresh( const QString& fileName );

protected:
	qCtagsSenseMembersThread* mThread;
	qCtagsSenseSQL* mSQL;
	qCtagsSenseEntry* mRootEntry;

protected slots:
	void queryFinished( qCtagsSenseEntry* rootEntry );

signals:
	void ready();
};

#endif // QCTAGSSENSEMEMBERSMODEL_H
