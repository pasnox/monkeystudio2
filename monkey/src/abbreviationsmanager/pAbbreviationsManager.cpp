/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pAbbreviationsManager.cpp
** Date      : 2008-01-14T00:36:48
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
#include "pAbbreviationsManager.h"
#include "../qscintillamanager/pEditor.h"
#include "../coremanager/MonkeyCore.h"
#include "../settingsmanager/Settings.h"
#include "../shellmanager/MkSShellInterpreter.h"
#include "../queuedstatusbar/QueuedStatusBar.h"

#include <qscintilla.h>

#include <QFile>
#include <QDir>
#include <QDebug>

pAbbreviationsManager::pAbbreviationsManager( QObject* parent )
	: QObject( parent )
{
}

void pAbbreviationsManager::initialize()
{
	// register command
	QString help = MkSShellInterpreter::tr
	(
		"This command manage the abbreviations, usage:\n"
		"\tabbreviation add [macro] [description] [language] [code]\n"
		"\tabbreviation del [macro] [language]\n"
		"\tabbreviation show [macro] [language]\n"
		"\tabbreviation list [language]\n"
		"\tabbreviation clear"
	);
	
	MonkeyCore::interpreter()->addCommandImplementation( "abbreviation", pAbbreviationsManager::commandInterpreter, help );
	
	// search abbreviations script
	QString fn = QString( "%1/abbreviations.mks" ).arg( MonkeyCore::settings()->storageToString( Settings::SP_SCRIPTS ) );
	QString fp = MonkeyCore::settings()->homeFilePath( fn );
	
	if ( !QFile::exists( fp ) )
	{
		foreach ( const QString& path, MonkeyCore::settings()->storagePaths( Settings::SP_SCRIPTS ) )
		{
			fp = QDir( path ).filePath( "abbreviations.mks" );
			
			if ( !QFile::exists( fp ) )
			{
				fp.clear();
			}
		}
	}
	
	// execute script
	if ( !fp.isEmpty() )
	{
		if ( !MonkeyCore::interpreter()->loadScript( fp ) )
		{
			MonkeyCore::statusBar()->appendMessage( tr( "An error occur while loading script: %1" ).arg( fp ) );
		}
	}
}

QString pAbbreviationsManager::commandInterpreter( const QString& command, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter )
{
	Q_UNUSED( command );
	Q_UNUSED( interpreter );
	const QStringList allowedOperations = QStringList( "add" ) << "del" << "show" << "list" << "clear";
	
	if ( result )
	{
		*result = 0;
	}
	
	if ( arguments.isEmpty() )
	{
		if ( result )
		{
			*result = MkSShellInterpreter::InvalidCommand;
		}
		
		return MkSShellInterpreter::tr( "Operation not defined. Available operations are: %1." ).arg( allowedOperations.join( ", " ) );
	}
	
	const QString operation = arguments.first();
	
	if ( !allowedOperations.contains( operation ) )
	{
		if ( result )
		{
			*result = MkSShellInterpreter::InvalidCommand;
		}
		
		return MkSShellInterpreter::tr( "Unknown operation: '%1'." ).arg( operation );
	}
	
	if ( operation == "add" )
	{
		if ( arguments.size() != 5 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return MkSShellInterpreter::tr( "'add' operation take 4 arguments, %1 given." ).arg( arguments.count() -1 );
		}
		
		const QString macro = arguments.at( 1 );
		const QString description = arguments.at( 2 );
		const QString language = arguments.at( 3 );
		const QString snippet = QString( arguments.at( 4 ) ).replace( "\\n", "\n" );
		
		MonkeyCore::abbreviationsManager()->add( pAbbreviation( macro, description, language, snippet ) );
	}
	
	if ( operation == "del" )
	{
		if ( arguments.size() != 3 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return MkSShellInterpreter::tr( "'del' operation take 2 arguments, %1 given." ).arg( arguments.count() -1 );
		}
		
		const QString macro = arguments.at( 1 );
		const QString language = arguments.at( 2 );
		
		MonkeyCore::abbreviationsManager()->remove( macro, language );
	}
	
	if ( operation == "show" )
	{
		if ( arguments.size() != 3 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return MkSShellInterpreter::tr( "'show' operation take 2 arguments, %1 given." ).arg( arguments.count() -1 );
		}
		
		const QString macro = arguments.at( 1 );
		const QString language = arguments.at( 2 );
		const pAbbreviation abbreviation = MonkeyCore::abbreviationsManager()->abbreviation( macro, language );
		
		if ( abbreviation.Macro.isEmpty() )
		{
			return MkSShellInterpreter::tr( "Macro not found." );
		}
		
		QStringList output;
		
		output << QString( "%1:" ).arg( abbreviation.Description );
		output << abbreviation.Snippet;
		
		return output.join( "\n" );
	}
	
	if ( operation == "list" )
	{
		if ( arguments.size() != 2 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return MkSShellInterpreter::tr( "'list' operation take 1 argument, %1 given." ).arg( arguments.count() -1 );
		}
		
		const QString language = arguments.at( 1 );
		QStringList output;
		
		foreach ( const pAbbreviation& abbreviation, MonkeyCore::abbreviationsManager()->abbreviations() )
		{
			if ( abbreviation.Language == language )
			{
				output << abbreviation.Macro;
			}
		}
		
		if ( !output.isEmpty() )
		{
			output.prepend( MkSShellInterpreter::tr( "Found macros:" ) );
		}
		else
		{
			output << MkSShellInterpreter::tr( "No macros found." );
		}
		
		return output.join( "\n" );
	}
	
	if ( operation == "clear" )
	{
		if ( arguments.size() != 1 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return MkSShellInterpreter::tr( "'clear' operation take no arguments, %1 given." ).arg( arguments.count() -1 );
		}
		
		MonkeyCore::abbreviationsManager()->clear();
	}
	
	return QString::null;
}

