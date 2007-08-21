/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pSettings.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class allow a singletonized QSettings for your application
 ********************************************************************************************************/
#include "pSettings.h"

#include <QDir>
#include <QFile>
#include <QMainWindow>

pSettings::pSettings( const QString& pName, const QString& pVersion, QObject* o )
	: QSettings( QDir::convertSeparators( QString( "%1/.%2/%3.ini" ).arg( QDir::homePath(), pName, pName ) ), QSettings::IniFormat, o )
{
	mProgramName = pName;
	mProgramVersion = pVersion;
	beginGroup( mProgramVersion );
}

pSettings::~pSettings()
{
	endGroup();
}

QString pSettings::programName() const
{
	return mProgramName;
}

QString pSettings::programVersion() const
{
	return mProgramVersion;
}

void pSettings::restoreState( QMainWindow* w )
{
	if ( !w )
		return;

	QPoint p = value( "MainWindow/Position" ).toPoint();
	QSize s = value( "MainWindow/Size" ).toSize();
	if ( !p.isNull() && !s.isNull() )
	{
		w->resize( s );
		w->move( p );
	}
	if ( value( "MainWindow/Maximized", true ).toBool() )
		w->showMaximized();
	w->restoreState( value( "MainWindow/State" ).toByteArray() );
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
