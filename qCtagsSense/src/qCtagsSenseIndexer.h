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
#ifndef QCTAGSSENSEINDEXER_H
#define QCTAGSSENSEINDEXER_H

#include "qCtagsSenseExport.h"

#include <QThread>
#include <QMutex>
#include <QMap>
#include <QStringList>

#include <ctags.h>

class qCtagsSenseSQL;

class QCTAGSSENSE_EXPORT qCtagsSenseIndexer : public QThread
{
	Q_OBJECT
	
public:
	qCtagsSenseIndexer( qCtagsSenseSQL* parent );
	virtual ~qCtagsSenseIndexer();
	
	void clear();
	QStringList filteredSuffixes() const;

public slots:
	void addFilteredSuffixes( const QStringList& suffixes );
	void addFilteredSuffix( const QString& suffix );
	void setFilteredSuffixes( const QStringList& suffixes );
	void setFilteredSuffix( const QString& suffix );
	
	void removeFile( const QString& fileName );
	void indexFile( const QString& fileName );
	void indexBuffers( const QMap<QString, QString>& buffers );

protected:
	qCtagsSenseSQL* mSQL;
	QStringList mFilteredSuffixes;
	QMap<QString, QString> mWaitingIndexation; // fileName, content
	QList<QString> mWaitingDeletion; // fileNames
	QMutex mMutex;
	bool mStop;
	
	bool removeEntries( const QStringList& fileNames );
	bool indexEntry( const QString& fileName );
	bool indexEntries( const QMap<QString, QString>& entries );
	int createFileEntry( const QString& fileName, const QString& language );
	bool createEntries( int fileId, TagEntryListItem* item );
	bool indexTags( const QMap<QString, TagEntryListItem*>& tags );
	TagEntryListItem* tagFileEntry( const QString& fileName, bool& ok );
	QMap<QString, TagEntryListItem*> tagPathEntries( const QString& pathName, bool& ok );
	QMap<QString, TagEntryListItem*> tagBuffersEntries( const QMap<QString, QString>& entries, bool& ok );
	
	virtual void run();

signals:
	void indexingStarted();
	void indexingProgress( int value, int max );
	void indexingFinished();
	void indexingChanged();
};

#endif // QCTAGSSENSEINDEXER_H
