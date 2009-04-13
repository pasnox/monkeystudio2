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
#include "qCtagsSenseFilesModel.h"
#include "qCtagsSenseSQL.h"
#include "qCtagsSenseUtils.h"

#include <QStringList>
#include <QThread>
#include <QMutex>
#include <QSqlRecord>
#include <QFileInfo>
#include <QDebug>

class QCTAGSSENSE_EXPORT qCtagsSenseFilesThread : public QThread
{
	Q_OBJECT
	
public:
	qCtagsSenseFilesThread( qCtagsSenseSQL* parent )
		: QThread( parent )
	{
		mSQL = parent;
		mStop = false;
		mRestart = false;
		mFiles = 0;
	}
	
	virtual ~qCtagsSenseFilesThread()
	{
		QMutexLocker locker( &mMutex );
		mStop = true;
		locker.unlock();
		wait();
	}

public slots:
	void executeQuery( const QString& sql, QStringList* files )
	{
		{
			QMutexLocker locker( &mMutex );
			mStop = false;
			mRestart = isRunning();
			mQuery = sql;
			mFiles = files;
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
	bool mRestart;
	QString mQuery;
	QStringList* mFiles;
	
	virtual void run()
	{
		{
			QMutexLocker locker( &mMutex );
			mRestart = false;
			mStop = false;
		}
		
		forever
		{
			delete mFiles;
			mFiles = 0;
			
			QSqlQuery q = mSQL->query();
			q.setForwardOnly( true );
			
			if ( !q.exec( mQuery ) )
			{
				qWarning() << "Can't get filenames";
				return;
			}
			
			QStringList files;
			
			while ( q.next() )
			{
				const QString fileName = q.record().value( 0 ).toString();
				files << fileName;
				
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
					continue;
				}
			}
			
			qSort( files.begin(), files.end(), qCtagsSenseUtils::caseInsensitiveFilePathLessThan );
			
			emit queryFinished( files );
			break;
		}
	}

signals:
	void queryFinished( const QStringList& files );
};

qCtagsSenseFilesModel::qCtagsSenseFilesModel( qCtagsSenseSQL* parent )
	: QAbstractItemModel( parent )
{
	mSQL = parent;
	mFiles = 0;
	mThread = new qCtagsSenseFilesThread( mSQL );
	
	connect( mThread, SIGNAL( queryFinished( const QStringList& ) ), this, SLOT( queryFinished( const QStringList& ) ) );
}

qCtagsSenseFilesModel::~qCtagsSenseFilesModel()
{
	delete mFiles;
}

int qCtagsSenseFilesModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 1;
}

QVariant qCtagsSenseFilesModel::data( const QModelIndex& index, int role ) const
{
	if ( index.isValid() )
	{
		if ( role == Qt::DisplayRole )
		{
			return QFileInfo( mFiles->at( index.internalId() ) ).fileName();
		}
		else if ( role == Qt::ToolTipRole )
		{
			return mFiles->at( index.internalId() );
		}
	}
	
	return QVariant();
}

QModelIndex qCtagsSenseFilesModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( !parent.isValid() )
	{
		if ( row >= 0 && row < rowCount( parent ) && column >= 0 &&  column < columnCount( parent ) )
		{
			return createIndex( row, column, row );
		}
	}
	
	return QModelIndex();
}

QModelIndex qCtagsSenseFilesModel::parent( const QModelIndex& index ) const
{
	Q_UNUSED( index );
	return QModelIndex();
}

int qCtagsSenseFilesModel::rowCount( const QModelIndex& parent ) const
{
	return parent.isValid() || !mFiles ? 0 : mFiles->count();
}

bool qCtagsSenseFilesModel::hasChildren( const QModelIndex& parent ) const
{
	return parent.isValid() || !mFiles ? false : !mFiles->isEmpty();
}
	
QString qCtagsSenseFilesModel::fileName( int id ) const
{
	return mFiles ? mFiles->value( id ) : QString::null;
}

int qCtagsSenseFilesModel::indexOf( const QString& fileName ) const
{
	return mFiles ? mFiles->indexOf( fileName ) : -1;
}

bool qCtagsSenseFilesModel::isRunning() const
{
	return mThread->isRunning();
}

void qCtagsSenseFilesModel::refresh( const QString& language  )
{
	const QString sql = QString(
		"SELECT filename FROM files WHERE LOWER( language ) = LOWER( '%1' )"
	).arg( language );
	
	QStringList* files = mFiles;
	mFiles = 0;
	
	reset();
	mThread->executeQuery( sql, files );
}

void qCtagsSenseFilesModel::queryFinished( const QStringList& files )
{
	mFiles = new QStringList( files );
	
	reset();
	emit ready();
}

#include "qCtagsSenseFilesModel.moc"
