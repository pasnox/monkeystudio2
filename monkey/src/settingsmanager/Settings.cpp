/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : Settings.cpp
** Date      : 2008-01-14T00:37:11
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
	QString p = PROGRAM_PREFIX;
	if ( p.endsWith( "/" ) )
		p.chop( 1 );
	p.append( "/bin" );
	if ( qApp->applicationDirPath() == p )
		mPath = "../lib/monkeystudio";
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
	l << QString( "%1/plugins" ).arg( mPath );
	if ( !l.contains( "plugins" ) && !l.contains( "./plugins" ) )
		l << "plugins";
	setValue( "Plugins/Path", l );
}
