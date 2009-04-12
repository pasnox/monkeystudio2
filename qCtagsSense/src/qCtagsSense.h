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
		Class, // Vera, Tcl, Ruby, Python, PHP, JavaScript, Java
		Macro, // Vera, Make
		Enumerator, // Vera
		Function, // Vim, Verilog, Vera, SQL, SML, SLang, Sh, Scheme, Python, PHP, Pascal, Lua, Lisp, JavaScript
		Enum, // Vera, Java
		LocalVariable, // Vera, SQL, Java
		Member, // Vera, Python
		Namespace, // SLang
		Prototype, // Vera, SQL
		Structure, // SML
		Typedef, // Vera
		Union,
		Variable, // Vim, Vera, SQL, PHP
		ExternVariable, // Vera
		Label, // YACC, SQL, Perl
		AutoCommandGroup, // Vim
		UserDefinedCommand, // Vim
		Map, // Vim
		Constant, // Verilog, PHP, Perl
		Event, // Verilog, SQL
		Module, // Verilog, Ruby
		NetDataType, // Verilog
		Port, // Verilog
		RegisterDataType, // Verilog
		Task, // Verilog, Vera
		Program, // Vera
		Method, // Tcl, Ruby, JavaScript, Java
		Procedure, // Tcl, SQL, Pascal
		Cursor, // SQL
		RecordField, // SQL
		Package, // SQL, Perl, Java
		Record, // SQL
		SubType, // SQL
		Table, // SQL
		Trigger, // SQL
		Index, // SQL
		Publication, // SQL
		Service, // SQL
		Domain, // SQL
		View, // SQL
		Synonym, // SQL
		MobiLinkTableScript, // SQL
		MobiLinkConnScript, // SQL
		Exception, // SML
		Functor, // SML
		Signature, // SML
		Type, // SML
		ValueBinding, // SML
		Set, // Scheme
		SingletonMethod, // Ruby
		SubRoutine, // REXX, Perl
		Interface, // PHP, Java
		JavaScriptFunction, // PHP, HTML
		Format, // Perl
		SubRoutineDeclaration, // Perl
		GlobalVariable, // JavaScript
		Anchor, // HTML
		Field, // Java
		EnumType, // Java
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
