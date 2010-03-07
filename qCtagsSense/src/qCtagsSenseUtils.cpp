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
#include "qCtagsSenseUtils.h"
#include "qCtagsSense.h"

#include <QCoreApplication>
#include <QVariant>

// Fileds
/*
	flags: "afmikKlnsStz") [fks]. 
*/

namespace qCtagsSenseUtils
{
	typedef QPair<QChar, qCtagsSense::Kind> CharKindPair; // kind char, kind enum
	typedef QList<CharKindPair> CharKindPairList;
	
	QMap<qCtagsSense::Kind, QString> mKindText;
	QMap<QString, CharKindPairList> mLanguageChars; // language, list of pair of char/kind
	QMap<QString, qCtagsSense::Kind> mKindStrings; // kind string, kind enum
	QMap<QString, QPixmap> mPixmaps;
};

void qCtagsSenseUtils::initMaps()
{
	if ( mLanguageChars.isEmpty() )
	{
		// language kinds
		CharKindPairList asmList = CharKindPairList()
			<< CharKindPair( 'd', qCtagsSense::Define )
			<< CharKindPair( 'l', qCtagsSense::Label )
			<< CharKindPair( 'm', qCtagsSense::Macro )
			<< CharKindPair( 't', qCtagsSense::Type );
		
		CharKindPairList aspList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Constant )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 's', qCtagsSense::SubRoutine )
			<< CharKindPair( 'v', qCtagsSense::Variable );
		
		CharKindPairList awkList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSense::Function );
		
		CharKindPairList basicList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Constant )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'l', qCtagsSense::Label )
			<< CharKindPair( 't', qCtagsSense::Type )
			<< CharKindPair( 'v', qCtagsSense::Variable )
			<< CharKindPair( 'g', qCtagsSense::Enumeration );
		
		CharKindPairList betaList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSense::Fragment )
			<< CharKindPair( 'p', qCtagsSense::AllPatterns ) // off
			<< CharKindPair( 's', qCtagsSense::Slot )
			<< CharKindPair( 'v', qCtagsSense::Pattern );
		
		CharKindPairList cList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'd', qCtagsSense::Macro )
			<< CharKindPair( 'e', qCtagsSense::Enumerator )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'g', qCtagsSense::Enumeration )
			<< CharKindPair( 'l', qCtagsSense::VariableLocal ) // off
			<< CharKindPair( 'm', qCtagsSense::Member )
			<< CharKindPair( 'n', qCtagsSense::Namespace )
			<< CharKindPair( 'p', qCtagsSense::Prototype ) // off
			<< CharKindPair( 's', qCtagsSense::Structure )
			<< CharKindPair( 't', qCtagsSense::Typedef )
			<< CharKindPair( 'u', qCtagsSense::Union )
			<< CharKindPair( 'v', qCtagsSense::Variable )
			<< CharKindPair( 'x', qCtagsSense::VariableGlobal ); // off
		
		CharKindPairList cppList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'd', qCtagsSense::Macro )
			<< CharKindPair( 'e', qCtagsSense::Enumerator )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'g', qCtagsSense::Enumeration )
			<< CharKindPair( 'l', qCtagsSense::VariableLocal ) // off
			<< CharKindPair( 'm', qCtagsSense::Member )
			<< CharKindPair( 'n', qCtagsSense::Namespace )
			<< CharKindPair( 'p', qCtagsSense::Prototype ) // off
			<< CharKindPair( 's', qCtagsSense::Structure )
			<< CharKindPair( 't', qCtagsSense::Typedef )
			<< CharKindPair( 'u', qCtagsSense::Union )
			<< CharKindPair( 'v', qCtagsSense::Variable )
			<< CharKindPair( 'x', qCtagsSense::VariableGlobal ); // off
		
		CharKindPairList csharpList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'd', qCtagsSense::Macro )
			<< CharKindPair( 'e', qCtagsSense::Enumerator )
			<< CharKindPair( 'E', qCtagsSense::Event )
			<< CharKindPair( 'f', qCtagsSense::Field )
			<< CharKindPair( 'g', qCtagsSense::Enumeration )
			<< CharKindPair( 'i', qCtagsSense::Interface )
			<< CharKindPair( 'l', qCtagsSense::VariableLocal ) // off
			<< CharKindPair( 'm', qCtagsSense::Method )
			<< CharKindPair( 'n', qCtagsSense::Namespace )
			<< CharKindPair( 'p', qCtagsSense::Property )
			<< CharKindPair( 's', qCtagsSense::Structure )
			<< CharKindPair( 't', qCtagsSense::Typedef );
		
		CharKindPairList cobolList = CharKindPairList()
			<< CharKindPair( 'd', qCtagsSense::ItemData )
			<< CharKindPair( 'f', qCtagsSense::FileDescriptor )
			<< CharKindPair( 'g', qCtagsSense::ItemGroup )
			<< CharKindPair( 'p', qCtagsSense::Paragraph )
			<< CharKindPair( 'P', qCtagsSense::ProgramId )
			<< CharKindPair( 's', qCtagsSense::Section );
		
		CharKindPairList eiffelList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'f', qCtagsSense::Feature )
			<< CharKindPair( 'l', qCtagsSense::VariableLocal ); // off
		
		CharKindPairList erlangList = CharKindPairList()
			<< CharKindPair( 'd', qCtagsSense::Macro )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'm', qCtagsSense::Module )
			<< CharKindPair( 'r', qCtagsSense::Record );
		
		CharKindPairList fortranList = CharKindPairList()
			<< CharKindPair( 'b', qCtagsSense::BlockData )
			<< CharKindPair( 'c', qCtagsSense::BlockCommon )
			<< CharKindPair( 'e', qCtagsSense::EntryPoint )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'i', qCtagsSense::Interface ) // off
			<< CharKindPair( 'k', qCtagsSense::Type )
			<< CharKindPair( 'l', qCtagsSense::Label )
			<< CharKindPair( 'L', qCtagsSense::VariableLocal ) // off
			<< CharKindPair( 'm', qCtagsSense::Module )
			<< CharKindPair( 'n', qCtagsSense::NameList )
			<< CharKindPair( 'p', qCtagsSense::Program )
			<< CharKindPair( 's', qCtagsSense::SubRoutine )
			<< CharKindPair( 't', qCtagsSense::DerivatedType )
			<< CharKindPair( 'v', qCtagsSense::Variable );
		
		CharKindPairList htmlList = CharKindPairList()
			<< CharKindPair( 'a', qCtagsSense::Anchor )
			<< CharKindPair( 'f', qCtagsSense::JavaScriptFunction );
		
		CharKindPairList javaList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'e', qCtagsSense::Enumeration )
			<< CharKindPair( 'f', qCtagsSense::Field )
			<< CharKindPair( 'g', qCtagsSense::EnumType )
			<< CharKindPair( 'i', qCtagsSense::Interface )
			<< CharKindPair( 'l', qCtagsSense::VariableLocal ) // off
			<< CharKindPair( 'm', qCtagsSense::Method )
			<< CharKindPair( 'p', qCtagsSense::Package );
		
		CharKindPairList javascriptList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'm', qCtagsSense::Method )
			<< CharKindPair( 'v', qCtagsSense::VariableGlobal );
		
		CharKindPairList lispList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSense::Function );
		
		CharKindPairList luaList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSense::Function );
		
		CharKindPairList makeList = CharKindPairList()
			<< CharKindPair( 'm', qCtagsSense::Macro );
		
		CharKindPairList pascalList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'p', qCtagsSense::Procedure );
	
		CharKindPairList perlList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Constant )
			<< CharKindPair( 'f', qCtagsSense::Format )
			<< CharKindPair( 'l', qCtagsSense::Label )
			<< CharKindPair( 'p', qCtagsSense::Package )
			<< CharKindPair( 's', qCtagsSense::SubRoutine )
			<< CharKindPair( 'd', qCtagsSense::SubRoutineDeclaration ); // off
		
		CharKindPairList phpList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'i', qCtagsSense::Interface )
			<< CharKindPair( 'd', qCtagsSense::Constant )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'v', qCtagsSense::Variable )
			<< CharKindPair( 'j', qCtagsSense::JavaScriptFunction );
		
		CharKindPairList pythonList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'm', qCtagsSense::Member );
		
		CharKindPairList rexxList = CharKindPairList()
			<< CharKindPair( 's', qCtagsSense::SubRoutine );
		
		CharKindPairList rubyList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'f', qCtagsSense::Method )
			<< CharKindPair( 'm', qCtagsSense::Module )
			<< CharKindPair( 'F', qCtagsSense::MethodSingleton );
		
		CharKindPairList schemeList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 's', qCtagsSense::Set );
		
		CharKindPairList shList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSense::Function );
		
		CharKindPairList slangList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'n', qCtagsSense::Namespace );
		
		CharKindPairList smlList = CharKindPairList()
			<< CharKindPair( 'e', qCtagsSense::Exception )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'c', qCtagsSense::Functor )
			<< CharKindPair( 's', qCtagsSense::Signature )
			<< CharKindPair( 'r', qCtagsSense::Structure )
			<< CharKindPair( 't', qCtagsSense::Type )
			<< CharKindPair( 'v', qCtagsSense::ValueBinding );
		
		CharKindPairList sqlList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Cursor )
			<< CharKindPair( 'd', qCtagsSense::Prototype ) // off
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'F', qCtagsSense::Field )
			<< CharKindPair( 'l', qCtagsSense::VariableLocal ) // off
			<< CharKindPair( 'L', qCtagsSense::Label )
			<< CharKindPair( 'P', qCtagsSense::Package )
			<< CharKindPair( 'p', qCtagsSense::Procedure )
			<< CharKindPair( 'r', qCtagsSense::Record )
			<< CharKindPair( 's', qCtagsSense::SubType )
			<< CharKindPair( 't', qCtagsSense::Table )
			<< CharKindPair( 'T', qCtagsSense::Trigger )
			<< CharKindPair( 'v', qCtagsSense::Variable )
			<< CharKindPair( 'i', qCtagsSense::Index )
			<< CharKindPair( 'e', qCtagsSense::Event )
			<< CharKindPair( 'U', qCtagsSense::Publication )
			<< CharKindPair( 'R', qCtagsSense::Service )
			<< CharKindPair( 'D', qCtagsSense::Domain )
			<< CharKindPair( 'V', qCtagsSense::View )
			<< CharKindPair( 'n', qCtagsSense::Synonym )
			<< CharKindPair( 'x', qCtagsSense::MobiLinkTableScript )
			<< CharKindPair( 'y', qCtagsSense::MobiLinkConnScript );
		
		CharKindPairList tclList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'm', qCtagsSense::Method )
			<< CharKindPair( 'p', qCtagsSense::Procedure );
		
		CharKindPairList veraList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'd', qCtagsSense::Macro )
			<< CharKindPair( 'e', qCtagsSense::Enumerator )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'g', qCtagsSense::Enumeration )
			<< CharKindPair( 'l', qCtagsSense::VariableLocal )
			<< CharKindPair( 'm', qCtagsSense::Member )
			<< CharKindPair( 'p', qCtagsSense::Program )
			<< CharKindPair( 'P', qCtagsSense::Prototype )
			<< CharKindPair( 't', qCtagsSense::Task )
			<< CharKindPair( 'T', qCtagsSense::Typedef )
			<< CharKindPair( 'v', qCtagsSense::Variable )
			<< CharKindPair( 'x', qCtagsSense::VariableGlobal );
		
		CharKindPairList verilogList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Constant )
			<< CharKindPair( 'e', qCtagsSense::Event )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'm', qCtagsSense::Module )
			<< CharKindPair( 'n', qCtagsSense::NetDataType )
			<< CharKindPair( 'p', qCtagsSense::Port )
			<< CharKindPair( 'r', qCtagsSense::RegisterDataType )
			<< CharKindPair( 't', qCtagsSense::Task );
		
		CharKindPairList vimList = CharKindPairList()
			<< CharKindPair( 'a', qCtagsSense::AutoCommandGroup )
			<< CharKindPair( 'c', qCtagsSense::UserDefinedCommand )
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'm', qCtagsSense::Map )
			<< CharKindPair( 'v', qCtagsSense::Variable );
		
		CharKindPairList yaccList = CharKindPairList()
			<< CharKindPair( 'l', qCtagsSense::Label );
		
		// fill language kinds map
		mLanguageChars[ "Asm" ] = asmList;
		mLanguageChars[ "Asp" ] = aspList;
		mLanguageChars[ "Awk" ] = awkList;
		mLanguageChars[ "Basic" ] = basicList;
		mLanguageChars[ "BETA" ] = betaList;
		mLanguageChars[ "C" ] = cList;
		mLanguageChars[ "C++" ] = cppList;
		mLanguageChars[ "C#" ] = csharpList;
		mLanguageChars[ "Cobol" ] = cobolList;
		mLanguageChars[ "Eiffel" ] = eiffelList;
		mLanguageChars[ "Erlang" ] = erlangList;
		mLanguageChars[ "Fortran" ] = fortranList;
		mLanguageChars[ "HTML" ] = htmlList;
		mLanguageChars[ "Java" ] = javaList;
		mLanguageChars[ "JavaScript" ] = javascriptList;
		mLanguageChars[ "Lisp" ] = lispList;
		mLanguageChars[ "Lua" ] = luaList;
		mLanguageChars[ "Make" ] = makeList;
		mLanguageChars[ "Pascal" ] = pascalList;
		mLanguageChars[ "Perl" ] = perlList;
		mLanguageChars[ "PHP" ] = phpList;
		mLanguageChars[ "Python" ] = pythonList;
		mLanguageChars[ "REXX" ] = rexxList;
		mLanguageChars[ "Ruby" ] = rubyList;
		mLanguageChars[ "Scheme" ] = schemeList;
		mLanguageChars[ "Sh" ] = shList;
		mLanguageChars[ "SLang" ] = slangList;
		mLanguageChars[ "SML" ] = smlList;
		mLanguageChars[ "SQL" ] = sqlList;
		mLanguageChars[ "Tcl" ] = tclList;
		mLanguageChars[ "Vera" ] = veraList;
		mLanguageChars[ "Verilog" ] = verilogList;
		mLanguageChars[ "Vim" ] = vimList;
		mLanguageChars[ "YACC" ] = yaccList;
		
		// kind strings
		mKindStrings[ "class" ] = qCtagsSense::Class;
		mKindStrings[ "enum" ] = qCtagsSense::Enumeration;
		mKindStrings[ "function" ] = qCtagsSense::Function;
		mKindStrings[ "namespace" ] = qCtagsSense::Namespace;
		mKindStrings[ "struct" ] = qCtagsSense::Structure;
		mKindStrings[ "union" ] = qCtagsSense::Union;
	
		// kind text
		mKindText[ qCtagsSense::Class ] = QCoreApplication::translate( "qCtagsSense", "Class" );
		mKindText[ qCtagsSense::Macro ] = QCoreApplication::translate( "qCtagsSense", "Macro" );
		mKindText[ qCtagsSense::Enumerator ] = QCoreApplication::translate( "qCtagsSense", "Enumerator" );
		mKindText[ qCtagsSense::Function ] = QCoreApplication::translate( "qCtagsSense", "Function" );
		mKindText[ qCtagsSense::Enumeration ] = QCoreApplication::translate( "qCtagsSense", "Enum" );
		mKindText[ qCtagsSense::VariableLocal ] = QCoreApplication::translate( "qCtagsSense", "Local Variable" );
		mKindText[ qCtagsSense::Member ] = QCoreApplication::translate( "qCtagsSense", "Member" );
		mKindText[ qCtagsSense::Namespace ] = QCoreApplication::translate( "qCtagsSense", "Namespace" );
		mKindText[ qCtagsSense::Prototype ] = QCoreApplication::translate( "qCtagsSense", "Prototype" );
		mKindText[ qCtagsSense::Structure ] = QCoreApplication::translate( "qCtagsSense", "Structure" );
		mKindText[ qCtagsSense::Typedef ] = QCoreApplication::translate( "qCtagsSense", "Typedef" );
		mKindText[ qCtagsSense::Union ] = QCoreApplication::translate( "qCtagsSense", "Union" );
		mKindText[ qCtagsSense::Variable ] = QCoreApplication::translate( "qCtagsSense", "Variable" );
		mKindText[ qCtagsSense::VariableGlobal ] = QCoreApplication::translate( "qCtagsSense", "Extern Variable" );
		mKindText[ qCtagsSense::Unknow ] = QCoreApplication::translate( "qCtagsSense", "Unknow" );
	}
}

