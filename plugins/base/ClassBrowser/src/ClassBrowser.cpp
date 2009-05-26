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
		connect( MonkeyCore::fileManager(), SIGNAL( fileOpened( const QString& ) ), this, SLOT( fileOpened( const QString& ) ) );
		connect( MonkeyCore::fileManager(), SIGNAL( currentFileChanged( pAbstractChild*, const QString& ) ), this, SLOT( currentFileChanged( pAbstractChild*, const QString& ) ) );
		connect( MonkeyCore::fileManager(), SIGNAL( opened( XUPProjectItem* ) ), this, SLOT( opened( XUPProjectItem* ) ) );
		connect( MonkeyCore::fileManager(), SIGNAL( buffersChanged( const QMap<QString, QString>& ) ), this, SLOT( buffersChanged( const QMap<QString, QString>& ) ) );
		connect( mDock->browser(), SIGNAL( entryActivated( qCtagsSenseEntry* ) ), this, SLOT( entryActivated( qCtagsSenseEntry* ) ) );
		connect( mDock->browser(), SIGNAL( fileNameActivated( const QString& ) ), this, SLOT( fileNameActivated( const QString& ) ) );
		connect( this, SIGNAL( systemPathsChanged( const QStringList& , const QStringList& ) ), mDock->browser(), SLOT( setSystemPaths( const QStringList& , const QStringList& ) ) );
		connect( this, SIGNAL( filteredSuffixesChanged( const QStringList& ) ), mDock->browser(), SLOT( setFilteredSuffixes( const QStringList& ) ) );
		connect( this, SIGNAL( integrationModeChanged( ClassBrowser::IntegrationMode ) ), this, SLOT( setIntegrationMode( ClassBrowser::IntegrationMode ) ) );
		connect( this, SIGNAL( integrationModeChanged( ClassBrowser::IntegrationMode ) ), MonkeyCore::multiToolBar(), SIGNAL( notifyChanges() ) );
		
		// set plugin enabled
		stateAction()->setChecked( true );
		// update integration mode
		emit integrationModeChanged( integrationMode() );
		// update filtered suffixes
		emit filteredSuffixesChanged( filteredSuffixes() );
		// index system paths
		emit systemPathsChanged( systemPaths(), QStringList() );
	}
	else if ( !b && isEnabled() )
	{
		// disconnections
		disconnect( MonkeyCore::fileManager(), SIGNAL( fileOpened( const QString& ) ), this, SLOT( fileOpened( const QString& ) ) );
		disconnect( MonkeyCore::fileManager(), SIGNAL( currentFileChanged( pAbstractChild*, const QString& ) ), this, SLOT( currentFileChanged( pAbstractChild*, const QString& ) ) );
		disconnect( MonkeyCore::fileManager(), SIGNAL( opened( XUPProjectItem* ) ), this, SLOT( opened( XUPProjectItem* ) ) );
		disconnect( MonkeyCore::fileManager(), SIGNAL( buffersChanged( const QMap<QString, QString>& ) ), this, SLOT( buffersChanged( const QMap<QString, QString>& ) ) );
		disconnect( mDock->browser(), SIGNAL( entryActivated( qCtagsSenseEntry* ) ), this, SLOT( entryActivated( qCtagsSenseEntry* ) ) );
		disconnect( mDock->browser(), SIGNAL( fileNameActivated( const QString& ) ), this, SLOT( fileNameActivated( const QString& ) ) );
		disconnect( this, SIGNAL( systemPathsChanged( const QStringList& , const QStringList& ) ), mDock->browser(), SLOT( setSystemPaths( const QStringList& , const QStringList& ) ) );
		disconnect( this, SIGNAL( filteredSuffixesChanged( const QStringList& ) ), mDock->browser(), SLOT( setFilteredSuffixes( const QStringList& ) ) );
		disconnect( this, SIGNAL( integrationModeChanged( ClassBrowser::IntegrationMode ) ), this, SLOT( setIntegrationMode( ClassBrowser::IntegrationMode ) ) );
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

ClassBrowser::IntegrationMode ClassBrowser::integrationMode() const
{
	return (ClassBrowser::IntegrationMode)settingsValue( "IntegrationMode", ClassBrowser::imBoth ).toInt();
}

void ClassBrowser::setIntegrationMode( ClassBrowser::IntegrationMode mode )
{
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

QStringList ClassBrowser::systemPaths() const
{
	return settingsValue( "SystemPaths" ).toStringList();
}

void ClassBrowser::setSystemPaths( const QStringList& paths )
{
	const QStringList oldPaths = systemPaths();
	
	if ( oldPaths != paths )
	{
		setSettingsValue( "SystemPaths", paths );
		
		emit systemPathsChanged( paths, oldPaths );
	}
}

QStringList ClassBrowser::filteredSuffixes() const
{
	const QStringList suffixes = QStringList()
		<< "*.gif" << "*.png" << "*.mng" << "*.jpg" << "*.jpeg" << "*.tiff" << "*.ico" << "*.icns"
		<< "*.pri" << "*.pro" << "*.qrc" << "*.ui" << "*.ts" << "*.qm" << "*.qch" << "*.xup" << "*.mks"
		<< "*.txt" << "*.iss" << "*.api" << "*.sip" << "*.ini" << "*.css" << "*.bak" << "*.old"
		<< "*.db" << "*.so" << "*.a" << "*.desktop"  << "*.gpl";
	
	return settingsValue( "FilteredSuffixes", suffixes ).toStringList();
}

void ClassBrowser::setFilteredSuffixes( const QStringList& suffixes )
{
	setSettingsValue( "FilteredSuffixes", suffixes );
	
	emit filteredSuffixesChanged( suffixes );
}

void ClassBrowser::fileOpened( const QString& fileName )
{
	if ( !fileName.isEmpty() )
	{
		mDock->browser()->tagEntry( fileName );
		mDock->browser()->setCurrentFileName( fileName );
	}
}

void ClassBrowser::currentFileChanged( pAbstractChild* child, const QString& fileName )
{
	Q_UNUSED( child );
	
	mDock->browser()->setCurrentFileName( fileName );
}

void ClassBrowser::opened( XUPProjectItem* project )
{
	mDock->browser()->tagEntry( project->path() );
}

void ClassBrowser::buffersChanged( const QMap<QString, QString>& entries )
{
	mDock->browser()->tagEntries( entries );
}

void ClassBrowser::entryActivated( qCtagsSenseEntry* entry )
{
	// need to stock parameters as they are temporary
	const QString fileName = entry->fileName;
	const int line = entry->lineNumber;
	
	MonkeyCore::fileManager()->goToLine( fileName, QPoint( 0, line ), true, pMonkeyStudio::defaultCodec() );
}

void ClassBrowser::fileNameActivated( const QString& fileName )
{
	MonkeyCore::fileManager()->openFile( fileName, pMonkeyStudio::defaultCodec() );
}

Q_EXPORT_PLUGIN2( BaseClassBrowser, ClassBrowser )
