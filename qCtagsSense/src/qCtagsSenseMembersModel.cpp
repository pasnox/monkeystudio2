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
#include "qCtagsSenseMembersModel.h"
#include "qCtagsSenseSQL.h"
#include "qCtagsSense.h"
#include "qCtagsSenseUtils.h"

#include <QThread>
#include <QMutex>
#include <QSqlRecord>
#include <QDebug>

class QCTAGSSENSE_EXPORT qCtagsSenseMembersThread : public QThread
{
	Q_OBJECT
	
public:
	qCtagsSenseMembersThread( qCtagsSenseSQL* parent )
		: QThread( parent )
	{
		mSQL = parent;
		mStop = false;
		mRestart = false;
	}
	
	virtual ~qCtagsSenseMembersThread()
	{
		QMutexLocker locker( &mMutex );
		mStop = true;
		locker.unlock();
		wait();
	}

public slots:
	void executeQuery( const QString& sql, const QString& fileName, qCtagsSenseEntry* rootEntry )
	{
		{
			QMutexLocker locker( &mMutex );
			mRestart = isRunning();
			mStop = false;
			mQuery = sql;
			mRootEntry = rootEntry;
			mFileName = fileName;
		}
		
		if ( !isRunning() )
		{
			start();
		}
	}

protected:
	QMutex mMutex;
	qCtagsSenseSQL* mSQL;
	bool mStop;
	QString mQuery;
	QString mFileName;
	qCtagsSenseEntry* mRootEntry;
	bool mRestart;
	
	virtual void run()
	{
		{
			QMutexLocker locker( &mMutex );
			mRestart = false;
		}
		
		forever
		{
			delete mRootEntry;
			mRootEntry = 0;
			
			QSqlQuery q = mSQL->query();
			q.setForwardOnly( true );
			
			if ( !q.exec( mQuery ) )
			{
				qWarning() << "Can't get entries for population";
				return;
			}
			
			mRootEntry = new qCtagsSenseEntry;
			QMap<QString, qCtagsSenseEntry*> scopeItems;
			scopeItems[ QString::null ] = mRootEntry;
			QList<qCtagsSenseEntry*> entries;
			
			mRootEntry->name = mFileName;
			
			while ( q.next() )
			{
				qCtagsSenseEntry* entry = qCtagsSenseUtils::entryForRecord( q.record(), mFileName );
				entries << entry;
				
				switch ( entry->kind )
				{
					case qCtagsSense::Class:
					case qCtagsSense::Enum:
					case qCtagsSense::Namespace:
					case qCtagsSense::Structure:
					case qCtagsSense::Union:
					{
						QString scope = entry->scope.second;
						
						if ( !scope.isEmpty() )
						{
							scope += "::";
						}
						
						scope += entry->name;
						scopeItems[ scope ] = entry;
						
						break;
					}
					default:
						break;
				}
				
				{
					QMutexLocker locker( &mMutex );
					
					if ( mStop )
					{
						return;
					}
					else if ( mRestart )
					{
						break;
					}
				}
			}
			
			{
				QMutexLocker locker( &mMutex );
				
				if ( mRestart )
				{
					mRestart = false;
					locker.unlock();
					qDeleteAll( entries );
					continue;
				}
			}
			
			foreach ( qCtagsSenseEntry* entry, entries )
			{
				entries.removeAll( entry );
				
				qCtagsSenseEntry* parentEntry = scopeItems.value( entry->scope.second );
				
				if ( !parentEntry )
				{
					parentEntry = scopeItems.value( QString::null );
				}
				
				entry->parent = parentEntry;
				parentEntry->children << entry;
				
				{
					QMutexLocker locker( &mMutex );
					
					if ( mStop )
					{
						return;
					}
					else if ( mRestart )
					{
						break;
					}
				}
			}
			
			{
				QMutexLocker locker( &mMutex );
				
				if ( mRestart )
				{
					mRestart = false;
					locker.unlock();
					qDeleteAll( entries );
					continue;
				}
			}
			
			emit queryFinished( mRootEntry );
			break;
		}
	}

signals:
	void queryFinished( qCtagsSenseEntry* rootEntry );
};

