/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pSettings.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class allow a singletonized QSettings for your application
 ********************************************************************************************************/
#include "pSettings.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMainWindow>

QString pSettings::mProgramName;
QString pSettings::mProgramVersion;

QString getIniFile( const QString& s )
{
#ifdef Q_OS_WIN
	return QString( "%1/%2.ini" ).arg( QApplication::applicationDirPath() ).arg( s );
#elif defined Q_OS_MAC
	return QString( "%1/../%2.ini" ).arg( QApplication::applicationDirPath() ).arg( s );
#elif defined Q_OS_UNIX
	return QString( "%1/.%2/%2.ini" ).arg( QDir::homePath() ).arg( s );
#else
	qFatal( "Platform not yet supported, please contact the authors." );
#endif
	return QString();
}

pSettings::pSettings( QObject* o )
	: QSettings( QDir::convertSeparators( getIniFile( mProgramName ) ), QSettings::IniFormat, o )
{ beginGroup( mProgramVersion ); }

pSettings::~pSettings()
{ endGroup(); }

void pSettings::setIniInformations( const QString& pName, const QString& pVersion )
{
	mProgramName = pName;
	mProgramVersion = pVersion;
}

QString pSettings::programName()
{ return mProgramName; }

QString pSettings::programVersion()
{ return mProgramVersion; }

void pSettings::restoreState( QMainWindow* w )
{
	if ( !w )
		return;
	w->restoreState( value( "MainWindow/State" ).toByteArray() );
	QPoint p = value( "MainWindow/Position" ).toPoint();
	QSize s = value( "MainWindow/Size" ).toSize();
	if ( !p.isNull() && !s.isNull() )
	{
		w->resize( s );
		w->move( p );
	}
	if ( value( "MainWindow/Maximized", true ).toBool() )
		w->showMaximized();
}

void pSettings::saveState( QMainWindow* w )
{
	if ( !w )
		return;
	setValue( "MainWindow/Maximized", w->isMaximized() );
	setValue( "MainWindow/Position", w->pos() );
	setValue( "MainWindow/Size", w->size() );
	setValue( "MainWindow/State", w->saveState() );
}
