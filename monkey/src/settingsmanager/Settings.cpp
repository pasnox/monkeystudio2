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
#include <QDir>

Settings::Settings( QObject* o )
	: pSettings( o )
{}

void Settings::setDefaultSettings()
{
	QString mPath;
	QString s;
	QStringList l;
	bool relativePath;
	bool binContainsPlugins = false;
#ifdef Q_OS_MAC
	mPath = "..";
#elif defined Q_OS_WIN
	mPath = ".";
#else
	mPath = "..";
	// get app path
	const QString appPath = qApp->applicationDirPath();
	// get app folders list
	const QStringList folders = QDir( appPath ).entryList( QDir::AllDirs | QDir::NoDotAndDotDot );
	// if it contains at least plugins it can be installed fodler
	if ( folders.contains( "plugins" ) )
	{
		// plugin is default build into bin, but datas are .. level
		mPath = "..";
		binContainsPlugins = true;
		// if it contains apis, then all fodlers are there
		if ( folders.contains( "apis" ) )
		mPath = ".";
	}
	else if ( QDir( PACKAGE_DATAS ).exists() )
		mPath = PACKAGE_DATAS;
#endif
	relativePath = QDir( mPath ).isRelative();
	// templates
	l.clear();
	l << QString( "%1/templates" ).arg( mPath );
	if ( !l.contains( "../templates" ) )
		l << "../templates";
	setValue( "Templates/DefaultDirectories", l );
	// apis
	s = QString( "%1/apis" ).arg( mPath );
	if ( !QDir( s ).exists() )
		s = QString( "%1/ctags/apis" ).arg( mPath );
	setValue( "SourceAPIs/CMake", QStringList( s +"/cmake.api" ) );
	setValue( "SourceAPIs/C#", QStringList( s +"/cs.api" ) );
	setValue( "SourceAPIs/C++", QStringList() << s +"/c.api" << s +"/cpp.api" << s +"/glut.api" << s +"/opengl.api" << s +"/qt-4.4.0.api" );
	// translations
	l.clear();
	l << QString( "%1/translations" ).arg( mPath );
	if ( !l.contains( "../translations" ) )
		l << "../translations";
	setValue( "Translations/Path", l );
	// plugins
	l.clear();
	if ( !binContainsPlugins )
		l << QString( "%1/plugins" ).arg( mPath );
	if ( !l.contains( "plugins" ) && !l.contains( "./plugins" ) )
		l << "plugins";
	setValue( "Plugins/Path", l );
	// syntax highlighter
	setDefaultCppSyntaxHighlight();
}

void Settings::setDefaultCppSyntaxHighlight()
{
#if defined Q_OS_MAC
	const QString font = "Bitstream Vera Sans Mono, 10";
#elif defined Q_OS_WIN
	const QString font = "Courier New, 10";
#else
	const QString font = "Bitstream Vera Sans Mono, 9";
#endif
	// configure styles
	LexerStyleList styles;	
	styles << LexerStyle( 0, 0, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 1, 10526880, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 2, 10526880, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 3, 8421631, false, "%1, 1, 0, 0", 16777215 );
	styles << LexerStyle( 4, 15728880, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 5, 160, false, "%1, 1, 0, 0", 16777215 );
	styles << LexerStyle( 6, 255, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 7, 14721024, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 9, 40960, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 10, 16711680, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 11, 0, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 12, 0, true, "%1, 0, 0, 0", 16711680 );
	styles << LexerStyle( 15, 8421631, false, "%1, 1, 0, 0", 16777215 );
	styles << LexerStyle( 16,0 , false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 17, 32896, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 18, 8388608, false, "%1, 0, 0, 0", 16777215 );
	styles << LexerStyle( 19, 0, false, "%1, 0, 0, 0", 16777215 );
	// write styles
	beginGroup( "Scintilla/C++" );
	foreach ( const LexerStyle& style, styles )
	{
		beginGroup( QString( "style%1" ).arg( style.id ) );
		setValue( "color", style.color );
		setValue( "eolfill", style.eolfill );
		setValue( "font", style.font.arg( font ).split( ',' ) );
		setValue( "paper", style.paper );
		endGroup();
	}
	setValue( "properties/foldatelse", QVariant( true ).toString() );
	setValue( "properties/foldcomments", QVariant( true ).toString() );
	setValue( "properties/foldcompact", QVariant( true ).toString() );
	setValue( "properties/foldpreprocessor", QVariant( true ).toString() );
	setValue( "properties/stylepreprocessor", QVariant( true ).toString() );
	setValue( "defaultcolor", 0 );
	setValue( "defaultpaper", 16777215 );
	setValue( "defaultfont", QString( "Verdana, 10, 0, 0, 0" ).split( ',' ) );
	setValue( "autoindentstyle", 1 );
	endGroup();
}
