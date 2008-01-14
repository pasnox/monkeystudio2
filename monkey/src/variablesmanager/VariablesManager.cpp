/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : VariablesManager.cpp
** Date      : 2008-01-14T00:37:18
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
#include "VariablesManager.h"

#include <QStringList>
#include <QList>
#include <QRegExp>
#include <QDateTime>

VariablesManager::VariablesManager( QObject* o )
	: QObject( o )
{}

QString VariablesManager::getVariable (QString name, Dictionary locals)
{
    QString result = QString::null;
    // monkeystudio_version
    if ( name == "editor_version" )
        result = PROGRAM_VERSION;
    // monkeystudio_version_string
    else if ( name == "editor_version_string" )
        result = QString( "%1 v%2" ).arg( PROGRAM_NAME ).arg( PROGRAM_VERSION );
    else if ( name == "date" )
        result = QDateTime::currentDateTime().toString( Qt::ISODate );
    if (!result.isEmpty())
        return result;
    if ( globals.contains(name))
        return globals[name];
    if (locals.contains(name))
        return locals[name];
    return QString::null;
}

bool VariablesManager::isSet (QString name, Dictionary& locals)
{
    if (    name == "editor_version" ||
            name == "editor_version_string" ||
            name == "date" )
        return true;
    return (globals.contains(name) || locals.contains(name));
}

#include <QDebug>
QString VariablesManager::replaceAllVariables (QString text, Dictionary locals)
{
    int p = 0;
    QString s;
    QRegExp rex( "(\\$[^$\\n]+\\$)" );
    // search and interpret values
    QList<QString> findedVariables;
    while ( ( p = rex.indexIn( text, p ) ) != -1 )
    {
        // got keyword
        s = rex.capturedTexts().value( 1 );
        findedVariables.append (s);
        p += rex.matchedLength();
		qWarning() << "var: " << s;
    }
    // replace occurences
    foreach ( QString s, findedVariables )
	{
		QString fuckDollar = QString(s).remove(s.size()-1,1).remove(0,1);
		bool toup = false;
		bool tolow = false;
		if (fuckDollar.endsWith (".upper"))
		{
			toup = true;
			fuckDollar.remove (".upper");
		}
		else if (fuckDollar.endsWith (".lower"))
		{
			tolow = true;
			fuckDollar.remove (".lower");
		}
		QString replaceWith = getVariable(fuckDollar,locals);
		if (toup)
			replaceWith = replaceWith.toUpper();
		else if (tolow)
			replaceWith = replaceWith.toLower();
        text.replace( s, replaceWith);
		text.replace( "\\n", "\n");
	}
    // return value
    return text;
}
