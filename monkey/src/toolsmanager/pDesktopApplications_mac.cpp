/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox
** Project   : pDesktopApplications
** FileName  : pDesktopApplications_mac.cpp
** Date      : sam. août 18 19:54:58 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pDesktopApplications.h"

#include <QDir>

const QFileInfoList getMacApplicationsFolders( QDir d )
{
	QFileInfoList ll;
	foreach ( QFileInfo f, d.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name ) )
	{
		if ( f.fileName().endsWith( ".app" ) )
			ll << f;
		else 
		{
			d.cd( f.filePath() );
			ll << getMacApplicationsFolders( d );
			d.cdUp();
		}
	}
	return ll;
}

QStringList pDesktopApplications::startMenuPaths() const
{ return QStringList( "/Applications" ); }

bool pDesktopApplications::categoriesAvailable() const
{ return false; }

void pDesktopApplications::populateApplications()
{
	foreach ( const QString fp, startMenuPaths() )
	{
		foreach ( QFileInfo f, getMacApplicationsFolders( QDir( fp ) ) )
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
