#include "qCtagsSenseGlobals.h"
#include "qCtagsSenseEntry.h"

#include <ctags.h>
#include <pIconManager.h>

#include <QHash>
#include <QDebug>

// Fileds
/*
	flags: "afmikKlnsStz") [fks]. 
*/

/*	ctags --list-maps
Ant      *.build.xml
Asm      *.asm *.ASM *.s *.S *.A51 *.29[kK] *.[68][68][kKsSxX] *.[xX][68][68]
Asp      *.asp *.asa
Awk      *.awk *.gawk *.mawk
Basic    *.bas *.bi *.bb *.pb
BETA     *.bet
C        *.c
C++      *.c++ *.cc *.cp *.cpp *.cxx *.h *.h++ *.hh *.hp *.hpp *.hxx *.C *.H
C#       *.cs
Cobol    *.cbl *.cob *.CBL *.COB
DosBatch *.bat *.cmd
Eiffel   *.e
Erlang   *.erl *.ERL *.hrl *.HRL
Flex     *.as *.mxml
Fortran  *.f *.for *.ftn *.f77 *.f90 *.f95 *.F *.FOR *.FTN *.F77 *.F90 *.F95
HTML     *.htm *.html
Java     *.java
JavaScript *.js
Lisp     *.cl *.clisp *.el *.l *.lisp *.lsp
Lua      *.lua
Make     *.mak *.mk [Mm]akefile GNUmakefile
MatLab   *.m
OCaml    *.ml *.mli
Pascal   *.p *.pas
Perl     *.pl *.pm *.plx *.perl
PHP      *.php *.php3 *.phtml
Python   *.py *.pyx *.pxd *.pxi *.scons
REXX     *.cmd *.rexx *.rx
Ruby     *.rb *.ruby
Scheme   *.SCM *.SM *.sch *.scheme *.scm *.sm
Sh       *.sh *.SH *.bsh *.bash *.ksh *.zsh
SLang    *.sl
SML      *.sml *.sig
SQL      *.sql
Tcl      *.tcl *.tk *.wish *.itcl
Tex      *.tex
Vera     *.vr *.vri *.vrh
Verilog  *.v
VHDL     *.vhdl *.vhd
Vim      *.vim
YACC     *.y
*/

namespace qCtagsSense
{
	typedef QHash<int, QString> IntStringHash; // kind string, kind enum
	
	QHash<qCtagsSense::Language, QString> mLanguagesName;
	QHash<qCtagsSense::Language, IntStringHash> mKindsName;
	QHash<qCtagsSense::Language, IntStringHash> mKindsDescription;
}

void qCtagsSense::initialize()
{
	for ( int i = 0 ;  i < languageCount() ;  ++i ) {
		const langType type = langType( i );
		const parserDefinition* language = parser( type );
		const qCtagsSense::Language lng = qCtagsSense::Language( type );
		
		mLanguagesName[ lng ] = language->name;
		
		for ( uint j = 0; j < language->kindCount; j++ ) {
			const sKindOption& kind = language->kinds[ j ];
			mKindsName[ lng ][ kind.letter ] = kind.name;
			mKindsDescription[ lng ][ kind.letter ] = kind.description;
		}
	}
	
	qWarning() << mLanguagesName;
	qWarning() << mKindsName;
	qWarning() << mKindsDescription;
}

QString qCtagsSense::languageName( qCtagsSense::Language language )
{
	return mLanguagesName.value( language );
}

qCtagsSense::Language qCtagsSense::language( const QString& language )
{
	return mLanguagesName.key( language, qCtagsSense::UnknowLanguage );
}

QString qCtagsSense::kindName( qCtagsSense::Language language, int kind )
{
	return mKindsName.value( language ).value( kind );
}

int qCtagsSense::kindLetter( qCtagsSense::Language language, const QString& kind )
{
	return mKindsName.value( language ).key( kind, -1 );
}

QString qCtagsSense::kindDescription( qCtagsSense::Language language, int kind )
{
	return mKindsDescription.value( language ).value( kind );
}



bool qCtagsSense::caseInsensitiveFilePathLessThan( const QString& left, const QString& right )
{
	const int result = QString::compare( QFileInfo( left ).fileName(), QFileInfo( right ).fileName(), Qt::CaseInsensitive );
	
	if ( result == 0 ) {
		return true;
	}
	else if ( result < 0 ) {
		return true;
	}
	else {
		return false;
	}
}

QFileInfoList qCtagsSense::getFiles( QDir& fromDir, const QStringList& filters, bool recursive )
{
	const QDir::Filters dirFilters = QDir::AllEntries | QDir::AllDirs | QDir::NoDotAndDotDot;
	const QDir::SortFlags dirSortFlags = QDir::DirsFirst | QDir::Name;
	QFileInfoList files;
	
	foreach ( const QFileInfo& file, fromDir.entryInfoList( dirFilters, dirSortFlags ) ) {
		if ( file.isFile() && ( filters.isEmpty() || QDir::match( filters, file.fileName() ) ) ) {
			files << file;
		}
		else if ( file.isDir() && recursive ) {
			fromDir.cd( file.filePath() );
			files << getFiles( fromDir, filters, recursive );
			fromDir.cdUp();
		}
	}
	
	return files;
}



QString qCtagsSense::entryDecorationFileName( const qCtagsSense::Entry* entry )
{
	const QString access = entry->access;
	const QString name = kindName( entry->language, entry->kind );
	return QString( "%1_%2.png" ).arg( access ).arg( name );
}

QPixmap qCtagsSense::entryDecorationPixmap( const qCtagsSense::Entry* entry )
{
	return pIconManager::pixmap( entryDecorationFileName( entry ), ":/icons" );
}

QIcon qCtagsSense::entryDecorationIcon( const qCtagsSense::Entry* entry )
{
	return pIconManager::icon( entryDecorationFileName( entry ), ":/icons" );
}

QString qCtagsSense::entryDisplay( const qCtagsSense::Entry* entry )
{
	return entry->name;
	/*
	qCtagsSense::Entry* parent = entry->parent;
	qCtagsSense::Entry* granParent = parent ? parent->parent : 0;
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
		case qCtagsSense::Entry::Function:
		case qCtagsSense::Entry::Prototype:
			//display.append( entry->signature );
			break;
		case qCtagsSense::Entry::VariableLocal:
		case qCtagsSense::Entry::Member:
		case qCtagsSense::Entry::Variable:
		case qCtagsSense::Entry::VariableGlobal:
			if ( !entry->typeRef.first.isEmpty() )
			{
				display.append( QString( ": %1" ).arg( entry->typeRef.first ) );
			}
			break;
		case qCtagsSense::Entry::Class:
		case qCtagsSense::Entry::Structure:
		case qCtagsSense::Entry::Union:
		case qCtagsSense::Entry::Macro:
		case qCtagsSense::Entry::Enumerator:
		case qCtagsSense::Entry::Enumeration:
		case qCtagsSense::Entry::Namespace:
		case qCtagsSense::Entry::Typedef:
		case qCtagsSense::Entry::UnknowKind:
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
	*/
}

QString qCtagsSense::entryToolTip( const qCtagsSense::Entry* entry )
{
	QString tooltip;
	
	tooltip += QString( "<b>*** %1 %2</b><br />(<u>%3:%4</u>)<br />" )
		.arg( QString( "<img src=\"%1\" /> " ).arg( pIconManager::filePath( entryDecorationFileName( entry ), ":/icons" ) ) )
		.arg( kindDescription( entry->language, entry->kind ) )
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
