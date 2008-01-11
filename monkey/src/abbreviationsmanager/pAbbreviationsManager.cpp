/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox <pasnox@gmail.com>
** Project   : pAbbreviationsManager
** FileName  : pAbbreviationsManager.cpp
** Date      : sam. août 11 19:31:08 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pAbbreviationsManager.h"
#include "pSettings.h"
#include "pEditor.h"
#include "MonkeyCore.h"

#include "qscilexer.h"

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


