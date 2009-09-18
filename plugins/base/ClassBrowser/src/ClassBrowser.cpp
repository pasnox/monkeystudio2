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
#include "ClassBrowser.h"
#include "pDockClassBrowser.h"
#include "ClassBrowserSettings.h"

#include <MonkeyCore.h>
#include <UIMain.h>
#include <pFileManager.h>
#include <qCtagsSenseBrowser.h>
#include <XUPProjectItem.h>
#include <pMonkeyStudio.h>
#include <qCtagsSense.h>
#include <pMultiToolBar.h>
#include <pAbstractChild.h>

#include <QDesktopServices>
#include <QDebug>

ClassBrowser::ClassBrowser()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Class Browser" );
	mPluginInfos.Description = tr( "Plugin for browsing classes members" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.FirstStartEnabled = true;
}

ClassBrowser::~ClassBrowser()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool ClassBrowser::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create dock
		mDock = new pDockClassBrowser( this );
		// create menu action for the dock
		pActionsManager::setDefaultShortcut( mDock->toggleViewAction(), QKeySequence( "F8" ) );
		// connections
		connect( MonkeyCore::fileManager(), SIGNAL( documentOpened( pAbstractChild* ) ), this, SLOT( documentOpened( pAbstractChild* ) ) );
		connect( MonkeyCore::fileManager(), SIGNAL( currentDocumentChanged( pAbstractChild* ) ), this, SLOT( currentDocumentChanged( pAbstractChild* ) ) );
		connect( MonkeyCore::fileManager(), SIGNAL( opened( XUPProjectItem* ) ), this, SLOT( opened( XUPProjectItem* ) ) );
		connect( MonkeyCore::fileManager(), SIGNAL( buffersChanged( const QMap<QString, QString>& ) ), this, SLOT( buffersChanged( const QMap<QString, QString>& ) ) );
		connect( mDock->browser(), SIGNAL( entryActivated( qCtagsSenseEntry* ) ), this, SLOT( entryActivated( qCtagsSenseEntry* ) ) );
		connect( mDock->browser(), SIGNAL( fileNameActivated( const QString& ) ), this, SLOT( fileNameActivated( const QString& ) ) );
		connect( this, SIGNAL( propertiesChanged( const qCtagsSenseProperties& ) ), mDock->browser(), SLOT( setProperties( const qCtagsSenseProperties& ) ) );
		connect( this, SIGNAL( integrationModeChanged( ClassBrowser::IntegrationMode ) ), MonkeyCore::multiToolBar(), SIGNAL( notifyChanges() ) );
		// update integration mode
		setIntegrationMode( integrationMode() );
		// update properties
		emit propertiesChanged( properties() );
		// set plugin enabled
		stateAction()->setChecked( true );
	}
	else if ( !b && isEnabled() )
	{
		// disconnections
		disconnect( MonkeyCore::fileManager(), SIGNAL( documentOpened( pAbstractChild* ) ), this, SLOT( documentOpened( pAbstractChild* ) ) );
		disconnect( MonkeyCore::fileManager(), SIGNAL( currentDocumentChanged( pAbstractChild* ) ), this, SLOT( currentDocumentChanged( pAbstractChild* ) ) );
		disconnect( MonkeyCore::fileManager(), SIGNAL( opened( XUPProjectItem* ) ), this, SLOT( opened( XUPProjectItem* ) ) );
		disconnect( MonkeyCore::fileManager(), SIGNAL( buffersChanged( const QMap<QString, QString>& ) ), this, SLOT( buffersChanged( const QMap<QString, QString>& ) ) );
		disconnect( mDock->browser(), SIGNAL( entryActivated( qCtagsSenseEntry* ) ), this, SLOT( entryActivated( qCtagsSenseEntry* ) ) );
		disconnect( mDock->browser(), SIGNAL( fileNameActivated( const QString& ) ), this, SLOT( fileNameActivated( const QString& ) ) );
		disconnect( this, SIGNAL( propertiesChanged( const qCtagsSenseProperties& ) ), mDock->browser(), SLOT( setProperties( const qCtagsSenseProperties& ) ) );
		disconnect( this, SIGNAL( integrationModeChanged( ClassBrowser::IntegrationMode ) ), MonkeyCore::multiToolBar(), SIGNAL( notifyChanges() ) );
		// it will remove itself from dock toolbar when deleted
		delete mDock;
		// set plugin disabled
		stateAction()->setChecked( false );
	}
	
	// return default value
	return true;
}

QPixmap ClassBrowser::pixmap() const
{
	return QPixmap( ":/icons/class.png" );
}

bool ClassBrowser::haveSettingsWidget() const
{
	return true;
}

