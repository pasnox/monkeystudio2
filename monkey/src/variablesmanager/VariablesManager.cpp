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
#include "VariablesManager.h"

#include <QStringList>
#include <QList>
#include <QRegExp>
#include <QDateTime>

/*!
	Class constructor
	\param o Parent object. Passing to QObject constructor
*/
VariablesManager::VariablesManager( QObject* o )
	: QObject( o )
{}

/*!
	Get value of variable
	\param name Variable name
	\param locals Local dictionary of variables
	\return Value of variable. Empty string, if variable is unknown
*/
QString VariablesManager::getVariable( QString name, Dictionary locals )
{
	QString result = QString::null;
	// monkeystudio_version
	if ( name == "editor_version" )
		result = PACKAGE_VERSION;
	// monkeystudio_version_string
	else if ( name == "editor_version_string" )
		result = QString( "%1 v%2" ).arg( PACKAGE_NAME ).arg( PACKAGE_VERSION );
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

/*!
	Check, if variable is set localy or globaly
	\param name Name of variable
	\param locals Local dictionary
	\retval true Variable is set
	\retval false Variable is not set
*/
bool VariablesManager::isSet (QString name, Dictionary& locals)
{
	if (    name == "editor_version" ||
			name == "editor_version_string" ||
			name == "date" )
		return true;
	return (globals.contains(name) || locals.contains(name));
}

/*!
	Replace all variables in the text by it's values
	\param text Text for processing
	\param locals Local dictionary of variables
	return New string
*/
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
