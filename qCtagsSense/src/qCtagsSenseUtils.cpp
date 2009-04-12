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

// Kinds
/*
Asm                                        
	d  defines                             
	l  labels                              
	m  macros                              
	t  types (structs and records)         
Asp                                        
	c  constants                           
	f  functions                           
	s  subroutines                         
	v  variables                           
Awk                                        
	f  functions                           
Basic                                      
	c  constants                           
	f  functions                           
	l  labels                              
	t  types                               
	v  variables                           
	g  enumerations                        
BETA                                       
	f  fragment definitions                
	p  all patterns [off]                  
	s  slots (fragment uses)               
	v  patterns (virtual or rebound)       
C / C++                                        
	c  classes                             
	d  macro definitions                   
	e  enumerators (values inside an enumeration)
	f  function definitions                      
	g  enumeration names                         
	l  local variables [off]                     
	m  class, struct, and union members          
	n  namespaces                                
	p  function prototypes [off]                 
	s  structure names                           
	t  typedefs                                  
	u  union names                               
	v  variable definitions                      
	x  external and forward variable declarations [off]
C#                                                     
	c  classes                                         
	d  macro definitions                               
	e  enumerators (values inside an enumeration)      
	E  events                                          
	f  fields                                          
	g  enumeration names                               
	i  interfaces                                      
	l  local variables [off]                           
	m  methods                                         
	n  namespaces                                      
	p  properties                                      
	s  structure names                                 
	t  typedefs                                        
Cobol                                                  
	d  data items                                      
	f  file descriptions (FD, SD, RD)                  
	g  group items                                     
	p  paragraphs                                      
	P  program ids                                     
	s  sections                                        
Eiffel                                                 
	c  classes                                         
	f  features                                        
	l  local entities [off]                            
Erlang                                                 
	d  macro definitions                               
	f  functions                                       
	m  modules                                         
	r  record definitions                              
Fortran                                                
	b  block data                                      
	c  common blocks                                   
	e  entry points                                    
	f  functions                                       
	i  interface contents, generic names, and operators [off]
	k  type and structure components                         
	l  labels                                                
	L  local, common block, and namelist variables [off]     
	m  modules                                               
	n  namelists                                             
	p  programs                                              
	s  subroutines                                           
	t  derived types and structures                          
	v  program (global) and module variables
*/


namespace qCtagsSenseUtils
{
	typedef QPair<QChar, qCtagsSense::Kind> CharKindPair; // kind char, kind enum
	typedef QList<CharKindPair> CharKindPairList;
	
	QMap<qCtagsSense::Kind, QString> mKindText;
	QMap<QString, CharKindPairList> mLanguageChars; // language, list of pair of char/kind
	QMap<QString, QPixmap> mPixmaps;
};

