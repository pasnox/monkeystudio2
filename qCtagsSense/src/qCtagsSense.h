#ifndef QCTAGSSENSE_H
#define QCTAGSSENSE_H

#include "qCtagsSenseExport.h"

#include <QObject>
#include <QMap>
#include <QPair>

class qCtagsSenseSQL;
class qCtagsSenseIndexer;

class QCTAGSSENSE_EXPORT qCtagsSense : public QObject
{
	Q_OBJECT
	
public:
	enum Kind
	{
		Unknow = -1,
		Class, // c
		Macro, // d
		Enumerator, // e
		Function, // f
		Enum, // g
		LocalVariable, // l
		Member, // m
		Namespace, // n
		Prototype, // p
		Structure, // s
		Typedef, // t
		Union, // u
		Variable, // v
		ExternVariable // x
	};
	
	qCtagsSense( QObject* parent = 0 );
	virtual ~qCtagsSense();
	
	bool isValid() const;
	qCtagsSenseSQL* sql() const;
	qCtagsSenseIndexer* indexer() const;

public slots:
	void tagEntry( const QString& fileName );
	void tagEntries( const QMap<QString, QString>& entries );

protected:
	bool mInitialized;
	qCtagsSenseSQL* mSQL;
	qCtagsSenseIndexer* mIndexer;

signals:
	void indexingStarted();
	void indexingProgress( int value, int max );
	void indexingFinished();
	void indexingChanged();
};

struct qCtagsSenseEntry;
typedef QList<qCtagsSenseEntry*> qCtagsSenseEntryList;

struct QCTAGSSENSE_EXPORT qCtagsSenseEntry
{
	qCtagsSenseEntry( qCtagsSenseEntry* pItem = 0, bool numberEntry = false, ulong number = -1,  bool scope = false, bool entry = false, bool truncate = false, qCtagsSense::Kind _kind = qCtagsSense::Unknow )
	{
		parent = pItem;
		lineNumberEntry = numberEntry;
		lineNumber = number;
		isFileScope = scope;
		isFileEntry = entry;
		truncateLine = truncate;
		kind = _kind;
	}
	
	~qCtagsSenseEntry()
	{
		qDeleteAll( children );
	}
	
	qCtagsSenseEntry* parent;
	
	QString fileName;
	QString language;
	bool lineNumberEntry;
	ulong lineNumber;
	bool isFileScope; // is header file
	bool isFileEntry;
	bool truncateLine;
	QString name;
	qCtagsSense::Kind kind;
	// extended fields
	QString access;
	QString fileScope;
	QString implementation;
	QString inheritance;
	QPair<QString, QString> scope;
	QString signature;
	QPair<QString, QString> typeRef;
	// children
	qCtagsSenseEntryList children;
};

#endif // QCTAGSSENSE_H
