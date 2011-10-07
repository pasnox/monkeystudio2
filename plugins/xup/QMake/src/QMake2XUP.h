/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
#ifndef QMAKE2XUP_H
#define QMAKE2XUP_H

#include <QDomDocument>

#include <pMonkeyStudio.h>

namespace QMake2XUP
{
    QString convertFromPro( const QString& fileName, const QString& codec );
    QString convertToPro( const QDomDocument& project );
    
    QString escape( const QString& string );

    QString convertNodeToPro( const QDomNode& node, int weight = 0, bool multiline = false, bool nested = false, const QString& EOL = pMonkeyStudio::getEol() );
    QString tabbedString( int weight, const QString& string, const QString& eol = QString::null );
    QString nodeAttribute( const QDomNode& node, const QString& attribute, const QString& defaultValue = QString::null );
    bool isMultiline( const QDomNode& node );
    bool isNested( const QDomNode& node );
    bool isBlock( const QDomNode& node );
    bool isProject( const QDomNode& node );
    bool isComment( const QDomNode& node );
    bool isEmptyLine( const QDomNode& node );
    bool isVariable( const QDomNode& node );
    bool isValue( const QDomNode& node );
    bool isLastValue( const QDomNode& node );
};

#endif // QMAKE2XUP_H