qCtagsSenseMembersModel::qCtagsSenseMembersModel( qCtagsSenseSQL* parent )
	: QAbstractItemModel( parent )
{
	mSQL = parent;
	mRootEntry = 0;
	mThread = new qCtagsSenseMembersThread( mSQL );
	
	connect( mThread, SIGNAL( queryFinished( qCtagsSenseEntry* ) ), this, SLOT( queryFinished( qCtagsSenseEntry* ) ) );
}

qCtagsSenseMembersModel::~qCtagsSenseMembersModel()
{
	delete mRootEntry;
}

int qCtagsSenseMembersModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 1;
}

QVariant qCtagsSenseMembersModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( mRootEntry && section == 0 && orientation == Qt::Horizontal )
	{
		if ( role == Qt::DisplayRole )
		{
			return QFileInfo( mRootEntry->name ).fileName();
		}
		else if ( role == Qt::ToolTipRole )
		{
			return mRootEntry->name;
		}
	}
	
	return QVariant();
}

QVariant qCtagsSenseMembersModel::data( const QModelIndex& index, int role ) const
{
	const QModelIndex parentIndex = index.parent();
	
	if ( !index.isValid() || index.row() > rowCount( parentIndex ) || index.column() > columnCount( parentIndex ) )
	{
		return QVariant();
	}
	
	qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
	
	switch ( role )
	{
		case Qt::DecorationRole:
			return qCtagsSenseUtils::entryDecoration( entry );
			break;
		case Qt::DisplayRole:
			return qCtagsSenseUtils::entryDisplay( entry );
			break;
		case Qt::ToolTipRole:
			return qCtagsSenseUtils::entryToolTip( entry );
			break;
		default:
			break;
	}
	
	return QVariant();
}

QModelIndex qCtagsSenseMembersModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( row >= 0 && row < rowCount( parent ) && column >= 0 && column < columnCount( parent ) )
	{
		if ( parent.isValid() )
		{
			qCtagsSenseEntry* parentEntry = static_cast<qCtagsSenseEntry*>( parent.internalPointer() );
			
			return createIndex( row, column, parentEntry->children.value( row ) );
		}
		else
		{
			return createIndex( row, column, mRootEntry->children.value( row ) );
		}
	}
	
	return QModelIndex();
}

QModelIndex qCtagsSenseMembersModel::parent( const QModelIndex& index ) const
{
	if ( index.isValid() )
	{
		qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
		qCtagsSenseEntry* parentEntry = entry->parent;
		
		if ( parentEntry == mRootEntry )
		{
			return QModelIndex();
		}
		
		const int row = parentEntry->parent->children.indexOf( parentEntry );
		
		return createIndex( row, index.column(), parentEntry );
	}
	
	return QModelIndex();
}

int qCtagsSenseMembersModel::rowCount( const QModelIndex& parent ) const
{
	if ( parent.isValid() )
	{
		qCtagsSenseEntry* parentEntry = static_cast<qCtagsSenseEntry*>( parent.internalPointer() );
		
		return parentEntry->children.count();
	}
	
	return mRootEntry ? mRootEntry->children.count() : 0;
}

bool qCtagsSenseMembersModel::hasChildren( const QModelIndex& parent ) const
{
	if ( parent.isValid() )
	{
		qCtagsSenseEntry* parentEntry = static_cast<qCtagsSenseEntry*>( parent.internalPointer() );
		
		return !parentEntry->children.isEmpty();
	}
	
	return mRootEntry ? !mRootEntry->children.isEmpty() : false;
}

void qCtagsSenseMembersModel::refresh( const QString& fileName )
{
	const QString sql = QString(
		"SELECT entries.*, language FROM entries "
		"INNER JOIN files ON files.id = entries.file_id "
		"AND files.filename = '%1'"
	).arg( fileName );
	
	qCtagsSenseEntry* root = mRootEntry;
	mRootEntry = 0;
	
	reset();
	mThread->executeQuery( sql, fileName, root );
}

void qCtagsSenseMembersModel::queryFinished( qCtagsSenseEntry* rootEntry )
{
	mRootEntry = rootEntry;
	
	reset();
	emit ready();
}

#include "qCtagsSenseMembersModel.moc"