qCtagsSense::Kind qCtagsSenseUtils::kindType( const QChar& c, const QString& language )
{
	const CharKindPairList kinds = mLanguageChars.value( language );
	
	foreach ( const CharKindPair& pair, kinds )
	{
		if ( c == pair.first )
		{
			return pair.second;
		}
	}
	
	return qCtagsSense::Unknow;
}

qCtagsSense::Kind qCtagsSenseUtils::kindType( const QString& string )
{
	return mKindStrings.value( string, qCtagsSense::Unknow );
}

QChar qCtagsSenseUtils::kindChar( qCtagsSense::Kind kind, const QString& language )
{
	const CharKindPairList kinds = mLanguageChars.value( language );
	
	foreach ( const CharKindPair& pair, kinds )
	{
		if ( kind == pair.second )
		{
			return pair.first;
		}
	}
	
	return QChar();
}

QString qCtagsSenseUtils::kindText( qCtagsSense::Kind kind )
{
	return mKindText.value( kind, "Unknow kind - please update the map" );
}

QString qCtagsSenseUtils::kindText( const QChar& c, const QString& language )
{
	return kindText( kindType( c, language ) );
}

bool qCtagsSenseUtils::caseInsensitiveFilePathLessThan( const QString& s1, const QString& s2 )
{
	return QFileInfo( s1 ).fileName().toLower() < QFileInfo( s2 ).fileName().toLower();
}

