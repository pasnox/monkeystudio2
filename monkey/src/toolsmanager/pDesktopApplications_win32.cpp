/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pDesktopApplications_win32.cpp
** Date      : 2008-01-14T00:37:18
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

#define _WIN32_IE 0x0400
#include <shlobj.h>

QStringList pDesktopApplications::startMenuPaths() const
{
	QStringList l;
	wchar_t s[MAX_PATH];
	// get common start menu files
	if ( SHGetSpecialFolderPathW( NULL, s, CSIDL_COMMON_STARTMENU, false ) )
		l << QString::fromWCharArray( s ).replace( "\\", "/" );
	// get start menu files
	if ( SHGetSpecialFolderPathW( NULL, s, CSIDL_STARTMENU, false ) )
		l << QString::fromWCharArray( s ).replace( "\\", "/" );
	// return values
	return l;
}

bool pDesktopApplications::categoriesAvailable() const
{ return false; }

void pDesktopApplications::populateApplications()
{
	foreach ( const QString fp, startMenuPaths() )
	{
		foreach ( QFileInfo f, pMonkeyStudio::getFiles( QDir( fp ) ) )
		{
			// get folder object
			pDesktopFolder* df = &mStartMenu;
			// get relative path
			QString p = f.absolutePath().remove( fp ).remove( 0, 1 );
			// get last folder object
			QString s;
			foreach ( QString n, p.split( "/", QString::SkipEmptyParts ) )
			{
				s += n +"/";
				if ( df->Folders.contains( n ) )
					df = df->Folders[n];
				else
				{
					df->Folders[n] = new pDesktopFolder();
					df = df->Folders[n];
					df->Path = fp +"/" +s;
					if ( df->Path.endsWith( "/" ) )
						df->Path.chop( 1 );
				}
			}
			// add application
			if ( !df->Applications.contains( f.absoluteFilePath() ) )
			{
				pDesktopApplication* a = new pDesktopApplication();
				a->Name = f.completeBaseName();
				a->Icon = QString();
				a->GenericName = QString();
				a->Comment = QString();
				df->Applications[f.absoluteFilePath()] = a;
			}
		}
	}
}
