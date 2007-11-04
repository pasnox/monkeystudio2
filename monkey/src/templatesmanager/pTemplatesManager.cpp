/*****************************************************
* AUTHOR   : Nox P@sNox
* PROJECT  : 
* FILENAME : pTemplatesManager.cpp
* DATE     : 2007/07/28
* TIME     : 15:17
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#include "pTemplatesManager.h"
#include "pMonkeyStudio.h"
#include "pSettings.h"
#include "ProjectItem.h"

#include <QDir>
#include <QHash>
#include <QDateTime>
#include <QApplication>

const QList<pTemplate> pTemplatesManager::defaultTemplates()
{
	return QList<pTemplate>()
	// C++ Files
	<< pTemplate( "C++", pTemplate::ttFiles, "Header", "Simple Header", ":/templates/icons/templates/misc.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Files/header.h" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "Source", "Simple Source", ":/templates/icons/templates/misc.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Files/source.cpp" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "Main", "Simple Main", ":/templates/icons/templates/console.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Files/main.cpp" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "Qt Console Main", "Simple Qt Console Main", ":/templates/icons/templates/qt_console.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Files/qt_console_main.cpp" ) ) )
	<< pTemplate( "C++", pTemplate::ttFiles, "Qt GUI Main", "Simple Qt GUI Main", ":/templates/icons/templates/qt_gui.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Files/qt_gui_main.cpp" ) ) )
	// C++ Extras
	<< pTemplate( "C++", pTemplate::ttExtras, "Namespace", "Simple Namespace", ":/templates/icons/templates/namespace.png", unTokenize( QStringList() << "$TEMPLATE_PATH$/C++/Extras/namespace.h" << "$TEMPLATE_PATH$/C++/Extras/namespace.cpp" ) )
	<< pTemplate( "C++", pTemplate::ttExtras, "QObject", "Simple QObject Class", ":/templates/icons/templates/qobject.png", unTokenize( QStringList() << "$TEMPLATE_PATH$/C++/Files/qobject_header.h" << "$TEMPLATE_PATH$/C++/Files/qobject_source.cpp" ) )
	<< pTemplate( "C++", pTemplate::ttExtras, "QWidget", "Simple QWidget Class", ":/templates/icons/templates/qwidget.png", unTokenize( QStringList() << "$TEMPLATE_PATH$/C++/Files/qwidget_header.h" << "$TEMPLATE_PATH$/C++/Files/qwidget_source.cpp" ) ) 
	<< pTemplate( "C++", pTemplate::ttExtras, "QDialog", "Simple QDialog Class", ":/templates/icons/templates/qdialog.png", unTokenize( QStringList() << "$TEMPLATE_PATH$/C++/Files/qdialog_header.h" << "$TEMPLATE_PATH$/C++/Files/qdialog_source.cpp" ) ) 
	<< pTemplate( "C++", pTemplate::ttExtras, "QMainWindow", "Simple QMainWindow Class", ":/templates/icons/templates/qmainwindow.png", unTokenize( QStringList() << "$TEMPLATE_PATH$/C++/Files/qmainwindow_header.h" << "$TEMPLATE_PATH$/C++/Files/qmainwindow_source.cpp" ) ) 
	// C++ Projects
    << pTemplate( "C++", pTemplate::ttProjects, "Monkey native project", "Use it for all non-qt projects", ":/templates/icons/templates/misc.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Projects/project.monkey" ) ) )
	<< pTemplate( "C++", pTemplate::ttProjects, "Qt Console", "Simple Qt Console Application", ":/templates/icons/templates/qt_console.png", unTokenize( QStringList() << "$TEMPLATE_PATH$/C++/Projects/qt_console.pro" << "$TEMPLATE_PATH$/C++/Files/qt_console_main.cpp" ) ) 
	<< pTemplate( "C++", pTemplate::ttProjects, "Qt GUI", "Simple Qt GUI Application", ":/templates/icons/templates/qt_gui.png", unTokenize( QStringList() << "$TEMPLATE_PATH$/C++/Projects/qt_gui.pro" << "$TEMPLATE_PATH$/C++/Files/qt_gui_main.cpp" ) ) 
	<< pTemplate( "C++", pTemplate::ttProjects, "Qt Dll", "Simple Qt Dll", ":/templates/icons/templates/qt_dll.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Projects/qt_dll.pro" ) ) ) 
	<< pTemplate( "C++", pTemplate::ttProjects, "Qt Static Lib", "Simple Qt Static Lib", ":/templates/icons/templates/qt_static_lib.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Projects/qt_gui.pro" ) ) )	
	<< pTemplate( "C++", pTemplate::ttProjects, "QMake C++ App", "QMake Based C++ Application", ":/templates/icons/templates/qt_console.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Projects/qt_cpp_app.pro" ) ) )
	<< pTemplate( "C++", pTemplate::ttProjects, "QMake C++ Dll", "QMake Based C++ Dll", ":/templates/icons/templates/qt_dll.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Projects/qt_cpp_dll.pro" ) ) )
	<< pTemplate( "C++", pTemplate::ttProjects, "QMake C++ Static Lib", "QMake Based C++ Static Lib", ":/templates/icons/templates/qt_static_lib.png", unTokenize( QStringList( "$TEMPLATE_PATH$/C++/Projects/qt_cpp_static_lib.pro" ) ) );
}

const QList<pTemplate> pTemplatesManager::availableTemplates()
{
	// get settings
	pSettings* s = pSettings::instance();
	// values list
	QList<pTemplate> mTemplates;
	// read settings
	int size = s->beginReadArray( "Templates" );
	for ( int i = 0; i < size; i++ )
	{
		s->setArrayIndex( i );
		mTemplates << pTemplate( s->value( "Language" ).toString(), (pTemplate::TemplateType)s->value( "Type" ).toInt(), s->value( "Name" ).toString(), s->value( "Description" ).toString(), s->value( "Icon" ).toString(), s->value( "FileNames" ).toStringList() );
	}
	s->endArray();
	// untokenize, need doing it separatly because qsettings is not thread safe
	for ( int i = 0; i < mTemplates.count(); i++ )
	{
		pTemplate& t = mTemplates[i];
		t.Icon = unTokenize( t.Icon );
		t.FileNames = unTokenize( t.FileNames );
	}
	// get default abbreviations if needed
	if ( mTemplates.isEmpty() )
		mTemplates << defaultTemplates();
	// return list
	return mTemplates;
}

void pTemplatesManager::setTemplatesPath( const QString& s )
{
	pSettings::instance()->setValue( "Templates/DefaultDirectory", pMonkeyStudio::tokenizeHome( s ) );
}

const QString pTemplatesManager::templatesPath()
{
	return pMonkeyStudio::unTokenizeHome( pSettings::instance()->value( "Templates/DefaultDirectory", 
		QApplication::applicationDirPath ()+"/../templates").toString() );
}

void pTemplatesManager::setTemplatesHeader( const QString& s )
{
	pSettings::instance()->setValue( "Templates/Header", s );
}

const QString pTemplatesManager::templatesHeader()
{
	QString s = pSettings::instance()->value( "Templates/Header", QString() ).toString();

	if ( s.isEmpty() )
		s = "/****************************************************************************\n"
			"**\n"
			"** 		Created using $editor_version_string$\n"
			"** Author    : $author$\n"
			"** Project   : $name$\n"
			"** FileName  : $filename$\n"
			"** Date      : $date$\n"
			"** License   : $license$\n"
			"** Comment   : $comment$\n"
			"**\n"
			"** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE\n"
			"** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n"
			"**\n"
			"****************************************************************************/\n";

	return s;
}