qCtagsSenseEntry* qCtagsSenseUtils::entryForRecord( const QSqlRecord& rec, const QString& fileName )
{
	qCtagsSenseEntry* entry = new qCtagsSenseEntry;
	
	entry->fileName = fileName.isEmpty() ? rec.value( "fileName" ).toString() : fileName;
	entry->language = rec.value( "language" ).toString();
	entry->lineNumberEntry = rec.value( "line_number_entry" ).toBool();
	entry->lineNumber = rec.value( "line_number" ).value<ulong>();
	entry->isFileScope = rec.value( "is_file_scope" ).toBool();
	entry->isFileEntry = rec.value( "is_file_entry" ).toBool();
	entry->truncateLine = rec.value( "truncate_line" ).toBool();
	entry->name = rec.value( "name" ).toString();
	entry->kind = (qCtagsSense::Kind)rec.value( "kind" ).toInt();
	// extended fields
	entry->access = rec.value( "access" ).toString();
	entry->fileScope = rec.value( "file_scope" ).toString();
	entry->implementation = rec.value( "implementation" ).toString();
	entry->inheritance = rec.value( "inheritance" ).toString();
	entry->scope.first = rec.value( "scope_value" ).toString();
	entry->scope.second = rec.value( "scope_key" ).toString();
	entry->signature = rec.value( "signature" ).toString();
	entry->typeRef.first = rec.value( "type" ).toString();
	entry->typeRef.second = rec.value( "type_name" ).toString();
	
	return entry;
}