void pAbbreviationsManager::clear()
{
	mAbbreviations.clear();
}

void pAbbreviationsManager::add( const pAbbreviation& abbreviation )
{	
	foreach ( const pAbbreviation& abbr, mAbbreviations )
	{
		if ( abbr.Macro == abbreviation.Macro && abbr.Language == abbreviation.Language )
		{
			mAbbreviations.removeOne( abbr );
			break;
		}
	}
	
	mAbbreviations << abbreviation;
}

void pAbbreviationsManager::add( const pAbbreviationList& abbreviations )
{
	foreach ( const pAbbreviation& oAbbr, mAbbreviations )
	{
		foreach ( const pAbbreviation& nAbbr, abbreviations )
		{
			if ( oAbbr.Macro == nAbbr.Macro && oAbbr.Language == nAbbr.Language )
			{
				mAbbreviations.removeOne( oAbbr );
				break;
			}
		}
	}
	
	mAbbreviations << abbreviations;
}

void pAbbreviationsManager::set( const pAbbreviationList& abbreviations )
{
	mAbbreviations = abbreviations;
}

void pAbbreviationsManager::remove( const pAbbreviation& abbreviation )
{
	mAbbreviations.removeOne( abbreviation );
}

void pAbbreviationsManager::remove( const pAbbreviationList& abbreviations )
{
	foreach ( const pAbbreviation& abbreviation, abbreviations )
	{
		mAbbreviations.removeOne( abbreviation );
	}
}

void pAbbreviationsManager::remove( const QString& macro, const QString& language )
{
	foreach ( const pAbbreviation& abbreviation, mAbbreviations )
	{
		if ( abbreviation.Macro == macro && abbreviation.Language == language )
		{
			mAbbreviations.removeOne( abbreviation );
		}
	}
}

const pAbbreviationList& pAbbreviationsManager::abbreviations() const
{
	return mAbbreviations;
}

pAbbreviation pAbbreviationsManager::abbreviation( const QString& macro, const QString& language ) const
{
	foreach ( const pAbbreviation& abbreviation, mAbbreviations )
	{
		if ( abbreviation.Macro == macro && abbreviation.Language == language )
		{
			return abbreviation;
		}
	}
	
	return pAbbreviation();
}

void pAbbreviationsManager::expandMacro( pEditor* editor )
{
	// need valid editor & lexer
	if ( !editor || !editor->lexer() )
	{
		return;
	}
	
	// get current cursor position
	const QPoint pos = editor->cursorPosition();
	
	// get macro
	QString macro = editor->text( pos.y() ).left( pos.x() );
	
	// calculate the index
	int i = macro.lastIndexOf( " " );
	if ( i == -1 )
	{
		i = macro.lastIndexOf( "\t" );
	}
	
	// get clean macro
	macro = macro.mid( i ).trimmed();
	
	if ( macro.isEmpty() )
	{
		MonkeyCore::statusBar()->appendMessage( tr( "Empty macro !" ), 1000 );
		return;
	}
	
	// get language
	const QString lng = editor->lexer()->language();
	
	// look for abbreviation and lexer to replace
	foreach ( const pAbbreviation& abbreviation, mAbbreviations )
	{
		// if template is found for language
		if ( abbreviation.Language == lng && abbreviation.Macro == macro )
		{
			// begin undo
			editor->beginUndoAction();
			
			// select macro in document
			editor->setSelection( pos.y(), i +1, pos.y(), i +1 +macro.length() );
			
			// remove macro from document
			editor->removeSelectedText();
			
			// for calculate final cursor position if it found a |
			QPoint op;
			int k;
			
			// get code lines
			QStringList lines = abbreviation.Snippet.split( "\n" );
			int j = 0;
			
			// iterating code lines
			foreach ( QString line, lines )
			{
				// looking for cursor position
				k = line.indexOf( "|" );
				
				// calculate cursor position
				if ( k != -1 )
				{
					op.ry() = pos.y() +j;
					op.rx() = k +i +1;
					line.replace( "|", "" );
				}
				
				// if no last line
				if ( j < lines.count() -1 )
				{
					// insert code line and an end of line
					editor->insert( line +"\n" );
					// set cursor on next line
					editor->setCursorPosition( pos.y() +j +1, 0 );
				}
				
				// insert codel ine
				else
				{
					editor->insert( line );
				}
				
				// process indentation for code line if line is not first one
				if ( j > 0 )
				{
					editor->setIndentation( pos.y() +j, editor->indentation( pos.y() ) +editor->indentation( pos.y() +j ) );
				}
				
				// increment j for calculate correct line
				j++;
			}
			
			// set new cursor position is needed
			if ( !op.isNull() )
			{
				editor->setCursorPosition( op.y(), op.x() );
			}
			
			// end undo
			editor->endUndoAction();
			
			// hide autocompletion combobox
			editor->cancelList();
			
			// finish
			return;
		}
	}
	
	MonkeyCore::statusBar()->appendMessage( tr( "No '%1' macro found for '%2' language" ).arg( macro ).arg( lng ) );
}