const QString pTemplatesManager::tokenize( QString s )
{
	return s.replace( templatesPath(), "$TEMPLATE_PATH$" );
}

const QString pTemplatesManager::unTokenize( QString s )
{
	return s.replace( "$TEMPLATE_PATH$", templatesPath() );
}

const QStringList pTemplatesManager::tokenize( QStringList l )
{
	for ( int i = 0; i < l.count(); i++ )
		l[i] = tokenize( l.at( i ) );
	return l;
}

const QStringList pTemplatesManager::unTokenize( QStringList l )
{
	for ( int i = 0; i < l.count(); i++ )
		l[i] = unTokenize( l.at( i ) );
	return l;
}

QString processFunction( const QString& v, const QString& f )
{
	// get lower function name
	QString s = f.toLower();
	// interpret
	if ( s == "upper" )
		return v.toUpper();
	else if ( s == "lower" )
		return v.toLower();
	else if ( s == "trimmed" )
		return v.trimmed();
	else if ( s == "simplified" )
		return v.simplified();
	// default value
	return v;
}

const QString pTemplatesManager::processContent( pTemplateContent tc )
{
	QHash<QString, QString> values;
	int p = 0;
	QString s;
	QRegExp r( "(\\$[^$\\s]+\\$)" );
	// search and interpret values
	while ( ( p = r.indexIn( tc.Content, p ) ) != -1 )
	{
		// got keyword
		s = r.capturedTexts().value( 1 ).toLower();
		// process value if needed
		if ( !values.contains( s ) )
		{
			// slip to check function needed
			QStringList l = QString( s ).replace( "$", "" ).split( "." );
			// set value
			QString v = l.at( 0 );
			// monkeystudio_version
			if ( v == "editor_version" )
				l[0] = PROGRAM_VERSION;
			// monkeystudio_version_string
			else if ( v == "editor_version_string" )
				l[0] = QString( "%1 v%2" ).arg( PROGRAM_NAME ).arg( PROGRAM_VERSION );
			// filename
			else if ( v == "filename" )
				l[0] = tc.FileName;
			// basename
			else if ( v == "basename" )
				l[0] = QFileInfo( tc.FileName ).baseName();
			// name
			else if ( v == "name" )
				l[0] = tc.Name;
			// date
			else if ( v == "date" )
				l[0] = QDateTime::currentDateTime().toString( Qt::ISODate );
			// license
			else if ( v == "license" )
				l[0] = tc.License;
			// comment
			else if ( v == "comment" )
				l[0] = tc.Comment;
			// author
			else if ( v == "author" )
				l[0] = tc.Author;
			// project
			else if ( v == "project" )
				l[0] = tc.Project ? tc.Project->name() : QString();
			else
				l[0] = "";
			// process function
			if ( l.count() > 1 )
				for ( int i = 0; i < l.count(); i++ )
					l[0] = processFunction( l.at( 0 ), l.at( i ) );
			// save value
			values[s] = l.at( 0 );
		}
		// next occurence
		p += r.matchedLength();
	}
	// replace occurences
	foreach ( QString k, values.keys() )
		tc.Content.replace( k, values[k], Qt::CaseInsensitive );
	// return value
	return tc.Content;
}
