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

#include <QCoreApplication>
#include <QMap>
#include <QVariant>

// Fileds
/*
	flags: "afmikKlnsStz") [fks]. 
*/

namespace qCtagsSenseUtils
{
	typedef QPair<QChar, qCtagsSenseEntry::Kind> CharKindPair; // kind char, kind enum
	typedef QList<CharKindPair> CharKindPairList;
	
	QMap<qCtagsSenseEntry::Kind, QString> mKindText;
	QMap<QString, CharKindPairList> mLanguageChars; // language, list of pair of char/kind
	QMap<QString, qCtagsSenseEntry::Kind> mKindStrings; // kind string, kind enum
	QMap<QString, QPixmap> mPixmaps; // kind filename, pixmap
};

void qCtagsSenseUtils::initialize()
{
	if ( mLanguageChars.isEmpty() )
	{
		// language kinds
		CharKindPairList asmList = CharKindPairList()
			<< CharKindPair( 'd', qCtagsSenseEntry::Define )
			<< CharKindPair( 'l', qCtagsSenseEntry::Label )
			<< CharKindPair( 'm', qCtagsSenseEntry::Macro )
			<< CharKindPair( 't', qCtagsSenseEntry::Type );
		
		CharKindPairList aspList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Constant )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 's', qCtagsSenseEntry::SubRoutine )
			<< CharKindPair( 'v', qCtagsSenseEntry::Variable );
		
		CharKindPairList awkList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSenseEntry::Function );
		
		CharKindPairList basicList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Constant )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'l', qCtagsSenseEntry::Label )
			<< CharKindPair( 't', qCtagsSenseEntry::Type )
			<< CharKindPair( 'v', qCtagsSenseEntry::Variable )
			<< CharKindPair( 'g', qCtagsSenseEntry::Enumeration );
		
		CharKindPairList betaList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSenseEntry::Fragment )
			<< CharKindPair( 'p', qCtagsSenseEntry::AllPatterns ) // off
			<< CharKindPair( 's', qCtagsSenseEntry::Slot )
			<< CharKindPair( 'v', qCtagsSenseEntry::Pattern );
		
		CharKindPairList cList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'd', qCtagsSenseEntry::Macro )
			<< CharKindPair( 'e', qCtagsSenseEntry::Enumerator )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'g', qCtagsSenseEntry::Enumeration )
			<< CharKindPair( 'l', qCtagsSenseEntry::VariableLocal ) // off
			<< CharKindPair( 'm', qCtagsSenseEntry::Member )
			<< CharKindPair( 'n', qCtagsSenseEntry::Namespace )
			<< CharKindPair( 'p', qCtagsSenseEntry::Prototype ) // off
			<< CharKindPair( 's', qCtagsSenseEntry::Structure )
			<< CharKindPair( 't', qCtagsSenseEntry::Typedef )
			<< CharKindPair( 'u', qCtagsSenseEntry::Union )
			<< CharKindPair( 'v', qCtagsSenseEntry::Variable )
			<< CharKindPair( 'x', qCtagsSenseEntry::VariableGlobal ); // off
		
		CharKindPairList cppList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'd', qCtagsSenseEntry::Macro )
			<< CharKindPair( 'e', qCtagsSenseEntry::Enumerator )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'g', qCtagsSenseEntry::Enumeration )
			<< CharKindPair( 'l', qCtagsSenseEntry::VariableLocal ) // off
			<< CharKindPair( 'm', qCtagsSenseEntry::Member )
			<< CharKindPair( 'n', qCtagsSenseEntry::Namespace )
			<< CharKindPair( 'p', qCtagsSenseEntry::Prototype ) // off
			<< CharKindPair( 's', qCtagsSenseEntry::Structure )
			<< CharKindPair( 't', qCtagsSenseEntry::Typedef )
			<< CharKindPair( 'u', qCtagsSenseEntry::Union )
			<< CharKindPair( 'v', qCtagsSenseEntry::Variable )
			<< CharKindPair( 'x', qCtagsSenseEntry::VariableGlobal ); // off
		
		CharKindPairList csharpList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'd', qCtagsSenseEntry::Macro )
			<< CharKindPair( 'e', qCtagsSenseEntry::Enumerator )
			<< CharKindPair( 'E', qCtagsSenseEntry::Event )
			<< CharKindPair( 'f', qCtagsSenseEntry::Field )
			<< CharKindPair( 'g', qCtagsSenseEntry::Enumeration )
			<< CharKindPair( 'i', qCtagsSenseEntry::Interface )
			<< CharKindPair( 'l', qCtagsSenseEntry::VariableLocal ) // off
			<< CharKindPair( 'm', qCtagsSenseEntry::Method )
			<< CharKindPair( 'n', qCtagsSenseEntry::Namespace )
			<< CharKindPair( 'p', qCtagsSenseEntry::Property )
			<< CharKindPair( 's', qCtagsSenseEntry::Structure )
			<< CharKindPair( 't', qCtagsSenseEntry::Typedef );
		
		CharKindPairList cobolList = CharKindPairList()
			<< CharKindPair( 'd', qCtagsSenseEntry::ItemData )
			<< CharKindPair( 'f', qCtagsSenseEntry::FileDescriptor )
			<< CharKindPair( 'g', qCtagsSenseEntry::ItemGroup )
			<< CharKindPair( 'p', qCtagsSenseEntry::Paragraph )
			<< CharKindPair( 'P', qCtagsSenseEntry::ProgramId )
			<< CharKindPair( 's', qCtagsSenseEntry::Section );
		
		CharKindPairList eiffelList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'f', qCtagsSenseEntry::Feature )
			<< CharKindPair( 'l', qCtagsSenseEntry::VariableLocal ); // off
		
		CharKindPairList erlangList = CharKindPairList()
			<< CharKindPair( 'd', qCtagsSenseEntry::Macro )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'm', qCtagsSenseEntry::Module )
			<< CharKindPair( 'r', qCtagsSenseEntry::Record );
		
		CharKindPairList fortranList = CharKindPairList()
			<< CharKindPair( 'b', qCtagsSenseEntry::BlockData )
			<< CharKindPair( 'c', qCtagsSenseEntry::BlockCommon )
			<< CharKindPair( 'e', qCtagsSenseEntry::EntryPoint )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'i', qCtagsSenseEntry::Interface ) // off
			<< CharKindPair( 'k', qCtagsSenseEntry::Type )
			<< CharKindPair( 'l', qCtagsSenseEntry::Label )
			<< CharKindPair( 'L', qCtagsSenseEntry::VariableLocal ) // off
			<< CharKindPair( 'm', qCtagsSenseEntry::Module )
			<< CharKindPair( 'n', qCtagsSenseEntry::NameList )
			<< CharKindPair( 'p', qCtagsSenseEntry::Program )
			<< CharKindPair( 's', qCtagsSenseEntry::SubRoutine )
			<< CharKindPair( 't', qCtagsSenseEntry::DerivatedType )
			<< CharKindPair( 'v', qCtagsSenseEntry::Variable );
		
		CharKindPairList htmlList = CharKindPairList()
			<< CharKindPair( 'a', qCtagsSenseEntry::Anchor )
			<< CharKindPair( 'f', qCtagsSenseEntry::JavaScriptFunction );
		
		CharKindPairList javaList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'e', qCtagsSenseEntry::Enumeration )
			<< CharKindPair( 'f', qCtagsSenseEntry::Field )
			<< CharKindPair( 'g', qCtagsSenseEntry::EnumType )
			<< CharKindPair( 'i', qCtagsSenseEntry::Interface )
			<< CharKindPair( 'l', qCtagsSenseEntry::VariableLocal ) // off
			<< CharKindPair( 'm', qCtagsSenseEntry::Method )
			<< CharKindPair( 'p', qCtagsSenseEntry::Package );
		
		CharKindPairList javascriptList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'm', qCtagsSenseEntry::Method )
			<< CharKindPair( 'v', qCtagsSenseEntry::VariableGlobal );
		
		CharKindPairList lispList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSenseEntry::Function );
		
		CharKindPairList luaList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSenseEntry::Function );
		
		CharKindPairList makeList = CharKindPairList()
			<< CharKindPair( 'm', qCtagsSenseEntry::Macro );
		
		CharKindPairList pascalList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'p', qCtagsSenseEntry::Procedure );
	
		CharKindPairList perlList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Constant )
			<< CharKindPair( 'f', qCtagsSenseEntry::Format )
			<< CharKindPair( 'l', qCtagsSenseEntry::Label )
			<< CharKindPair( 'p', qCtagsSenseEntry::Package )
			<< CharKindPair( 's', qCtagsSenseEntry::SubRoutine )
			<< CharKindPair( 'd', qCtagsSenseEntry::SubRoutineDeclaration ); // off
		
		CharKindPairList phpList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'i', qCtagsSenseEntry::Interface )
			<< CharKindPair( 'd', qCtagsSenseEntry::Constant )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'v', qCtagsSenseEntry::Variable )
			<< CharKindPair( 'j', qCtagsSenseEntry::JavaScriptFunction );
		
		CharKindPairList pythonList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'm', qCtagsSenseEntry::Member );
		
		CharKindPairList rexxList = CharKindPairList()
			<< CharKindPair( 's', qCtagsSenseEntry::SubRoutine );
		
		CharKindPairList rubyList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'f', qCtagsSenseEntry::Method )
			<< CharKindPair( 'm', qCtagsSenseEntry::Module )
			<< CharKindPair( 'F', qCtagsSenseEntry::MethodSingleton );
		
		CharKindPairList schemeList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 's', qCtagsSenseEntry::Set );
		
		CharKindPairList shList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSenseEntry::Function );
		
		CharKindPairList slangList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'n', qCtagsSenseEntry::Namespace );
		
		CharKindPairList smlList = CharKindPairList()
			<< CharKindPair( 'e', qCtagsSenseEntry::Exception )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'c', qCtagsSenseEntry::Functor )
			<< CharKindPair( 's', qCtagsSenseEntry::Signature )
			<< CharKindPair( 'r', qCtagsSenseEntry::Structure )
			<< CharKindPair( 't', qCtagsSenseEntry::Type )
			<< CharKindPair( 'v', qCtagsSenseEntry::ValueBinding );
		
		CharKindPairList sqlList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Cursor )
			<< CharKindPair( 'd', qCtagsSenseEntry::Prototype ) // off
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'F', qCtagsSenseEntry::Field )
			<< CharKindPair( 'l', qCtagsSenseEntry::VariableLocal ) // off
			<< CharKindPair( 'L', qCtagsSenseEntry::Label )
			<< CharKindPair( 'P', qCtagsSenseEntry::Package )
			<< CharKindPair( 'p', qCtagsSenseEntry::Procedure )
			<< CharKindPair( 'r', qCtagsSenseEntry::Record )
			<< CharKindPair( 's', qCtagsSenseEntry::SubType )
			<< CharKindPair( 't', qCtagsSenseEntry::Table )
			<< CharKindPair( 'T', qCtagsSenseEntry::Trigger )
			<< CharKindPair( 'v', qCtagsSenseEntry::Variable )
			<< CharKindPair( 'i', qCtagsSenseEntry::Index )
			<< CharKindPair( 'e', qCtagsSenseEntry::Event )
			<< CharKindPair( 'U', qCtagsSenseEntry::Publication )
			<< CharKindPair( 'R', qCtagsSenseEntry::Service )
			<< CharKindPair( 'D', qCtagsSenseEntry::Domain )
			<< CharKindPair( 'V', qCtagsSenseEntry::View )
			<< CharKindPair( 'n', qCtagsSenseEntry::Synonym )
			<< CharKindPair( 'x', qCtagsSenseEntry::MobiLinkTableScript )
			<< CharKindPair( 'y', qCtagsSenseEntry::MobiLinkConnScript );
		
		CharKindPairList tclList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'm', qCtagsSenseEntry::Method )
			<< CharKindPair( 'p', qCtagsSenseEntry::Procedure );
		
		CharKindPairList veraList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Class )
			<< CharKindPair( 'd', qCtagsSenseEntry::Macro )
			<< CharKindPair( 'e', qCtagsSenseEntry::Enumerator )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'g', qCtagsSenseEntry::Enumeration )
			<< CharKindPair( 'l', qCtagsSenseEntry::VariableLocal )
			<< CharKindPair( 'm', qCtagsSenseEntry::Member )
			<< CharKindPair( 'p', qCtagsSenseEntry::Program )
			<< CharKindPair( 'P', qCtagsSenseEntry::Prototype )
			<< CharKindPair( 't', qCtagsSenseEntry::Task )
			<< CharKindPair( 'T', qCtagsSenseEntry::Typedef )
			<< CharKindPair( 'v', qCtagsSenseEntry::Variable )
			<< CharKindPair( 'x', qCtagsSenseEntry::VariableGlobal );
		
		CharKindPairList verilogList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSenseEntry::Constant )
			<< CharKindPair( 'e', qCtagsSenseEntry::Event )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'm', qCtagsSenseEntry::Module )
			<< CharKindPair( 'n', qCtagsSenseEntry::NetDataType )
			<< CharKindPair( 'p', qCtagsSenseEntry::Port )
			<< CharKindPair( 'r', qCtagsSenseEntry::RegisterDataType )
			<< CharKindPair( 't', qCtagsSenseEntry::Task );
		
		CharKindPairList vimList = CharKindPairList()
			<< CharKindPair( 'a', qCtagsSenseEntry::AutoCommandGroup )
			<< CharKindPair( 'c', qCtagsSenseEntry::UserDefinedCommand )
			<< CharKindPair( 'f', qCtagsSenseEntry::Function )
			<< CharKindPair( 'm', qCtagsSenseEntry::Map )
			<< CharKindPair( 'v', qCtagsSenseEntry::Variable );
		
		CharKindPairList yaccList = CharKindPairList()
			<< CharKindPair( 'l', qCtagsSenseEntry::Label );
		
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
		mKindStrings[ "class" ] = qCtagsSenseEntry::Class;
		mKindStrings[ "enum" ] = qCtagsSenseEntry::Enumeration;
		mKindStrings[ "function" ] = qCtagsSenseEntry::Function;
		mKindStrings[ "namespace" ] = qCtagsSenseEntry::Namespace;
		mKindStrings[ "struct" ] = qCtagsSenseEntry::Structure;
		mKindStrings[ "union" ] = qCtagsSenseEntry::Union;
	
		// kind text
		mKindText[ qCtagsSenseEntry::Class ] = QCoreApplication::translate( "qCtagsSense", "Class" );
		mKindText[ qCtagsSenseEntry::Macro ] = QCoreApplication::translate( "qCtagsSense", "Macro" );
		mKindText[ qCtagsSenseEntry::Enumerator ] = QCoreApplication::translate( "qCtagsSense", "Enumerator" );
		mKindText[ qCtagsSenseEntry::Function ] = QCoreApplication::translate( "qCtagsSense", "Function" );
		mKindText[ qCtagsSenseEntry::Enumeration ] = QCoreApplication::translate( "qCtagsSense", "Enum" );
		mKindText[ qCtagsSenseEntry::VariableLocal ] = QCoreApplication::translate( "qCtagsSense", "Local Variable" );
		mKindText[ qCtagsSenseEntry::Member ] = QCoreApplication::translate( "qCtagsSense", "Member" );
		mKindText[ qCtagsSenseEntry::Namespace ] = QCoreApplication::translate( "qCtagsSense", "Namespace" );
		mKindText[ qCtagsSenseEntry::Prototype ] = QCoreApplication::translate( "qCtagsSense", "Prototype" );
		mKindText[ qCtagsSenseEntry::Structure ] = QCoreApplication::translate( "qCtagsSense", "Structure" );
		mKindText[ qCtagsSenseEntry::Typedef ] = QCoreApplication::translate( "qCtagsSense", "Typedef" );
		mKindText[ qCtagsSenseEntry::Union ] = QCoreApplication::translate( "qCtagsSense", "Union" );
		mKindText[ qCtagsSenseEntry::Variable ] = QCoreApplication::translate( "qCtagsSense", "Variable" );
		mKindText[ qCtagsSenseEntry::VariableGlobal ] = QCoreApplication::translate( "qCtagsSense", "Extern Variable" );
		mKindText[ qCtagsSenseEntry::Unknow ] = QCoreApplication::translate( "qCtagsSense", "Unknow" );
	}
}

