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
#include "qCtagsSenseIndexer.h"
#include "qCtagsSense.h"
#include "qCtagsSenseUtils.h"
#include "qCtagsSenseSQL.h"

#include <QMutexLocker>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

qCtagsSenseIndexer::qCtagsSenseIndexer( qCtagsSenseSQL* parent )
	: QThread( parent )
{
	mStop = false;
	mSQL = parent;
}

qCtagsSenseIndexer::~qCtagsSenseIndexer()
{
	mStop = true;
	wait();
}

void qCtagsSenseIndexer::clear()
{
	QMutexLocker locker( &mMutex );
	
	const bool wasStopped = mStop;
	mFilteredSuffixes.clear();
	mWaitingDeletion.clear();
	mWaitingIndexation.clear();
	mStop = true;
	wait();
	mStop = wasStopped;
}

QStringList qCtagsSenseIndexer::filteredSuffixes() const
{
	QMutexLocker locker( &const_cast<qCtagsSenseIndexer*>( this )->mMutex );
	
	return mFilteredSuffixes;
}

void qCtagsSenseIndexer::addFilteredSuffixes( const QStringList& suffixes )
{
	QMutexLocker locker( &mMutex );
	
	foreach ( const QString& suffix, suffixes )
	{
		if ( !mFilteredSuffixes.contains( suffix ) )
		{
			mFilteredSuffixes << suffix;
		}
	}
}

void qCtagsSenseIndexer::addFilteredSuffix( const QString& suffix )
{
	addFilteredSuffixes( QStringList( suffix ) );
}

void qCtagsSenseIndexer::setFilteredSuffixes( const QStringList& suffixes )
{
	QMutexLocker locker( &mMutex );
	
	mFilteredSuffixes = suffixes;
}

void qCtagsSenseIndexer::setFilteredSuffix( const QString& suffix )
{
	setFilteredSuffixes( QStringList( suffix ) );
}

void qCtagsSenseIndexer::removeFile( const QString& fileName )
{
	if ( mStop )
	{
		return;
	}
	
	QMutexLocker locker( &mMutex );

	if ( !mWaitingDeletion.contains( fileName ) )
	{
		mWaitingDeletion << fileName;
	}
	
	locker.unlock();

	if ( !isRunning() )
	{
		start();
	}
}

void qCtagsSenseIndexer::indexFile( const QString& fileName )
{
	if ( mStop )
	{
		return;
	}
	
	QMutexLocker locker( &mMutex );
	
	if ( !mWaitingIndexation.contains( fileName ) && QFile::exists( fileName ) )
	{
		mWaitingIndexation[ fileName ] = QString::null;
	}
	
	locker.unlock();

	if ( !isRunning() )
	{
		start();
	}
}

void qCtagsSenseIndexer::indexFiles( const QStringList& fileNames )
{
	if ( mStop )
	{
		return;
	}
	
	QMutexLocker locker( &mMutex );
	
	foreach ( const QString& fileName, fileNames )
	{
		if ( !mWaitingIndexation.contains( fileName ) && QFile::exists( fileName ) )
		{
			mWaitingIndexation[ fileName ] = QString::null;
		}
	}
	
	locker.unlock();

	if ( !isRunning() )
	{
		start();
	}
}

void qCtagsSenseIndexer::indexBuffers( const QMap<QString, QString>& buffers )
{
	if ( mStop )
	{
		return;
	}
	
	QMutexLocker locker( &mMutex );
	
	foreach ( const QString& fileName, buffers.keys() )
	{
		if ( !mWaitingIndexation.contains( fileName ) )
		{
			mWaitingIndexation[ fileName ] = buffers[ fileName ];
			
			// null buffers are not accepted as they conflict with true files
			if ( mWaitingIndexation[ fileName ].isNull() )
			{
				mWaitingIndexation[ fileName ] = QString( "" );
			}
		}
		
		if ( mStop )
		{
			return;
		}
	}
	
	locker.unlock();

	if ( !isRunning() )
	{
		start();
	}
}

