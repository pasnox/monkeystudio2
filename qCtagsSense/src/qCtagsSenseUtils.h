#ifndef QCTAGSSENSEUTILS_H
#define QCTAGSSENSEUTILS_H

#include <QMap>
#include <QDir>
#include <QPixmap>
#include <QSqlRecord>

#include "qCtagsSense.h"

namespace qCtagsSenseUtils
{
	void initMaps();
	qCtagsSense::Kind kindType( const QChar& c );
	QChar kindChar( qCtagsSense::Kind kind );
	QString kindText( qCtagsSense::Kind kind );
	QString kindText( const QChar& c );
	
	bool caseInsensitiveFilePathLessThan( const QString& s1, const QString& s2 );
	
	qCtagsSenseEntry* entryForRecord( const QSqlRecord& rec, const QString& fileName );
	QString entryDecorationFilePath( qCtagsSenseEntry* entry );
	QPixmap entryDecoration( qCtagsSenseEntry* entry );
	QString entryDisplay( qCtagsSenseEntry* entry );
	QString entryToolTip( qCtagsSenseEntry* entry );
	
	QFileInfoList getFiles( QDir fromDir, const QStringList& filters, bool recursive = true );
};

#endif // QCTAGSSENSEUTILS_H