QString qCtagsSenseUtils::entryDecorationFilePath( const qCtagsSenseEntry* entry )
{
	qCtagsSense::Kind kind = entry->kind;
	QString access = entry->access;
	QString name;
	
	if ( access == "public" )
	{
		access.clear();
	}
	
	switch ( kind )
	{
		case qCtagsSense::Class:
			name = "class";
			access.clear();
			break;
		case qCtagsSense::Macro:
			name = "macro";
			access.clear();
			break;
		case qCtagsSense::Enumerator:
			name = "enumerator";
			access.clear();
			break;
		case qCtagsSense::Function:
		case qCtagsSense::Prototype:
			name = "function";
			break;
		case qCtagsSense::Enumeration:
			name = "enum";
			access.clear();
			break;
		case qCtagsSense::VariableLocal:
		case qCtagsSense::Member:
		case qCtagsSense::Variable:
		case qCtagsSense::VariableGlobal:
			name = "variable";
			break;
		case qCtagsSense::Namespace:
			name = "namespace";
			access.clear();
			break;
		case qCtagsSense::Structure:
			name = "structure";
			access.clear();
			break;
		case qCtagsSense::Typedef:
			name = "typedef";
			access.clear();
			break;
		case qCtagsSense::Union:
			name = "union";
			break;
		case qCtagsSense::Unknow:
			name = "unknow";
			break;
		default:
			#ifdef Q_CC_MSVC
			#pragma message("handle all enumerator")
			#else
			#warning handle all enumerator
			#endif
			break;
	}
	
	QString fn = name;
	
	if ( !access.isEmpty() )
	{
		fn += "_" +access;
	}
	
	fn += ".png";
	fn = QString( ":/icons/%1" ).arg( fn );
	
	//qWarning() << fn;
	
	if ( !mPixmaps.contains( fn ) )
	{
		mPixmaps[ fn ] = QPixmap( fn );
	}
	
	return fn;
}

