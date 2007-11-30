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
    QRegExp rex( "(\\$[^$]+\\$)" );
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
