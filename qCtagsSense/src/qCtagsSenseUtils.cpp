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
HTML                                                         
	a  named anchors                                         
	f  JavaScript functions                                  
Java                                                         
	c  classes                                               
	e  enum constants                                        
	f  fields                                                
	g  enum types                                            
	i  interfaces                                            
	l  local variables [off]                                 
	m  methods                                               
	p  packages                                              
JavaScript                                                   
	f  functions                                             
	c  classes                                               
	m  methods                                               
	v  global variables                                      
Lisp                                                         
	f  functions                                             
Lua                                                          
	f  functions                                             
Make                                                         
	m  macros                                                
Pascal                                                       
	f  functions                                             
	p  procedures                                            
Perl                                                         
	c  constants                                             
	f  formats                                               
	l  labels                                                
	p  packages                                              
	s  subroutines                                           
	d  subroutine declarations [off]                         
PHP                                                          
	c  classes                                               
	i  interfaces                                            
	d  constant definitions                                  
	f  functions                                             
	v  variables                                             
	j  javascript functions                                  
	j  javascript functions                                  
	j  javascript functions                                  
Python                                                       
	c  classes                                               
	f  functions                                             
	m  class members                                         
REXX                                                         
	s  subroutines                                           
Ruby                                                         
	c  classes                                               
	f  methods                                               
	m  modules                                               
	F  singleton methods                                     
Scheme                                                       
	f  functions                                             
	s  sets                                                  
Sh                                                           
	f  functions                                             
SLang                                                        
	f  functions                                             
	n  namespaces                                            
SML                                                          
	e  exception declarations                                
	f  function definitions                                  
	c  functor definitions                                   
	s  signature declarations                                
	r  structure declarations                                
	t  type definitions                                      
	v  value bindings                                        
SQL                                                          
	c  cursors                                               
	d  prototypes [off]                                      
	f  functions                                             
	F  record fields                                         
	l  local variables [off]                                 
	L  block label                                           
	P  packages                                              
	p  procedures                                            
	r  records [off]                                         
	s  subtypes                                              
	t  tables                                                
	T  triggers                                              
	v  variables                                             
	i  indexes                                               
	e  events                                                
	U  publications                                          
	R  services                                              
	D  domains                                               
	V  views                                                 
	n  synonyms                                              
	x  MobiLink Table Scripts                                
	y  MobiLink Conn Scripts                                 
Tcl                                                          
	c  classes                                               
	m  methods
	p  procedures
Vera
	c  classes
	d  macro definitions
	e  enumerators (values inside an enumeration)
	f  function definitions
	g  enumeration names
	l  local variables [off]
	m  class, struct, and union members
	p  programs
	P  function prototypes [off]
	t  tasks
	T  typedefs
	v  variable definitions
	x  external variable declarations [off]
Verilog
	c  constants (define, parameter, specparam)
	e  events
	f  functions
	m  modules
	n  net data types
	p  ports
	r  register data types
	t  tasks
Vim
	a  autocommand groups
	c  user-defined commands
	f  function definitions
	m  maps
	v  variable definitions
YACC
	l  labels
*/


namespace qCtagsSenseUtils
{
	QMap<qCtagsSense::Kind, QChar> mKindChar;
	QMap<QChar, qCtagsSense::Kind> mCharKind;
	QMap<qCtagsSense::Kind, QString> mKindText;
	
	QMap<QString, QPixmap> mPixmaps;
};

void qCtagsSenseUtils::initMaps()
{
	if ( mKindChar.isEmpty() )
	{
		mKindChar[ qCtagsSense::Class ] = 'c';
		mKindChar[ qCtagsSense::Macro ] = 'd';
		mKindChar[ qCtagsSense::Enumerator ] = 'e';
		mKindChar[ qCtagsSense::Function ] = 'f';
		mKindChar[ qCtagsSense::Enum ] = 'g';
		mKindChar[ qCtagsSense::LocalVariable ] = 'l';
		mKindChar[ qCtagsSense::Member ] = 'm';
		mKindChar[ qCtagsSense::Namespace ] = 'n';
		mKindChar[ qCtagsSense::Prototype ] = 'p';
		mKindChar[ qCtagsSense::Structure ] = 's';
		mKindChar[ qCtagsSense::Typedef ] = 't';
		mKindChar[ qCtagsSense::Union ] = 'u';
		mKindChar[ qCtagsSense::Variable ] = 'v';
		mKindChar[ qCtagsSense::ExternVariable ] = 'x';
		
		mCharKind[ 'c' ] = qCtagsSense::Class;
		mCharKind[ 'd' ] = qCtagsSense::Macro;
		mCharKind[ 'e' ] = qCtagsSense::Enumerator;
		mCharKind[ 'f' ] = qCtagsSense::Function;
		mCharKind[ 'g' ] = qCtagsSense::Enum;
		mCharKind[ 'l' ] = qCtagsSense::LocalVariable;
		mCharKind[ 'm' ] = qCtagsSense::Member;
		mCharKind[ 'n' ] = qCtagsSense::Namespace;
		mCharKind[ 'p' ] = qCtagsSense::Prototype;
		mCharKind[ 's' ] = qCtagsSense::Structure;
		mCharKind[ 't' ] = qCtagsSense::Typedef;
		mCharKind[ 'u' ] = qCtagsSense::Union;
		mCharKind[ 'v' ] = qCtagsSense::Variable;
		mCharKind[ 'x' ] = qCtagsSense::ExternVariable;
		
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

qCtagsSense::Kind qCtagsSenseUtils::kindType( const QChar& c )
{
	if ( mCharKind.contains( c ) )
	{
		return mCharKind[ c ];
	}
	
	return qCtagsSense::Unknow;
}

QChar qCtagsSenseUtils::kindChar( qCtagsSense::Kind kind )
{
	return mKindChar.value( kind );
}

QString qCtagsSenseUtils::kindText( qCtagsSense::Kind kind )
{
	return mKindText.value( kind );
}

QString qCtagsSenseUtils::kindText( const QChar& c )
{
	return kindText( kindType( c ) );
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
