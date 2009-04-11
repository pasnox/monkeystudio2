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
	
	bool removeEntry( const QString& fileName );
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