void qCtagsSenseUtils::initMaps()
{
	if ( mLanguageChars.isEmpty() )
	{
		/*
		CharKindPairList sqlList = CharKindPairList()
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: )
			<< CharKindPair( '', qCtagsSense:: );
		*/
		
		CharKindPairList htmlList = CharKindPairList()
			<< CharKindPair( 'a', qCtagsSense::Anchor )
			<< CharKindPair( 'f', qCtagsSense::JavaScriptFunction );
		
		CharKindPairList javaList = CharKindPairList()
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'e', qCtagsSense::Enum )
			<< CharKindPair( 'f', qCtagsSense::Field )
			<< CharKindPair( 'g', qCtagsSense::EnumType )
			<< CharKindPair( 'i', qCtagsSense::Interface )
			<< CharKindPair( 'l', qCtagsSense::LocalVariable ) // off
			<< CharKindPair( 'm', qCtagsSense::Method )
			<< CharKindPair( 'p', qCtagsSense::Package );
		
		CharKindPairList javascriptList = CharKindPairList()
			<< CharKindPair( 'f', qCtagsSense::Function )
			<< CharKindPair( 'c', qCtagsSense::Class )
			<< CharKindPair( 'm', qCtagsSense::Method )
			<< CharKindPair( 'v', qCtagsSense::GlobalVariable );
		
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
			<< CharKindPair( 'F', qCtagsSense::SingletonMethod );
		
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
			<< CharKindPair( 'F', qCtagsSense::RecordField )
			<< CharKindPair( 'l', qCtagsSense::LocalVariable ) // off
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
			<< CharKindPair( 'g', qCtagsSense::Enum )
			<< CharKindPair( 'l', qCtagsSense::LocalVariable )
			<< CharKindPair( 'm', qCtagsSense::Member )
			<< CharKindPair( 'p', qCtagsSense::Program )
			<< CharKindPair( 'P', qCtagsSense::Prototype )
			<< CharKindPair( 't', qCtagsSense::Task )
			<< CharKindPair( 'T', qCtagsSense::Typedef )
			<< CharKindPair( 'v', qCtagsSense::Variable )
			<< CharKindPair( 'x', qCtagsSense::ExternVariable );
		
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
		
		//mLanguageChars
	
		// kind text
		mKindText[ qCtagsSense::Class ] = QCoreApplication::translate( "qCtagsSense", "Class" );
		mKindText[ qCtagsSense::Macro ] = QCoreApplication::translate( "qCtagsSense", "Macro" );
		mKindText[ qCtagsSense::Enumerator ] = QCoreApplication::translate( "qCtagsSense", "Enumerator" );
		mKindText[ qCtagsSense::Function ] = QCoreApplication::translate( "qCtagsSense", "Function" );
		mKindText[ qCtagsSense::Enum ] = QCoreApplication::translate( "qCtagsSense", "Enum" );
		mKindText[ qCtagsSense::LocalVariable ] = QCoreApplication::translate( "qCtagsSense", "Local Variable" );
		mKindText[ qCtagsSense::Member ] = QCoreApplication::translate( "qCtagsSense", "Member" );
		mKindText[ qCtagsSense::Namespace ] = QCoreApplication::translate( "qCtagsSense", "Namespace" );
		mKindText[ qCtagsSense::Prototype ] = QCoreApplication::translate( "qCtagsSense", "Prototype" );
		mKindText[ qCtagsSense::Structure ] = QCoreApplication::translate( "qCtagsSense", "Structure" );
		mKindText[ qCtagsSense::Typedef ] = QCoreApplication::translate( "qCtagsSense", "Typedef" );
		mKindText[ qCtagsSense::Union ] = QCoreApplication::translate( "qCtagsSense", "Union" );
		mKindText[ qCtagsSense::Variable ] = QCoreApplication::translate( "qCtagsSense", "Variable" );
		mKindText[ qCtagsSense::ExternVariable ] = QCoreApplication::translate( "qCtagsSense", "Extern Variable" );
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
	return mKindText.value( kind );
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
	
	entry->fileName = fileName;
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

QString qCtagsSenseUtils::entryDecorationFilePath( qCtagsSenseEntry* entry )
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
		case qCtagsSense::Enum:
			name = "enum";
			access.clear();
			break;
		case qCtagsSense::LocalVariable:
		case qCtagsSense::Member:
		case qCtagsSense::Variable:
		case qCtagsSense::ExternVariable:
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

QPixmap qCtagsSenseUtils::entryDecoration( qCtagsSenseEntry* entry )
{
	return mPixmaps.value( entryDecorationFilePath( entry ) );
}

QString qCtagsSenseUtils::entryDisplay( qCtagsSenseEntry* entry )
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
			display.append( entry->signature );
			break;
		case qCtagsSense::LocalVariable:
		case qCtagsSense::Member:
		case qCtagsSense::Variable:
		case qCtagsSense::ExternVariable:
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
		case qCtagsSense::Enum:
		case qCtagsSense::Namespace:
		case qCtagsSense::Typedef:
		case qCtagsSense::Unknow:
			break;
	}
	
	return display;
}

QString qCtagsSenseUtils::entryToolTip( qCtagsSenseEntry* entry )
{
	QString tooltip;
	
	tooltip += QString( "<b>*** %1 %2</b><br />(<u>%3:%4</u>)<br />" )
		.arg( QString( "<img src=\"%1\" /> " ).arg( entryDecorationFilePath( entry ) ) )
		.arg( kindText( entry->kind ) )
		.arg( QFileInfo( entry->fileName ).fileName() )
		.arg( entry->lineNumber );
	
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
	
	tooltip += entry->signature;
	
	return tooltip;
}

#warning remove me and use pMonkeyStudio::getFiles

QFileInfoList qCtagsSenseUtils::getFiles( QDir fromDir, const QStringList& filters, bool recursive )
{
	QFileInfoList files;
	foreach ( QFileInfo file, fromDir.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name ) )
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
