/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pDesktopApplications_unix.cpp
** Date      : 2008-01-14T00:37:17
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
#include "pDesktopApplications.h"
#include "pMonkeyStudio.h"
#include "MonkeyExport.h"

#include <QSettings>

struct Q_MONKEY_EXPORT pThemeFolder
{
	pThemeFolder()	{}
		
	QStringList Path; // first entry defined the infos
	int Size;
	QString Context; // Applications, Actions, Animations, Categories, Devices, Emblems, Emotes, FileSystems, International, MimeTypes, Places, Status, Stock, 
	QString Type; // Threshold, Fixed, Scalable
	int MinSize; // Only Scalable
	int MaxSize; // Only Scalable
	int Threshold; // Only Threshold
};

struct Q_MONKEY_EXPORT pThemeFile
{
	pThemeFile() {}
		
	QStringList Path; // first entry defined the infos
	QString Comment;
	bool Hidden;
	QStringList Inherits;
	QHash<QString, pThemeFolder> Folders;
};

const QStringList mIconExtensions = QStringList() << "png" << "svg" << "xpm";
QHash<QString, pThemeFile> mThemes;
static bool mThemeHashed = false;

const QStringList themesPaths()
{
	QString s;
	QStringList l;
	// for backwards compatibility
	s = QString( "/.icons" ).prepend( qgetenv( "HOME" ) );
	if ( QFile::exists( s ) )
		l << s;
	// freedesktop recommendation
	s = QString( "/icons" ).prepend( qgetenv( "XDG_DATA_DIRS" ) );
	if ( QFile::exists( s ) )
		l << s;
	// freedesktop recommendation
	s = "/usr/share/pixmaps";
	if ( QFile::exists( s ) )
		l << s;
	// own add for fucking system
	s = "/usr/share/icons";
	if ( !l.contains( s ) && QFile::exists( s ) )
		l << s;
	// return
	return l;
}

bool DirectorySizeDistance( const pThemeFolder& t, int s )
{
	// Fixed
	if ( t.Type == "Fixed" )
		return abs( t.Size -s );
	// Scaled
	if ( t.Type == "Scaled" )
	{
		if ( s < t.MinSize )
			return t.MinSize -s;
		if ( s > t.MaxSize )
			return s -t.MaxSize;
		return 0;
	}
	// Threshold
  if ( t.Type == "Threshold" )
  {
		if ( s < t.Size -t.Threshold )
			return t.MinSize -s;
		if ( s > t.Size +t.Threshold )
			return s -t.MaxSize;
		return 0;
	}
	// None
	return 0;
}

bool DirectoryMatchesSize( const pThemeFolder& t, int s )
{
	// Fixed
  if ( t.Type == "Fixed" )
		return t.Size == s;
  // Scaled
  if ( t.Type == "Scaled" )
		return t.MinSize <= s <= t.MaxSize;
  // Threshold
  if ( t.Type == "Threshold" )
		return t.Size -t.Threshold <= s <= t.Size +t.Threshold;
  // None
  return 0;
}

QString LookupIcon( const QString& i, int s, const QString& t )
{
	// Fixed, Threshold
	QString f;
	foreach ( pThemeFolder tf, mThemes.value( t ).Folders )
	{
		foreach ( QString d, tf.Path )
		{
			foreach ( QString e, mIconExtensions )
			{
				if ( DirectoryMatchesSize( tf, s ) )
				{
					f = QString( "%1/%2.%3" ).arg( d, i, e );
					if ( QFile::exists( f ) )
						return f;
				}
			}
		}
	}
	// Scalable
	QString cf;
	int ms = INT_MAX;
	foreach ( pThemeFolder tf, mThemes.value( t ).Folders )
	{
		foreach ( QString d, tf.Path )
		{
			foreach ( QString e, mIconExtensions )
			{
				f = QString( "%1/%2.%3" ).arg( d, i, e );
				if ( QFile::exists( f ) && DirectorySizeDistance( tf, s ) < ms )
				{
					cf = f;
					ms = DirectorySizeDistance( tf, s );
				}
			}
		}
	}
	// return
	return cf;
}

QString LookupFallbackIcon( const QString& i )
{
	foreach ( QString d, themesPaths() )
	{
		foreach ( QString e, mIconExtensions )
		{
			d.append( QString( "/%1.%2" ).arg( i, e ) );
			if ( QFile::exists( d ) )
				return d;
		}
	}
	return QString();
}

QString FindIconHelper( const QString& i, int s, const QString& t )
{
	QStringList l;
  QString f = LookupIcon( i, s, t );
  if ( !f.isNull() )
    return f;
  if ( !mThemes.value( t ).Inherits.isEmpty() )
    l << mThemes.value( t ).Inherits;
  else if ( t.toLower() != "hicolor" )
    l << "hicolor";
	foreach ( QString p, l )
	{
    f = FindIconHelper( i, s, p );
    if ( !f.isEmpty() )
      return f;
  }
  return QString();
}