QPixmap qCtagsSenseUtils::entryDecoration( const qCtagsSenseEntry* entry )
{
	return mPixmaps.value( entryDecorationFilePath( entry ) );
}

QString qCtagsSenseUtils::entryDisplay( const qCtagsSenseEntry* entry )
{
	qCtagsSenseEntry* parent = entry->parent;
	qCtagsSenseEntry* granParent = parent ? parent->parent : 0;
	QString display = entry->name;
	
	if ( !granParent )
	{
		if ( !entry->scope.second.isEmpty() )
		{
			display.prepend( QString( "%1::" ).arg( entry->scope.second ) );
		}
	}
	
	switch ( entry->kind )
	{
		case qCtagsSense::Function:
		case qCtagsSense::Prototype:
			//display.append( entry->signature );
			break;
		case qCtagsSense::VariableLocal:
		case qCtagsSense::Member:
		case qCtagsSense::Variable:
		case qCtagsSense::VariableGlobal:
			if ( !entry->typeRef.first.isEmpty() )
			{
				display.append( QString( ": %1" ).arg( entry->typeRef.first ) );
			}
			break;
		case qCtagsSense::Class:
		case qCtagsSense::Structure:
		case qCtagsSense::Union:
		case qCtagsSense::Macro:
		case qCtagsSense::Enumerator:
		case qCtagsSense::Enumeration:
		case qCtagsSense::Namespace:
		case qCtagsSense::Typedef:
		case qCtagsSense::Unknow:
			break;
		default:
			#ifdef Q_CC_MSVC
			#pragma message("handle all enumerator")
			#else
			#warning handle all enumerator
			#endif
			break;
	}
	
	return display;
}

