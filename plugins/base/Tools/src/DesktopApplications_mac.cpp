/****************************************************************************
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
****************************************************************************/
#include "DesktopApplications.h"

#include <QDir>

const QFileInfoList getMacApplicationsFolders( QDir dir )
{
	QFileInfoList applications;
	
	foreach ( const QFileInfo& file, dir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot ) ) {
		if ( file.isBundle() ) {
			applications << file;
		}
		else if ( file.isDir() {
			dir.cd( file.filePath() );
			applications << getMacApplicationsFolders( dir );
			dir.cdUp();
		}
	}
	
	return applications;
}

QStringList DesktopApplications::startMenuPaths() const
{
	return QStringList( "/Applications" );
}

bool DesktopApplications::categoriesAvailable() const
{
	return false;
}

void DesktopApplications::scan()
{
	foreach ( const QString& path, startMenuPaths() ) {
		foreach ( const QFileInfo& file, getMacApplicationsFolders( QDir( path ) ) ) {
			// get folder object
			DesktopFolder* folder = &mStartMenu;
			// get relative path
			const QString applicationPath = file.absolutePath().remove( path ).remove( 0, 1 );
			// get last folder object
			QString s;
			#warning renamme the s variable, dunno what it mean ...
			
			foreach ( const QString& part, applicationPath.split( "/", QString::SkipEmptyParts ) ) {
				s += part +"/";
				
				if ( folder->folders.contains( part ) ) {
					folder = folder->folders[ part ];
				}
				else {
					folder->folders[ part ] = new DesktopFolder();
					folder = folder->folders[ part ];
					folder->path = path +"/" +s;
					
					if ( folder->path.endsWith( "/" ) ) {
						folder->path.chop( 1 );
					}
				}
			}
			
			// add application
			if ( !folder->applications.contains( file.absoluteFilePath() ) ) {
				DesktopApplication* application = new DesktopApplication();
				application->name = file.completeBaseName();
				application->icon = QString();
				application->genericName = QString();
				application->comment = QString();
				folder->applications[ file.absoluteFilePath() ] = application;
			}
		}
	}
}
