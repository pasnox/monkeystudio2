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

#include <qscintilla.h>

const QList<pAbbreviation> pAbbreviationsManager::defaultAbbreviations()
{
	return QList<pAbbreviation>()
	// C++
	<< pAbbreviation( "classd", "class declaration", "C++", "class |\n{\npublic:\n};" )
	<< pAbbreviation( "forb", "for statement", "C++", "for( |; ; )\n{\n}" )
	<< pAbbreviation( "ifb", "if statement", "C++", "if( | )\n{\n}" )
	<< pAbbreviation( "ife", "if else statement", "C++", "if( | )\n{\n}\nelse\n{\n}" )
	<< pAbbreviation( "pr", "private", "C++", "private|" )
	<< pAbbreviation( "pro", "protected", "C++", "protected|" )
	<< pAbbreviation( "pu", "public", "C++", "public|" )
	<< pAbbreviation( "structd", "struct declaration", "C++", "struct |\n{\n};" )
	<< pAbbreviation( "switchb", "switch statement", "C++", "switch( | )\n{\n}" )
	<< pAbbreviation( "whileb", "while statement", "C++", "while( | )\n{\n}" );
}

const QList<pAbbreviation> pAbbreviationsManager::availableAbbreviations()
{
	// get settings
	pSettings* s = MonkeyCore::settings();
	// values list
	QList<pAbbreviation> mAbbreviations;
	// read abbreviations from settings
	int size = s->beginReadArray( "Abbreviations" );
	for ( int i = 0; i < size; i++ )
	{
		s->setArrayIndex( i );
		mAbbreviations << pAbbreviation( s->value( "Template" ).toString(), s->value( "Description" ).toString(), s->value( "Language" ).toString(), s->value( "Code" ).toString() );
	}
	s->endArray();
	// get default abbreviations if needed
	if ( mAbbreviations.isEmpty() )
		mAbbreviations << defaultAbbreviations();
	// return abbreviations
	return mAbbreviations;
}

void pAbbreviationsManager::expandAbbreviation( pEditor* e )
{
	if ( !e || !e->lexer() )
		return;
	// get current cursor position
	const QPoint p = e->cursorPosition();
	// get word template
	QString t = e->text( p.y() ).left( p.x() );
	// calculate the index
	int i = t.lastIndexOf( " " );
	if ( i == -1 )
		i = t.lastIndexOf( "\t" );
	// get true word template
	t = t.mid( i ).trimmed();
	// get language
	const QString lng = e->lexer()->language();
	// look for abbreviation and lexer to replace
	foreach ( pAbbreviation a, availableAbbreviations() )
	{
		// if template is found for language
		if ( a.Language == lng && a.Template == t )
		{
			// select word template from document
			e->setSelection( p.y(), i +1, p.y(), i +1 +t.length() );
			// remove word template from document
			e->removeSelectedText();
			// for calculate final cursor position if it found a |
			QPoint op;
			int k;
			// get code lines
			QStringList l = a.Code.split( "\n" );
			int j = 0;
			// iterating code lines
			foreach ( QString s, l )
			{
				// looking for cursor position
				k = s.indexOf( "|" );
				// calculate cursor position
				if ( k != -1 )
				{
					op.ry() = p.y() +j;
					op.rx() = k +i +1;
					s.replace( "|", "" );
				}
				// if no last line
				if ( j < l.count() -1 )
				{
					// insert code line and an end of line
					e->insert( s +"\n" );
					// set cursor on next line
					e->setCursorPosition( p.y() +j +1, 0 );
				}
				// insert codel ine
				else
					e->insert( s );
				// process indentation for code line if line is not first one
				if ( j > 0 )
					e->setIndentation( p.y() +j, e->indentation( p.y() ) +e->indentation( p.y() +j ) );
				// increment j for calculate correct line
				j++;
			}
			// set new cursor position is needed
			if ( !op.isNull() )
				e->setCursorPosition( op.y(), op.x() );
			// finish
			return;
		}
	}
}