// PROTECTED

bool qCtagsSenseIndexer::removeEntries( const QStringList& fileNames )
{
	QSqlQuery q = mSQL->query();
	QString del_sql;
	
	foreach ( QString fileName, fileNames )
	{
		QFileInfo file( fileName );
		
		if ( file.isFile() )
		{
			del_sql = QString( "DELETE FROM Files WHERE filename = '%1'" ).arg( fileName );
		}
		else
		{
			del_sql = QString( "DELETE FROM Files WHERE filename LIKE '%1/%'" ).arg( fileName );
		}
		
		if ( !q.exec( del_sql ) )
		{
			qWarning() << "Can't delete file entry for" << fileName.toLocal8Bit().constData();
			return false;
		}
		
		if ( mStop )
		{
			return false;
		}
	}
	
	return true;
}

bool qCtagsSenseIndexer::indexEntry( const QString& fileName )
{
	QMap<QString, TagEntryListItem*> entries;
	bool ok = false;
	QFileInfo file( fileName );
	
	// tag files
	if ( file.isFile() )
	{
		TagEntryListItem* item = tagFileEntry( fileName, ok );
		
		if ( ok && item )
		{
			entries[ fileName ] = item;
		}
	}
	else if ( file.isDir() )
	{
		entries = tagPathEntries( fileName, ok );
	}
	
	// index tags
	ok = ok && indexTags( entries );
	
	// free tags
	foreach ( TagEntryListItem* item, entries )
	{
		freeTagEntryListItem( item );
		
		if ( mStop )
		{
			return false;
		}
	}
	
	entries.clear();
	
	return ok;
}

bool qCtagsSenseIndexer::indexEntries( const QMap<QString, QString>& entries )
{
	bool ok = false;
	QMap<QString, TagEntryListItem*> tagEntries = tagBuffersEntries( entries, ok );

	// index tags
	ok = ok && indexTags( tagEntries );
	
	// free tags
	foreach ( TagEntryListItem* item, tagEntries )
	{
		freeTagEntryListItem( item );
		
		if ( mStop )
		{
			return false;
		}
	}
	
	tagEntries.clear();
	
	return ok;
}

int qCtagsSenseIndexer::createFileEntry( const QString& fileName, const QString& language )
{
	QSqlQuery q = mSQL->query();
	const QString set_sql = "INSERT INTO files (filename, language) VALUES( ?, ? )";
	
	q.prepare( set_sql );
	q.addBindValue( fileName );
	q.addBindValue( language );
	
	if ( !q.exec() )
	{
		qWarning() << "Can't create file id for" << fileName.toLocal8Bit().constData();
		return -1;
	}
	
	return q.lastInsertId().toInt();
}

bool qCtagsSenseIndexer::createEntries( int fileId, TagEntryListItem* item )
{
	QSqlQuery q = mSQL->query();
	const QString sql = QString(
		"INSERT INTO entries "
		"( file_id, line_number_entry, line_number, is_file_scope, is_file_entry, truncate_line, "
		"name, kind, access, file_scope, implementation, inheritance, scope_value, "
		"scope_key, signature, type, type_name ) "
		"VALUES( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )"
	);
	
	while ( item != NULL )
	{
		tagEntryInfo* entry = &item->tag;
		
		q.prepare( sql );
		q.addBindValue( fileId );
		q.addBindValue( entry->lineNumberEntry );
		q.addBindValue( QVariant::fromValue( entry->lineNumber ) );
		q.addBindValue( entry->isFileScope );
		q.addBindValue( entry->isFileEntry );
		q.addBindValue( entry->truncateLine );
		q.addBindValue( entry->name );
		q.addBindValue( qCtagsSenseUtils::kindType( QChar( entry->kind ), entry->language ) );
		q.addBindValue( entry->extensionFields.access );
		q.addBindValue( entry->extensionFields.fileScope );
		q.addBindValue( entry->extensionFields.implementation );
		q.addBindValue( entry->extensionFields.inheritance );
		q.addBindValue( entry->extensionFields.scope[ 0 ] );
		q.addBindValue( entry->extensionFields.scope[ 1 ] );
		q.addBindValue( entry->extensionFields.signature );
		q.addBindValue( entry->extensionFields.typeRef[ 0 ] );
		q.addBindValue( entry->extensionFields.typeRef[ 1 ] );
		
		if ( !q.exec() )
		{
			qWarning() << "Can't create entry for" << entry->name;
			qWarning() << q.lastError().text();
			return false;
		}
		
		item = item->next;
		
		if ( mStop )
		{
			return false;
		}
	}
	
	return true;
}

