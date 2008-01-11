#include "Settings.h"

#include <QApplication>
#include <QStringList>

Settings::Settings( QObject* o )
	: pSettings( o )
{}

void Settings::setDefaultSettings()
{
	QString mPath;
	QString s;
	QStringList l;
#ifdef Q_OS_MAC
	mPath = "..";
#elif defined Q_OS_WIN
	mPath = ".";
#else
	mPath = ".";
	if ( qApp->applicationDirPath() == "/usr/local/bin" )
		mPath = "/usr/local/monkeystudio";
#endif
	// templates
	l.clear();
	l << QString( "%1/templates" ).arg( mPath );
	if ( !l.contains( "../templates" ) )
		l << "../templates";
	setValue( "Templates/DefaultDirectories", l );
	// apis
	s = QString( "%1/apis" ).arg( mPath );
	setValue( "SourceAPIs/CMake", QStringList( s +"/cmake.api" ) );
	setValue( "SourceAPIs/C#", QStringList( s +"/cs.api" ) );
	setValue( "SourceAPIs/C++", QStringList() << s +"/c.api" << s +"/cpp.api" << s +"/glut.api" << s +"/opengl.api" << s +"/qt-4.3.2.api" );
	// translations
	l.clear();
	l << QString( "%1/translations" ).arg( mPath );
	if ( !l.contains( "../translations" ) )
		l << "../translations";
	setValue( "Translations/Path", l );
	// plugins
	l.clear();
	if (mPath == "/usr/local/bin/")
		l<< "/usr/local/monkeystudio/plugins";
	else
		l << QString( "%1/plugins" ).arg( mPath );
	if ( !l.contains( "plugins" ) )
		l << "plugins";
	setValue( "Plugins/Path", l );
}
