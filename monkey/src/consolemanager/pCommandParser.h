/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Andrei Kopats aka hlamer <hlamer@tut.by>
** Project   : Monkey Studio IDE
** FileName  : pCommandParser.h
** Date      : 2008-01-14T00:36:50
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
/*!
	\file pCommandParser.h
	\date 2008-01-14T00:36:50
	\author Andrei Kopats
	\brief Header for pCommandParser class
*/

#ifndef PCOMMANDPARSER_H
#define PCOMMANDPARSER_H

#include <fresh.h>

#include "pConsoleManager.h"

#include <QObject>

class XUPItem;

/*!
	Parent class for parsers of console output.
	
	Inherit this class for create own parser. There is some description of it 
	in a wiki documentation of project
*/
class Q_MONKEY_EXPORT pCommandParser : public QObject
{
	Q_OBJECT

protected:

/*!
	Structure, which containing regular expression for searching some phrase
	in a console output of programm, and also information, how it should be 
	used.
	This structures is using in the default implementation of parsers, but, 
	not nessesery uses by all
	
	FileName, col, row, Text, FullText fields should contain text, which
	including %d patterns (where d is any number)
	%d patterns will be replaced with submatching of regular expression, when
	parsing result will be generated
*/
struct Pattern
{
	QRegExp regExp;
	QString FileName;
	QString col;
	QString row;
	pConsoleManager::StepType Type;
	QString Text;
	QString FullText;
};

	QString mName;
	QList <Pattern> patterns;
	
	QString replaceWithMatch(QRegExp&, QString);
public:
	pCommandParser (QObject* p) :QObject (p) {};
	virtual ~pCommandParser();

	virtual QString name() const;

public :
	/* returnting count of lines, that was successfully parsed 
		(and should be removed from buffer)
		*/
		virtual int processParsing(QString*);

signals:
	void newStepAvailable( const pConsoleManager::Step& );

};

typedef QList<pCommandParser*> pCommandParserList;

Q_DECLARE_METATYPE( pCommandParserList );

#endif // PCOMMANDPARSER_H