bool qCtagsSenseIndexer::indexTags( const QMap<QString, TagEntryListItem*>& tags )
{
	// remove already existing files entries
	if ( !removeEntries( tags.keys() ) )
	{
		qWarning() << "Can't remove entries for" << tags.keys();
		return false;
	}
	
	// index new ones
	foreach ( const QString& fileName, tags.keys() )
	{
		TagEntryListItem* tag = tags[ fileName ];
		
		int fileId = createFileEntry( fileName, QString::fromLocal8Bit( tag->tag.language ) );
		
		if ( fileId == -1 )
		{
			return false;
		}
		
		if ( !createEntries( fileId, tag ) )
		{
			return false;
		}
		
		if ( mStop )
		{
			return false;
		}
	}
	
	return true;
}

TagEntryListItem* qCtagsSenseIndexer::tagFileEntry( const QString& fileName, bool& ok )
{
	if ( !QFile::exists( fileName ) )
	{
		qWarning() << "File does not exists" << fileName.toLocal8Bit().constData();
		ok = false;
		return 0;
	}
	
	{
		QMutexLocker locker( &mMutex );
		
		if ( QDir::match( mFilteredSuffixes, fileName ) )
		{
			// skipping file is not an error
			ok = true;
			return 0;
		}
	}
	
	// files with no suffixes can't be parsed
	if ( QFileInfo( fileName ).suffix().isEmpty() )
	{
		ok = true;
		return 0;
	}
	
	ok = true;
	return createTagEntryListItem( fileName.toLocal8Bit().constData(), 0 );
}

QMap<QString, TagEntryListItem*> qCtagsSenseIndexer::tagPathEntries( const QString& pathName, bool& ok )
{
	QMap<QString, TagEntryListItem*> entries;
	
	if ( !QFile::exists( pathName ) )
	{
		qWarning() << "Path does not exists" << pathName.toLocal8Bit().constData();
		ok = false;
		return entries;
	}
	
	QDir dir( pathName );

	foreach ( const QFileInfo& file, qCtagsSenseUtils::getFiles( dir, QStringList( "*" ), true ) )
	{
		TagEntryListItem* item = tagFileEntry( file.absoluteFilePath(), ok );
		
		if ( mStop )
		{
			ok = false;
		}
		
		if ( !ok )
		{
			qWarning() << "Failed to index" << file.absoluteFilePath().toLocal8Bit().constData();
			qWarning() << "Failed to index" << pathName.toLocal8Bit().constData();
			return entries;
		}
		else if ( ok && item )
		{
			entries[ file.absoluteFilePath() ] = item;
		}
	}
	
	ok = true;
	return entries;
}

