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
#include "qCtagsSenseSearchModel.h"
#include "qCtagsSenseSQL.h"
#include "qCtagsSense.h"
#include "qCtagsSenseUtils.h"

#include <QThread>
#include <QMutex>
#include <QSqlRecord>
#include <QDebug>

class QCTAGSSENSE_EXPORT qCtagsSenseSearchThread : public QThread
{
	Q_OBJECT
	
public:
	qCtagsSenseSearchThread( qCtagsSenseSQL* parent )
		: QThread( parent )
	{
		mSQL = parent;
		mStop = false;
		mRestart = false;
	}
	
	virtual ~qCtagsSenseSearchThread()
	{
		QMutexLocker locker( &mMutex );
		mStop = true;
		locker.unlock();
		wait();
	}
	
	void addMapToDelete( SearchMapEntries* entries )
	{
		QMutexLocker locker( &mMutex );
		mEntriesList << entries;
	}

public slots:
	void executeQuery( const QString& sql )
	{
		{
			QMutexLocker locker( &mMutex );
			mRestart = isRunning();
			mStop = false;
			mQuery = sql;
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
	QList<SearchMapEntries*> mEntriesList;
	bool mRestart;
	
	void clearEntries()
	{
		foreach ( SearchMapEntries* entries, mEntriesList )
		{
			if ( entries )
			{
				foreach ( const QString& fileName, entries->keys() )
				{
					qDeleteAll( (*entries)[ fileName ] );
				}
			}
			
			delete entries;
		}
		
		mEntriesList.clear();
	}
	
	virtual void run()
	{
		{
			QMutexLocker locker( &mMutex );
			mRestart = false;
		}
		
		forever
		{
			emit searching( true );
			clearEntries();
			
			SearchMapEntries* mEntries = new SearchMapEntries();
			QSqlQuery q = mSQL->query();
			q.setForwardOnly( true );
			
			bool ok = q.exec( mQuery );
			
			if ( !ok )
			{
				qWarning() << "Can't get entries for population";
			}
			
			while ( q.next() )
			{
				qCtagsSenseEntry* entry = qCtagsSenseUtils::entryForRecord( q.record(), QString::null );
				(*mEntries)[ entry->fileName ] << entry;
				
				{
					QMutexLocker locker( &mMutex );
					
					if ( mStop )
					{
						break;
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
					clearEntries();
					continue;
				}
			}
			
			emit searching( false );
			emit queryFinished( mEntries );
			break;
		}
	}

signals:
	void queryFinished( SearchMapEntries* entries );
	void searching( bool searching );
};

qCtagsSenseSearchModel::qCtagsSenseSearchModel( qCtagsSenseSQL* parent )
	: QAbstractItemModel( parent )
{
	mSQL = parent;
	mEntries = 0;
	mThread = new qCtagsSenseSearchThread( mSQL );
	
	connect( mThread, SIGNAL( searching( bool ) ), this, SIGNAL( searching( bool ) ) );
	connect( mThread, SIGNAL( queryFinished( SearchMapEntries* ) ), this, SLOT( queryFinished( SearchMapEntries* ) ) );
}

qCtagsSenseSearchModel::~qCtagsSenseSearchModel()
{
	clear();
}

int qCtagsSenseSearchModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 1;
}

QVariant qCtagsSenseSearchModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( section == 0 && orientation == Qt::Horizontal )
	{
		if ( role == Qt::DisplayRole )
		{
			return tr( "Matching tags" );
		}
	}
	
	return QVariant();
}

QVariant qCtagsSenseSearchModel::data( const QModelIndex& index, int role ) const
{
	const QModelIndex parentIndex = index.parent();
	
	if ( !index.isValid() || index.row() > rowCount( parentIndex ) || index.column() > columnCount( parentIndex ) )
	{
		return QVariant();
	}
	
	if ( parentIndex.isValid() )
	{
		qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
		
		switch ( role )
		{
			case Qt::DecorationRole:
				return qCtagsSenseUtils::entryDecoration( entry );
				break;
			case Qt::DisplayRole:
				return entry->name; //qCtagsSenseUtils::entryDisplay( entry );
				break;
			case Qt::ToolTipRole:
				return qCtagsSenseUtils::entryToolTip( entry );
				break;
			case qCtagsSenseSearchModel::TypeRole:
				return qCtagsSenseSearchModel::Entry;
				break;
			case qCtagsSenseSearchModel::DataRole:
				return QVariant::fromValue( entry );
				break;
			default:
				break;
		}
	}
	else
	{
		QString fileName = mEntries->keys().at( index.row() );
		
		switch ( role )
		{
			case Qt::DecorationRole:
				break;
			case Qt::DisplayRole:
				return QFileInfo( fileName ).fileName();
				break;
			case Qt::ToolTipRole:
				return fileName;
				break;
			case qCtagsSenseSearchModel::TypeRole:
				return qCtagsSenseSearchModel::FileName;
				break;
			case qCtagsSenseSearchModel::DataRole:
				return fileName;
				break;
			default:
				break;
		}
	}
	
	return QVariant();
}

QModelIndex qCtagsSenseSearchModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( row >= 0 && row < rowCount( parent ) && column >= 0 && column < columnCount( parent ) )
	{
		if ( parent.isValid() )
		{
			QString key = mEntries->keys().at( parent.row() );
			
			return createIndex( row, column, (*mEntries)[ key ].at( row ) );
		}
		else
		{
			return createIndex( row, column, 0 );
		}
	}
	
	return QModelIndex();
}

QModelIndex qCtagsSenseSearchModel::parent( const QModelIndex& index ) const
{
	if ( index.isValid() )
	{
		if ( index.internalPointer() )
		{
			qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
			int row = mEntries->keys().indexOf( entry->fileName );
			
			return createIndex( row, index.column(), 0 );
		}
	}
	
	return QModelIndex();
}

int qCtagsSenseSearchModel::rowCount( const QModelIndex& parent ) const
{
	if ( parent.isValid() )
	{
		if ( !parent.internalPointer() )
		{
			const QString key = mEntries->keys().at( parent.row() );
			
			return (*mEntries)[ key ].count();
		}
		
		return 0;
	}
	
	return mEntries ? mEntries->count() : 0;
}

bool qCtagsSenseSearchModel::hasChildren( const QModelIndex& parent ) const
{
	if ( parent.isValid() )
	{
		if ( !parent.internalPointer() )
		{
			const QString key = mEntries->keys().at( parent.row() );
			
			return !(*mEntries)[ key ].isEmpty();
		}
		
		return false;
	}
	
	return mEntries ? !mEntries->isEmpty() : false;
}

void qCtagsSenseSearchModel::clear()
{
	mThread->addMapToDelete( mEntries );
	mEntries = 0;
	reset();
	emit ready();
}

void qCtagsSenseSearchModel::refresh( const QString& search )
{
	const QString sql = QString(
		"SELECT entries.*, language, filename FROM entries "
		"INNER JOIN files ON files.id = entries.file_id "
		"AND entries.name LIKE '%%1%'"
	).arg( search );
	
	clear();
	mThread->executeQuery( sql );
}

void qCtagsSenseSearchModel::queryFinished( SearchMapEntries* entries )
{
	mEntries = entries;
	
	reset();
	emit ready();
}

#include "qCtagsSenseSearchModel.moc"
