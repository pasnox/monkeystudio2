/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : QMakeParser.h
** Date      : 2008-01-14T00:54:11
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
#ifndef QMAKEPARSER_H
#define QMAKEPARSER_H

#include <QString>

class QMakeItem;

class QMakeParser
{
public:
	QMakeParser( const QString&, QMakeItem* );
	~QMakeParser();
	
	bool isOpen() const;
	
protected:
	bool loadFile( const QString&, QMakeItem* );
	int parseBuffer( int, QMakeItem* );
	
	QMakeItem* processNested( const QString&, QMakeItem* );
	QMakeItem* processValues( const QString&, QMakeItem* );
	QMakeItem* addScope( const QString&, const QString&, bool, QMakeItem* );
	QMakeItem* addVariable( const QString&, const QString&, QMakeItem* );
	QMakeItem* addFunction( const QString&, const QString&, QMakeItem* );
	QMakeItem* addValue( const QString&, QMakeItem* );
	QMakeItem* addComment( const QString&, QMakeItem* );
	QMakeItem* addEmpty( QMakeItem* );
	
	bool mIsOpen;
	QMakeItem* mRoot;

};

#endif // QMAKEPARSER_H