QWidget* ClassBrowser::settingsWidget()
{
	return new ClassBrowserSettings( this, qApp->activeWindow() );
}

qCtagsSenseProperties ClassBrowser::properties() const
{
	const QStringList suffixes = QStringList()
		<< "*.gif" << "*.png" << "*.mng" << "*.jpg" << "*.jpeg" << "*.tiff" << "*.ico" << "*.icns"
		<< "*.pri" << "*.pro" << "*.qrc" << "*.ui" << "*.ts" << "*.qm" << "*.qch" << "*.xup" << "*.mks"
		<< "*.txt" << "*.iss" << "*.api" << "*.sip" << "*.ini" << "*.css" << "*.bak" << "*.old"
		<< "*.db" << "*.so" << "*.a" << "*.desktop"  << "*.gpl";
	
	qCtagsSenseProperties properties;
	
	properties.SystemPaths = settingsValue( "SystemPaths" ).toStringList();
	properties.FilteredSuffixes = settingsValue( "FilteredSuffixes", suffixes ).toStringList();
	properties.UsePhysicalDatabase = settingsValue( "UsePhysicalDatabase", false ).toBool();
	properties.DatabaseFileName = settingsValue( "DatabaseFileName", defaultDatabase() ).toString();
	
	return properties;
}

void ClassBrowser::setProperties( const qCtagsSenseProperties& properties )
{
	if ( this->properties() != properties )
	{
		setSettingsValue( "SystemPaths", properties.SystemPaths );
		setSettingsValue( "FilteredSuffixes", properties.FilteredSuffixes );
		setSettingsValue( "UsePhysicalDatabase", properties.UsePhysicalDatabase );
		setSettingsValue( "DatabaseFileName", properties.DatabaseFileName );
		
		emit propertiesChanged( properties );
	}
}

ClassBrowser::IntegrationMode ClassBrowser::integrationMode() const
{
	return (ClassBrowser::IntegrationMode)settingsValue( "IntegrationMode", ClassBrowser::imDock ).toInt();
}

void ClassBrowser::setIntegrationMode( ClassBrowser::IntegrationMode mode )
{
	if ( integrationMode() == mode )
	{
		//return;
	}
	
	if ( mDock )
	{
		switch ( mode )
		{
			case ClassBrowser::imDock:
				MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( mDock, infos().Caption, QIcon( pixmap() ) );
				MonkeyCore::multiToolBar()->toolBar( "Coding" )->removeAction( mDock->browser()->membersAction() );
				break;
			case ClassBrowser::imCombo:
				MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->removeDock( mDock );
				mDock->hide();
				MonkeyCore::multiToolBar()->toolBar( "Coding" )->addAction( mDock->browser()->membersAction() );
				break;
			case ClassBrowser::imBoth:
				MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( mDock, infos().Caption, QIcon( pixmap() ) );
				MonkeyCore::multiToolBar()->toolBar( "Coding" )->addAction( mDock->browser()->membersAction() );
				break;
		}
	}
	
	if ( integrationMode() != mode )
	{
		setSettingsValue( "IntegrationMode", mode );
		
		emit integrationModeChanged( mode );
	}
}

QString ClassBrowser::defaultDatabase()
{
	return QDir::cleanPath( QString( "%1/MkS_qCtagsSense.sqlite3" ).arg( QDesktopServices::storageLocation( QDesktopServices::TempLocation ) ) );
}

void ClassBrowser::documentOpened( pAbstractChild* document )
{
	if ( !document->filePath().isEmpty() )
	{
		mDock->browser()->tagEntry( document->filePath() );
	}
	
	mDock->browser()->setCurrentFileName( document->filePath() );
}

void ClassBrowser::currentDocumentChanged( pAbstractChild* document )
{
	if ( document )
	{
		mDock->browser()->setCurrentFileName( document->filePath() );
	}
}

void ClassBrowser::opened( XUPProjectItem* project )
{
	const QStringList files = project->topLevelProjectSourceFiles();
	mDock->browser()->tagEntries( files );
}

void ClassBrowser::buffersChanged( const QMap<QString, QString>& entries )
{
	mDock->browser()->tagEntries( entries );
}

void ClassBrowser::entryActivated( qCtagsSenseEntry* entry )
{	
	MonkeyCore::fileManager()->goToLine( entry->fileName, QPoint( 0, entry->lineNumber ), true, pMonkeyStudio::defaultCodec() );
}

void ClassBrowser::fileNameActivated( const QString& fileName )
{
	MonkeyCore::fileManager()->openFile( fileName, pMonkeyStudio::defaultCodec() );
}

Q_EXPORT_PLUGIN2( BaseClassBrowser, ClassBrowser )
