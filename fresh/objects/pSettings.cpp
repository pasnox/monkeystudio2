/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pSettings.cpp
** Date      : 2008-01-14T00:27:37
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
#include "pSettings.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMainWindow>

QString pSettings::mProgramName;
QString pSettings::mProgramVersion;

pSettings::pSettings( QObject* o,  const QString& pn, const QString& pv )
	: QSettings( QDir::convertSeparators( getIniFile( pn ) ), QSettings::IniFormat, o )
{ beginGroup( pv ); }

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

QString pSettings::getIniFile( const QString& s )
{
#ifdef Q_OS_MAC
	return QString( "%1/../%2.ini" ).arg( QApplication::applicationDirPath() ).arg( s );
#elif defined Q_OS_WIN
	return QString( "%1/%2.ini" ).arg( QApplication::applicationDirPath() ).arg( s );
#else
	return QString( "%1/.%2/%3.ini" ).arg( QDir::homePath() ).arg( mProgramName ).arg( s );
#endif
}

void pSettings::restoreState( QMainWindow* w )
{
	if ( !w )
		return;
	w->restoreGeometry( value( "MainWindow/Geometry" ).toByteArray() );
	w->restoreState( value( "MainWindow/State" ).toByteArray() );
	if ( value( "MainWindow/Geometry" ).toByteArray().isEmpty() )
		w->showMaximized();
}

void pSettings::saveState( QMainWindow* w )
{
	if ( !w )
		return;
	setValue( "MainWindow/Geometry", w->saveGeometry() );
	setValue( "MainWindow/State", w->saveState() );
}
