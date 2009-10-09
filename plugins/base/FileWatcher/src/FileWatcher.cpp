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
/*!
	\file FileWatcher.cpp
	\date 2009-10-06
	\author Filipe AZEVEDO
	\brief FileWatcher plugin main class. Implementing plugin interface for the core
*/

#include "FileWatcher.h"

#include <pIconManager.h>
#include <MonkeyCore.h>
#include <pFileManager.h>
#include <pAbstractChild.h>
#include <pOpenedFileExplorer.h>

#include <QFileSystemWatcher>
#include <QDebug>

/*!
	Get settings widget of plugin
	\return Pointer to created settings widget for plugin
*/
QWidget* FileWatcher::settingsWidget()
{
	return 0;
}

void FileWatcher::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "File Watcher" );
	mPluginInfos.Description = tr( "Plugin for tracking opened documents changes." );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = true;
	mPluginInfos.Pixmap = pIconManager::pixmap( "filewatcher.png" );
}

/*!
	Install plugin to the system
	\return Status code of action
	\retval true Successfull
	\retval false Some error ocurred
*/
bool FileWatcher::install()
{
	mFileWatcher = new QFileSystemWatcher( this );
	pFileManager* fm = MonkeyCore::fileManager();
	
	connect( mFileWatcher, SIGNAL( fileChanged( const QString& ) ), this, SLOT( fileChanged( const QString& ) ) );
	connect( fm, SIGNAL( documentOpened( pAbstractChild* ) ), this, SLOT( documentOpened( pAbstractChild* ) ) );
	connect( fm, SIGNAL( documentChanged( pAbstractChild* ) ), this, SLOT( documentChanged( pAbstractChild* ) ) );
	connect( fm, SIGNAL( documentModifiedChanged( pAbstractChild*, bool ) ), this, SLOT( documentModifiedChanged( pAbstractChild*, bool ) ) );
	connect( fm, SIGNAL( documentAboutToClose( pAbstractChild* ) ), this, SLOT( documentAboutToClose( pAbstractChild* ) ) );
	connect( fm, SIGNAL( documentClosed( pAbstractChild* ) ), this, SLOT( documentClosed( pAbstractChild* ) ) );
	connect( fm, SIGNAL( currentDocumentChanged( pAbstractChild* ) ), this, SLOT( currentDocumentChanged( pAbstractChild* ) ) );
	
	return true;
}

/*!
	Uninstall plugin from the system
	\return Status code of action
	\retval true Successfull
	\retval false Some error ocurred
*/
bool FileWatcher::uninstall()
{
	pFileManager* fm = MonkeyCore::fileManager();
	
	disconnect( mFileWatcher, SIGNAL( fileChanged( const QString& ) ), this, SLOT( fileChanged( const QString& ) ) );
	disconnect( fm, SIGNAL( documentOpened( pAbstractChild* ) ), this, SLOT( documentOpened( pAbstractChild* ) ) );
	disconnect( fm, SIGNAL( documentChanged( pAbstractChild* ) ), this, SLOT( documentChanged( pAbstractChild* ) ) );
	disconnect( fm, SIGNAL( documentModifiedChanged( pAbstractChild*, bool ) ), this, SLOT( documentModifiedChanged( pAbstractChild*, bool ) ) );
	disconnect( fm, SIGNAL( documentAboutToClose( pAbstractChild* ) ), this, SLOT( documentAboutToClose( pAbstractChild* ) ) );
	disconnect( fm, SIGNAL( documentClosed( pAbstractChild* ) ), this, SLOT( documentClosed( pAbstractChild* ) ) );
	disconnect( fm, SIGNAL( currentDocumentChanged( pAbstractChild* ) ), this, SLOT( currentDocumentChanged( pAbstractChild* ) ) );
	
	mFileWatcher->deleteLater();
	
	return true;
}

void FileWatcher::fileChanged( const QString& path )
{
	mExternallyModified[ path ] = QFile::exists( path ) ? FileWatcher::Modified : FileWatcher::Deleted;
	pAbstractChild* document = MonkeyCore::fileManager()->openedDocument( path );
	
	if ( mExternallyModified[ path ] == FileWatcher::Modified )
	{
		const QString documentBuffer = document->fileBuffer();
		QString fileBuffer;
		
		QFile file( path );
		file.open( QIODevice::ReadOnly ); // does not need to check as file is already open we can read it.
		fileBuffer = document->codec()->toUnicode( file.readAll() );
		file.close();
		
		if ( documentBuffer == fileBuffer && !document->isModified() )
		{
			mExternallyModified[ path ] = FileWatcher::None;
		}
	}
	
	qWarning() << "changed" << QFile::exists( document->filePath() ) << mExternallyModified[ path ];
	
	documentChanged( document );
}

void FileWatcher::documentOpened( pAbstractChild* document )
{
	const QString path = document->filePath();
	pOpenedFileExplorer* dock = MonkeyCore::workspace()->dockWidget();
	pOpenedFileModel* model = dock->model();
	mExternallyModified[ path ] = FileWatcher::None;
	mFileWatcher->addPath( path );
	
	const QString toolTip = tr( "File Path: %1\nLocally Modified: %2\nExternally Modified: %3\nExternally Deleted: %4" )
		.arg( path )
		.arg( QVariant( document->isModified() ).toString() )
		.arg( QVariant( mExternallyModified[ path ] == FileWatcher::Modified ).toString() )
		.arg( QVariant( mExternallyModified[ path ] == FileWatcher::Deleted ).toString() );
	
	model->setDocumentToolTip( document, toolTip );
}

void FileWatcher::documentChanged( pAbstractChild* document )
{
	const QString path = document->filePath();
	pOpenedFileExplorer* dock = MonkeyCore::workspace()->dockWidget();
	pOpenedFileModel* model = dock->model();
	QIcon icon;
	
	// externally deleted files make the filewatcher to no longer watch them
	if ( !mFileWatcher->files().contains( path ) )
	{
		mFileWatcher->addPath( path );
	}
	
	switch ( mExternallyModified[ path ] )
	{
		case FileWatcher::None:
			break;
		case FileWatcher::Modified:
		case FileWatcher::Deleted:
			icon = pIconManager::icon( "modified-deleted.png" );
			break;
	}
	
	if ( mExternallyModified[ path ] != FileWatcher::None && document->isModified() )
	{
		icon = pIconManager::icon( "modified-locally-externally.png" );
	}
	
	const QString toolTip = tr( "File Path: %1\nLocally Modified: %2\nExternally Modified: %3\nExternally Deleted: %4" )
		.arg( path )
		.arg( QVariant( document->isModified() ).toString() )
		.arg( QVariant( mExternallyModified[ path ] == FileWatcher::Modified ).toString() )
		.arg( QVariant( mExternallyModified[ path ] == FileWatcher::Deleted ).toString() );
	
	model->setDocumentIcon( document, icon );
	model->setDocumentToolTip( document, toolTip );
}

void FileWatcher::documentModifiedChanged( pAbstractChild* document, bool modified )
{
	Q_UNUSED( modified );
	documentChanged( document );
}

void FileWatcher::documentAboutToClose( pAbstractChild* document )
{
	mFileWatcher->removePath( document->filePath() );
	mExternallyModified.remove( document->filePath() );
}

void FileWatcher::documentClosed( pAbstractChild* document )
{
	Q_UNUSED( document );
}

void FileWatcher::currentDocumentChanged( pAbstractChild* document )
{
	Q_UNUSED( document );
}

Q_EXPORT_PLUGIN2( BaseFileWatcher, FileWatcher )