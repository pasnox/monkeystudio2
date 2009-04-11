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