QMap<QString, TagEntryListItem*> qCtagsSenseIndexer::tagBuffersEntries( const QMap<QString, QString>& entries, bool& ok )
{
	QMap<QString, TagEntryListItem*> tagEntries;
	
	foreach ( const QString& fileName, entries.keys() )
	{
		QFile  file( QString( "%1/mks_ctags_%2" ).arg( QDir::tempPath() ).arg( QFileInfo( fileName ).fileName() ) );
		
		if ( !file.open( QIODevice::WriteOnly ) )
		{
			qWarning() << "Failed to create temporary file" << fileName.toLocal8Bit().constData();
			ok = false;
			return tagEntries;
		}
		
		file.resize( 0 );
		
		if ( file.write( entries[ fileName ].toUtf8() ) == -1 )
		{
			qWarning() << "Failed to write in temporary file" << fileName.toLocal8Bit().constData();
			ok = false;
			file.close();
			file.remove();
			return tagEntries;
		}
		
		file.close();
		
		TagEntryListItem* item = tagFileEntry( file.fileName(), ok );
		
		file.remove();
		
		if ( mStop )
		{
			ok = false;
		}
		
		if ( !ok )
		{
			qWarning() << "Failed to index" << fileName.toLocal8Bit().constData();
			return tagEntries;
		}
		else if ( ok && item )
		{
			tagEntries[ fileName ] = item;
		}
	}
	
	ok = true;
	return tagEntries;
}

void qCtagsSenseIndexer::run()
{
	emit indexingStarted();
	
	bool changed = false;
	bool error = false;
	int value = 0;
	int total = 0;
	
	// start transaction
	mSQL->database().transaction();
	
	forever
	{
		QMutexLocker locker( &mMutex );
		
		// copy
		QStringList fileNamesToRemove = mWaitingDeletion;
		QMap<QString, QString> fileNamesToIndex = mWaitingIndexation;
		
		// clear
		mWaitingDeletion.clear();
		mWaitingIndexation.clear();
		
		locker.unlock();
		
		// compute files
		foreach ( const QString& fileName, fileNamesToIndex.keys() )
		{
			if ( QFileInfo( fileName ).isDir() )
			{
				fileNamesToIndex.remove( fileName );
				
				QDir dir( fileName );
				
				foreach ( const QFileInfo& file, qCtagsSenseUtils::getFiles( dir, QStringList( "*" ), true ) )
				{
					fileNamesToIndex[ file.absoluteFilePath() ] = QString::null;
				}
			}
			
			if ( mStop )
			{
				return;
			}
		}
		
		// do count
		total += fileNamesToRemove.count();
		total += fileNamesToIndex.count();
		
		emit indexingProgress( value, total );
		
		// deletion
		if ( removeEntries( fileNamesToRemove ) )
		{
			changed = true;
		}
		else if ( !error )
		{
			qWarning() << "Error while removing file";
			error = true;
		}
		
		value += fileNamesToRemove.count();
		emit indexingProgress( value, total );
		
		if ( mStop )
		{
			return;
		}
		
		// indexation
		while ( !fileNamesToIndex.isEmpty() )
		{
			foreach ( const QString& fileName, fileNamesToIndex.keys() )
			{
				if ( fileNamesToIndex[ fileName ].isNull() )
				{
					if ( indexEntry( fileName ) )
					{
						changed = true;
					}
					else if ( !error )
					{
						qWarning() << "Error while indexing files (" << fileName << ")";
						error = true;
					}
					
					fileNamesToIndex.remove( fileName );
					
					value++;
					emit indexingProgress( value, total );
				}
				
				if ( mStop )
				{
					return;
				}
			}
			
			if ( !fileNamesToIndex.isEmpty() )
			{
				if ( indexEntries( fileNamesToIndex ) )
				{
					changed = true;
				}
				else if ( !error )
				{
					qWarning() << "Error while indexing buffers";
					error = true;
				}
				
				fileNamesToIndex.clear();
				
				value++;
				emit indexingProgress( value, total );
			}
			
			if ( mStop )
			{
				return;
			}
		}
		
		locker.relock();
		
		if ( mStop )
		{
			return;
		}
		else if ( !mWaitingDeletion.isEmpty() || !mWaitingIndexation.isEmpty() )
		{
			continue;
		}
		
		break;
	}
	
	if ( error )
	{
		// rollback transaction
		mSQL->database().rollback();
	}
	else
	{
		// commit transaction
		mSQL->database().commit();
	}
	
	if ( changed && !error )
	{
		// inform that db has changed
		emit indexingChanged();
	}
	
	emit indexingFinished();
}