qCtagsSenseEntry::Kind qCtagsSenseUtils::kindType( const QChar& c, const QString& language )
{
	const CharKindPairList kinds = mLanguageChars.value( language );
	
	foreach ( const CharKindPair& pair, kinds )
	{
		if ( c == pair.first )
		{
			return pair.second;
		}
	}
	
	return qCtagsSenseEntry::Unknow;
}

qCtagsSenseEntry::Kind qCtagsSenseUtils::kindType( const QString& string )
{
	return mKindStrings.value( string, qCtagsSenseEntry::Unknow );
}

QChar qCtagsSenseUtils::kindChar( qCtagsSenseEntry::Kind kind, const QString& language )
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

QString qCtagsSenseUtils::kindText( qCtagsSenseEntry::Kind kind )
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

QString qCtagsSenseUtils::entryDecorationFilePath( const qCtagsSenseEntry* entry )
{
	qCtagsSenseEntry::Kind kind = entry->kind;
	QString access = entry->access;
	QString name;
	
	if ( access == "public" )
	{
		access.clear();
	}
	
	switch ( kind )
	{
		case qCtagsSenseEntry::Class:
			name = "class";
			access.clear();
			break;
		case qCtagsSenseEntry::Macro:
			name = "macro";
			access.clear();
			break;
		case qCtagsSenseEntry::Enumerator:
			name = "enumerator";
			access.clear();
			break;
		case qCtagsSenseEntry::Function:
		case qCtagsSenseEntry::Prototype:
			name = "function";
			break;
		case qCtagsSenseEntry::Enumeration:
			name = "enum";
			access.clear();
			break;
		case qCtagsSenseEntry::VariableLocal:
		case qCtagsSenseEntry::Member:
		case qCtagsSenseEntry::Variable:
		case qCtagsSenseEntry::VariableGlobal:
			name = "variable";
			break;
		case qCtagsSenseEntry::Namespace:
			name = "namespace";
			access.clear();
			break;
		case qCtagsSenseEntry::Structure:
			name = "structure";
			access.clear();
			break;
		case qCtagsSenseEntry::Typedef:
			name = "typedef";
			access.clear();
			break;
		case qCtagsSenseEntry::Union:
			name = "union";
			break;
		case qCtagsSenseEntry::Unknow:
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
		case qCtagsSenseEntry::Function:
		case qCtagsSenseEntry::Prototype:
			//display.append( entry->signature );
			break;
		case qCtagsSenseEntry::VariableLocal:
		case qCtagsSenseEntry::Member:
		case qCtagsSenseEntry::Variable:
		case qCtagsSenseEntry::VariableGlobal:
			if ( !entry->typeRef.first.isEmpty() )
			{
				display.append( QString( ": %1" ).arg( entry->typeRef.first ) );
			}
			break;
		case qCtagsSenseEntry::Class:
		case qCtagsSenseEntry::Structure:
		case qCtagsSenseEntry::Union:
		case qCtagsSenseEntry::Macro:
		case qCtagsSenseEntry::Enumerator:
		case qCtagsSenseEntry::Enumeration:
		case qCtagsSenseEntry::Namespace:
		case qCtagsSenseEntry::Typedef:
		case qCtagsSenseEntry::Unknow:
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