QString qCtagsSenseUtils::entryToolTip( const qCtagsSenseEntry* entry )
{
	QString tooltip;
	
	tooltip += QString( "<b>*** %1 %2</b><br />(<u>%3:%4</u>)<br />" )
		.arg( QString( "<img src=\"%1\" /> " ).arg( entryDecorationFilePath( entry ) ) )
		.arg( kindText( entry->kind ) )
		.arg( QFileInfo( entry->fileName ).fileName() )
		.arg( entry->lineNumber +1 );
	
	if ( !entry->inheritance.isEmpty() )
	{
		tooltip += QString( "<i>Inherits:</i> %1<br />" ).arg( entry->inheritance );
	}
	
	if ( !entry->access.isEmpty() )
	{
		tooltip += QString( "<i>[%1]</i> " ).arg( entry->access );
	}
	
	if ( !entry->implementation.isEmpty() )
	{
		tooltip += QString( "<i>[%1]</i> " ).arg( entry->implementation );
	}
	
	if ( !entry->typeRef.first.isEmpty() )
	{
		tooltip += QString( "<u><i>%1</i></u> " ).arg( entry->typeRef.first );
	}
	
	if ( !entry->typeRef.second.isEmpty() )
	{
		tooltip += QString( "%1 " ).arg( entry->typeRef.second );
	}
	
	if ( entry->scope.second.isEmpty() )
	{
		tooltip += entry->name;
	}
	else
	{
		tooltip += QString( "%1::%2" ).arg( entry->scope.second ).arg( entry->name );
	}
	
	tooltip += QString( "<i><b>%1</b></i>" ).arg( entry->signature );
	
	return tooltip;
}

QFileInfoList qCtagsSenseUtils::getFiles( QDir fromDir, const QStringList& filters, bool recursive )
{
	QFileInfoList files;
	foreach ( QFileInfo file, fromDir.entryInfoList( QDir::AllEntries | QDir::AllDirs | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name ) )
	{
		if ( file.isFile() && ( filters.isEmpty() || QDir::match( filters, file.fileName() ) ) )
			files << file;
		else if ( file.isDir() && recursive )
		{
			fromDir.cd( file.filePath() );
			files << getFiles( fromDir, filters, recursive );
			fromDir.cdUp();
		}
	}
	return files;
}
