/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
#include "CommandLineManager.h"

#include "main.h"
#include "coremanager/MonkeyCore.h"
#include "workspace/pFileManager.h"
#include "pMonkeyStudio.h"

#include <QStringList>
#include <QCoreApplication>
#include <QDebug>

CommandLineManager::CommandLineManager()
{
    mVersionShown = false;
}

CommandLineManager::~CommandLineManager()
{
}

void CommandLineManager::parse()
{
    QStringList args = QCoreApplication::arguments();
    args.removeFirst();
    
    for ( int i = 0; i < args.count(); i++ ) {
        const QString arg = args.at( i ).toLower();
        bool needNextArgument = false;
        
        if ( arg == "-projects" || arg == "-files" ) {
            needNextArgument = true;
        }
        else {
            mArguments[ arg ].clear();
        }
        
        if ( needNextArgument ) {
            if ( i == args.count() -1 ) {
                break;
            }
            
            QString param;
            
            while ( !( param = args.at( i +1 ).toLower() ).startsWith( "-" ) ) {
                mArguments[ arg ] << param;
                i++;
                
                if ( i == args.count() -1 ) {
                    break;
                }
            }
        }
    }
}

void CommandLineManager::process()
{
    foreach ( const QString& arg, mArguments.keys() ) {
        if ( arg == "-h" || arg == "--help" ) {
            showHelp();
        }
        else if ( arg == "-v" || arg == "--version" ) {
            showVersion();
        }
        else if ( arg == "-projects" ) {
            openProjects( mArguments[ arg ] );
        }
        else if ( arg == "-files" ) {
            openFiles( mArguments[ arg ] );
        }
        else {
            qWarning( "Unknow argument: %s (%s)", arg.toLocal8Bit().constData(), mArguments[ arg ].join( " " ).toLocal8Bit().constData() );
        }
    }
}

const QMap<QString, QStringList>& CommandLineManager::arguments() const
{
    return mArguments;
}

void CommandLineManager::showVersion()
{
    if ( !mVersionShown ) {
        mVersionShown = true;
        qWarning( "%s version %s (%s)", PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_VERSION_STR );
        qWarning( "%s & The Monkey Studio Team", PACKAGE_COPYRIGHTS );
        qWarning( "http://%s", PACKAGE_DOMAIN );
    }
}

void CommandLineManager::showHelp()
{
    showVersion();
    qWarning();
    qWarning( "Command line arguments:" );
    qWarning( "\t-h, --help      Show command line help" );
    qWarning( "\t-v, --version   Show program version" );
    qWarning( "\t-projects      Open the projects given as parameters (-projects project1 ...)" );
    qWarning( "\t-files         Open the files given as parameters (-files file1 ...)" );
}

void CommandLineManager::openProjects( const QStringList& fileNames )
{
    QDir dir( QCoreApplication::applicationDirPath() );
    
    foreach ( QString fileName, fileNames ) {
        if ( QFileInfo( fileName ).isRelative() ) {
            fileName = QDir::cleanPath( dir.absoluteFilePath( fileName ) );
        }
        
        MonkeyCore::fileManager()->openProject( fileName, pMonkeyStudio::defaultCodec() );
    }
}

void CommandLineManager::openFiles( const QStringList& fileNames )
{
    QDir dir( QCoreApplication::applicationDirPath() );
    
    foreach ( QString fileName, fileNames ) {
        if ( QFileInfo( fileName ).isRelative() ) {
            fileName = QDir::cleanPath( dir.absoluteFilePath( fileName ) );
        }
        
        MonkeyCore::fileManager()->openFile( fileName, pMonkeyStudio::defaultCodec() );
    }
}