QString FindIcon( const QString& i, int s, const QString& t )
{
  QString f = FindIconHelper( i, s, t );
  if ( !f.isEmpty() )
    return f;
  return LookupFallbackIcon( i );
}

void hashThemes()
{
	if ( !mThemeHashed )
	{
		mThemeHashed = true;
		foreach ( QString tp, themesPaths() )
		{
			foreach ( QFileInfo f, QDir( tp ).entryInfoList( QDir::AllDirs | QDir::NoDotAndDotDot ) )
			{
				// continue if fodler don't contains index.theme
				const QString fn = f.absoluteFilePath().append( "/index.theme" );
				if ( !QFile::exists( fn ) )
					continue;
				// read file
				pThemeFile t;
				QSettings s( fn, QSettings::IniFormat );
				// theme
				s.beginGroup( "Icon Theme" );
				if ( mThemes.contains( s.value( "Name" ).toString() ) )
				{
					t = mThemes[ s.value( "Name" ).toString() ];
					// update
					t.Path << f.absoluteFilePath();
					foreach ( QString s, s.value( "Inherits" ).toStringList() )
						if ( !t.Inherits.contains( s ) )
							t.Inherits << s;
				}
				else
				{
					t.Path << f.absoluteFilePath();
					t.Comment = s.value( "Comment" ).toString();
					t.Hidden = s.value( "Hidden" ).toBool();
					t.Inherits = s.value( "Inherits" ).toStringList();
				}
				// add folders
				foreach ( QString d, s.value( "Directories" ).toStringList() )
				{
					pThemeFolder tf;
					// drop if already contains this folder
					if ( t.Folders.contains( d ) )
					{
						tf = t.Folders[ d ];
						tf.Path << f.absoluteFilePath().append( "/" ).append( d );
					}
					else
					{
						s.beginGroup( d );
						tf.Path << f.absoluteFilePath().append( "/" ).append( d );
						tf.Size = s.value( "Size" ).toInt();
						tf.Context = s.value( "Context" ).toString();
						tf.Type = s.value( "Type" ).toString();
						tf.MinSize = s.value( "MinSize", s.value( "Size" ) ).toInt();
						tf.MaxSize = s.value( "MaxSize", s.value( "Size" ) ).toInt();
						tf.Threshold = s.value( "Threshold", 2 ).toInt();
						s.endGroup();
					}
					// add to folders list
					t.Folders[ d ] = tf;
				}
				// add theme to hash
				mThemes[ s.value( "Name" ).toString() ] = t;
				s.endGroup();
			}
		}
	}
}

QStringList pDesktopApplications::startMenuPaths() const
{
	// TODO: implement freedesktop recommandation
	return QStringList( "/usr/share/applications" );
}

bool pDesktopApplications::categoriesAvailable() const
{ return true; }

void pDesktopApplications::populateApplications()
{
	// hash icons themes
	hashThemes();
	// check all applications path
	foreach ( const QString fp, startMenuPaths() )
	{
		foreach ( QFileInfo f, pMonkeyStudio::getFiles( QDir( fp ), "*.desktop" ) )
		{
			// get folder object
			pDesktopFolder* df = &mStartMenu;
			// get relative path
			QString p = f.absolutePath().remove( fp ).remove( 0, 1 );
			// get last folder object
			QString fd;
			foreach ( QString n, p.split( "/", QString::SkipEmptyParts ) )
			{
				fd += n +"/";
				if ( df->Folders.contains( n ) )
					df = df->Folders[n];
				else
				{
					df->Folders[n] = new pDesktopFolder();
					df = df->Folders[n];
					df->Path = fp +"/" +fd;
					if ( df->Path.endsWith( "/" ) )
						df->Path.chop( 1 );
				}
			}
			// update application for breaking freeze
			QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
			// open file as settings file
			QSettings s( f.absoluteFilePath(), QSettings::IniFormat );
			s.beginGroup( "Desktop Entry" );
			if ( !df->Applications.contains( f.absoluteFilePath() ) && !s.value( "Name" ).toString().isEmpty() )
			{
				// add application
				pDesktopApplication* a = new pDesktopApplication();
				a->Name = s.value( "Name" ).toString();
				a->Icon = s.value( "Icon" ).toString();
				if ( !QFile::exists( a->Icon ) )
					a->Icon = FindIcon( a->Icon, 24, "Crystal SVG" );
				a->GenericName = s.value( "GenericName" ).toString();
				a->Comment = s.value( "Comment" ).toString();
				a->Categories = s.value( "Categories" ).toStringList();
				df->Applications[f.absoluteFilePath()] = a;
			}
			s.endGroup();
		}
	}
}
