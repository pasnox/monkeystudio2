#ifndef QCTAGSSENSEUTILS_H
#define QCTAGSSENSEUTILS_H

#include "qCtagsSenseExport.h"
#include "qCtagsSense.h"

#include <QMap>
#include <QDir>
#include <QPixmap>
#include <QSqlRecord>

namespace qCtagsSenseUtils
{
	QCTAGSSENSE_EXPORT void initMaps();
	QCTAGSSENSE_EXPORT qCtagsSense::Kind kindType( const QChar& c );
	QCTAGSSENSE_EXPORT QChar kindChar( qCtagsSense::Kind kind );
	QCTAGSSENSE_EXPORT QString kindText( qCtagsSense::Kind kind );
	QCTAGSSENSE_EXPORT QString kindText( const QChar& c );
	
	QCTAGSSENSE_EXPORT bool caseInsensitiveFilePathLessThan( const QString& s1, const QString& s2 );
	
	QCTAGSSENSE_EXPORT qCtagsSenseEntry* entryForRecord( const QSqlRecord& rec, const QString& fileName );
	QCTAGSSENSE_EXPORT QString entryDecorationFilePath( qCtagsSenseEntry* entry );
	QCTAGSSENSE_EXPORT QPixmap entryDecoration( qCtagsSenseEntry* entry );
	QCTAGSSENSE_EXPORT QString entryDisplay( qCtagsSenseEntry* entry );
	QCTAGSSENSE_EXPORT QString entryToolTip( qCtagsSenseEntry* entry );
	
	QCTAGSSENSE_EXPORT QFileInfoList getFiles( QDir fromDir, const QStringList& filters, bool recursive = true );
};

#endif // QCTAGSSENSEUTILS_H
