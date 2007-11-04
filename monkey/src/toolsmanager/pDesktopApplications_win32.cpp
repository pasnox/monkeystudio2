/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pDesktopApplications_win32.cpp
** Date      : 2007-11-04T22:46:52
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
