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
#ifdef Q_OS_MAC
	mPath = "..";
#elif defined Q_OS_WIN
	mPath = ".";
#else
	mPath = ".";
	if ( qApp->applicationDirPath() == "/usr/local/bin" )
		mPath = "../lib/monkeystudio";
#endif
	// templates
	setValue( "Templates/DefaultDirectories", QStringList( QString( "%1/templates" ).arg( mPath ) ) );
	// apis
	s = QString( "%1/apis" ).arg( mPath );
	setValue( "SourceAPIs/CMake", QStringList( s +"/cmake.api" ) );
	setValue( "SourceAPIs/C#", QStringList( s +"/cs.api" ) );
	setValue( "SourceAPIs/C++", QStringList() << s +"/c.api" << s +"/cpp.api" << s +"/glut.api" << s +"/opengl.api" << s +"/qt-4.3.2.api" );
	// translations
	setValue( "Translations/Path", QString( "%1/translations" ).arg( mPath ) );
	// plugins
	setValue( "Plugins/Path", QStringList() << QString( "%1/plugins" ).arg( mPath ) << "plugins" );
}
