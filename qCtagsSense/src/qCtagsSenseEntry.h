#ifndef QCTAGSSENSEENTRY_H
#define QCTAGSSENSEENTRY_H

#include "qCtagsSenseExport.h"
#include "qCtagsSenseGlobals.h"

#include <QList>
#include <QPair>
#include <QSqlRecord>
#include <QMetaType>

namespace qCtagsSense
{

struct Entry;
typedef QList<qCtagsSense::Entry> EntryList;

struct QCTAGSSENSE_EXPORT Entry
{
	Entry( Entry* _p = 0, bool _lne = false, ulong _ln = -1,  bool _fs = false, bool _fe = false, bool _tl = false, int _k = -1 );
	Entry( const QSqlRecord& record, const QString& fileName = QString::null );
	
	Entry* parent;
	
	QString fileName;
	qCtagsSense::Language language;
	bool lineNumberEntry;
	ulong lineNumber;
	bool isFileScope; // is header file
	bool isFileEntry;
	bool truncateLine;
	QString name;
	int kind;
	// extended fields
	QString access;
	QString fileScope;
	QString implementation;
	QString inheritance;
	QPair<QString, QString> scope;
	QString signature;
	QPair<QString, QString> typeRef;
	// children
	EntryList children;
};

}; // namespace qCtagsSense

Q_DECLARE_METATYPE( qCtagsSense::Entry* )

#endif // QCTAGSSENSEENTRY_H
