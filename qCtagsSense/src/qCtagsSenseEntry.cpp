#include "qCtagsSenseEntry.h"

#include <QVariant>

using namespace qCtagsSense;

Entry::Entry( Entry* _p, bool _lne, ulong _ln,  bool _fs, bool _fe, bool _tl, int _k )
{
	parent = _p;
	lineNumberEntry = _lne;
	lineNumber = _ln;
	isFileScope = _fs;
	isFileEntry = _fe;
	truncateLine = _tl;
	kind = _k;
}

Entry::Entry( const QSqlRecord& record, const QString& _fileName )
{
	fileName = _fileName.isEmpty() ? record.value( "fileName" ).toString() : _fileName;
	language = qCtagsSense::Language( record.value( "language" ).toInt() );
	lineNumberEntry = record.value( "line_number_entry" ).toBool();
	lineNumber = record.value( "line_number" ).value<ulong>();
	isFileScope = record.value( "is_file_scope" ).toBool();
	isFileEntry = record.value( "is_file_entry" ).toBool();
	truncateLine = record.value( "truncate_line" ).toBool();
	name = record.value( "name" ).toString();
	kind = record.value( "kind" ).toInt();
	// extended fields
	access = record.value( "access" ).toString();
	fileScope = record.value( "file_scope" ).toString();
	implementation = record.value( "implementation" ).toString();
	inheritance = record.value( "inheritance" ).toString();
	scope.first = record.value( "scope_value" ).toString();
	scope.second = record.value( "scope_key" ).toString();
	signature = record.value( "signature" ).toString();
	typeRef.first = record.value( "type" ).toString();
	typeRef.second = record.value( "type_name" ).toString();
}
