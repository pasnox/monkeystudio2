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
#include "qCtagsSenseKindFinder.h"
#include "qCtagsSenseSQL.h"
#include "qCtagsSenseUtils.h"

#include <QVariant>
#include <QDebug>

qCtagsSenseKindFinder::qCtagsSenseKindFinder( qCtagsSenseSQL* parent )
	: QThread( parent )
{
	mSQL = parent;
	mKind = qCtagsSense::Unknow;
	
	connect( this, SIGNAL( finished() ), this, SLOT( deleteLater() ) );
}

qCtagsSenseKindFinder::~qCtagsSenseKindFinder()
{
	delete mEntry;
}

void qCtagsSenseKindFinder::goTo( qCtagsSense::Kind kind, qCtagsSenseEntry* entry )
{
	mKind = kind;
	mEntry = entry;
	start();
}

void qCtagsSenseKindFinder::run()
{
	QSqlRecord record;
	const QString sql = QString(
		"SELECT entries.*, language, filename FROM entries "
		"INNER JOIN files ON files.id = entries.file_id "
		"AND name = ? "
		//"AND scope_value = ? "
		"AND scope_key = ? "
		"AND signature = ? "
		"AND language = ? "
		"AND kind = ?"
	);
	
	QSqlQuery q = mSQL->query();
	q.prepare( sql );
	q.addBindValue( mEntry->name );
	//q.addBindValue( mEntry->scope.first );
	q.addBindValue( mEntry->scope.second );
	q.addBindValue( mEntry->signature );
	q.addBindValue( mEntry->language );
	q.addBindValue( mKind );
	
	if ( !q.exec() )
	{
		qWarning() << "Can't retreive infos for" << mEntry->name << " for kind " << mKind;
		return;
	}
	
	if ( !q.next() )
	{
		// try to find matching member with not same signature (it can differ from header to source in C/C++)
		const QString sql_less = QString(
			"SELECT entries.*, language, filename FROM entries "
			"INNER JOIN files ON files.id = entries.file_id "
			"AND name = ? "
			//"AND scope_value = ? "
			"AND ( scope_key = ? OR scope_key LIKE ? ) "
			//"AND signature = ? "
			"AND language = ? "
			"AND kind = ?"
		);
		
		QString scope = mEntry->scope.second;
	
		if ( !scope.isEmpty() )
		{
			// may need to use different char for other language
			const QStringList parts = scope.split( "::" );
			
			if ( !parts.isEmpty() )
			{
				scope = parts.last();
			}
		}
		
		q.prepare( sql_less );
		q.addBindValue( mEntry->name );
		//q.addBindValue( mEntry->scope.first );
		q.addBindValue( mEntry->scope.second );
		q.addBindValue( scope.prepend( "%" ) );
		//q.addBindValue( mEntry->signature );
		q.addBindValue( mEntry->language );
		q.addBindValue( mKind );
		
		if ( !q.exec() )
		{
			qWarning() << "Can't retreive infos for" << mEntry->name << " for kind " << mKind;
			return;
		}
		
		if ( q.next() )
		{
			record = q.record();
		}
	}
	else
	{
		record = q.record();
	}
	
	if ( record.isEmpty() )
	{
		qWarning() << "No entry found" << mEntry->name << " for kind " << mKind;
		mEntry = 0;
	}
	else
	{
		mEntry = qCtagsSenseUtils::entryForRecord( record, q.record().value( "filename" ).toString() );
		
		emit entryActivated( mEntry );
	}
}
