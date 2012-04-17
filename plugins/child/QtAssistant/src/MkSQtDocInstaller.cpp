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
#include "MkSQtDocInstaller.h"
#include "3rdparty/qtdocinstaller.h"

#include <main.h>
#include <coremanager/MonkeyCore.h>
#include <pQueuedMessageToolBar.h>
#include <settingsmanager/Settings.h>

#include <QDesktopServices>
#include <QDir>
#include <QHelpEngine>
#include <QResource>
#include <QTimer>

MkSQtDocInstaller::MkSQtDocInstaller( QHelpEngine* engine )
    : QObject( engine )
{
    mHelpEngine = engine;
    mQtDocInstaller = 0;
}

QString MkSQtDocInstaller::collectionFileDirectory( bool createDir, const QString& cacheDir )
{
    /*QString collectionPath = QDesktopServices::storageLocation( QDesktopServices::DataLocation ).remove( PACKAGE_NAME );
    if ( collectionPath.isEmpty() )
    {
        if ( cacheDir.isEmpty() )
            collectionPath = QDir::homePath() +QDir::separator() +QLatin1String( ".assistant" );
        else
            collectionPath = QDir::homePath() +QLatin1String( "/." ) +cacheDir;
    }
    else
    {
        if ( cacheDir.isEmpty() )
            collectionPath = collectionPath +QLatin1String( "/Trolltech/Assistant" );
        else
            collectionPath = collectionPath +QDir::separator() +cacheDir;
    }
    collectionPath = QDir::cleanPath( collectionPath );
    if ( createDir )
    {
        QDir dir;
        if ( !dir.exists( collectionPath ) )
            dir.mkpath( collectionPath );
    }
    return QDir::cleanPath( collectionPath );*/
    const QString path = QDir::cleanPath( QString( "%1/doc/%2" )
        .arg( QFileInfo( MonkeyCore::settings()->fileName() ).absolutePath() )
        .arg( cacheDir )
    );
    
    QDir().mkpath( path );
    return path;
}

QString MkSQtDocInstaller::defaultHelpCollectionFileName()
{
    return collectionFileDirectory() +QDir::separator() +QString( "qthelpcollection_%1.qhc" ).arg( qVersion() );
}

bool MkSQtDocInstaller::checkDocumentation()
{
    bool b = initHelpDB();
    if ( b )
        QTimer::singleShot( 0, this, SLOT( lookForNewQtDocumentation() ) );
    else
        MonkeyCore::messageManager()->appendMessage( tr( "Can't initialize documentation database" ) +" (Qt Assistant)" );
    return b;
}

bool MkSQtDocInstaller::initHelpDB()
{
    if ( !mHelpEngine->setupData() )
        return false;

    bool assistantInternalDocRegistered = false;
    foreach ( const QString& ns, mHelpEngine->registeredDocumentations() )
    {
        if ( ns.startsWith( QLatin1String( "com.trolltech.com.assistantinternal_" ) ) )
        {
            assistantInternalDocRegistered = true;
            break;
        }
    }

    bool needsSetup = false;
    if ( !assistantInternalDocRegistered )
    {
        QFileInfo fi( mHelpEngine->collectionFile() );
        const QString helpFile = fi.absolutePath() +QDir::separator() +QLatin1String( "assistant.qch" );
        if ( !QFile::exists( helpFile ) )
        {
            QFile file( helpFile );
            if ( file.open( QIODevice::WriteOnly ) )
            {
                QResource res( QLatin1String( ":/documentation/assistant.qch" ) );
                if ( file.write( (const char*)res.data(), res.size() ) != res.size() )
                    MonkeyCore::messageManager()->appendMessage( tr( "Could not write assistant.qch" ) +" (Qt Assistant )" );
                file.close();
            }
        }
        QHelpEngineCore hc( fi.absoluteFilePath() );
        hc.setupData();
        hc.registerDocumentation( helpFile );
        needsSetup = true;
    }

    int i = mHelpEngine->customValue( QLatin1String( "UnfilteredFilterInserted" ) ).toInt();
    if ( i != 1 )
    {
        {
            QHelpEngineCore hc( mHelpEngine->collectionFile() );
            hc.setupData();
            hc.addCustomFilter( tr( "Unfiltered" ), QStringList() );
            hc.setCustomValue( QLatin1String( "UnfilteredFilterInserted" ), 1 );
        }
        bool block = mHelpEngine->blockSignals( true );
        mHelpEngine->setCurrentFilter( tr( "Unfiltered" ) );
        mHelpEngine->blockSignals( block );
        needsSetup = true;
    }

    if ( needsSetup )
        mHelpEngine->setupData();
    return true;
}

void MkSQtDocInstaller::lookForNewQtDocumentation()
{
    mQtDocInstaller = new QtDocInstaller( mHelpEngine->collectionFile() );
    connect( mQtDocInstaller, SIGNAL( errorMessage( const QString& ) ), this, SLOT( displayInstallationError( const QString& ) ) );
    connect( mQtDocInstaller, SIGNAL( docsInstalled( bool ) ), this, SLOT( qtDocumentationInstalled( bool ) ) );

    /*
    QString versionKey = QString( QLatin1String( "qtVersion%1$$$qt" ) ).arg( QLatin1String( QT_VERSION_STR ) );
    if ( mHelpEngine->customValue( versionKey, 0 ).toInt() != 1 )
        MonkeyCore::messageManager()->appendMessage( tr( "Looking for Qt Documentation..." ) );
    */
    mQtDocInstaller->installDocs();
}

void MkSQtDocInstaller::displayInstallationError( const QString& errorMessage )
{
    MonkeyCore::messageManager()->appendMessage( errorMessage );
}

void MkSQtDocInstaller::qtDocumentationInstalled( bool newDocsInstalled )
{
    if ( newDocsInstalled )
        mHelpEngine->setupData();
}
