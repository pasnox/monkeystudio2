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
#include "qCtagsSenseBrowser.h"
#include "qCtagsSense.h"
#include "qCtagsSenseIndexer.h"
#include "qCtagsSenseLanguagesModel.h"
#include "qCtagsSenseFilesModel.h"
#include "qCtagsSenseMembersModel.h"
#include "qCtagsSenseKindFinder.h"

#include <ctags.h>

#include <QMenu>
#include <QFileInfo>
#include <QDebug>

qCtagsSenseBrowser::qCtagsSenseBrowser( QWidget* parent )
	: QFrame( parent )
{
	setupUi( this );
	pbIndexing->setVisible( false );
	setAttribute( Qt::WA_DeleteOnClose );
	tvMembers->setContextMenuPolicy( Qt::CustomContextMenu );
	
	mSense = new qCtagsSense( this );
	mLanguagesModel = new qCtagsSenseLanguagesModel( mSense->sql() );
	mFilesModel = new qCtagsSenseFilesModel( mSense->sql() );
	mMembersModel = new qCtagsSenseMembersModel( mSense->sql() );
	
	cbLanguages->setModel( mLanguagesModel );
	cbFileNames->setModel( mFilesModel );
	
	QTreeView* tv = new QTreeView( cbMembers );
	tv->setHeaderHidden( true );
	cbMembers->setView( tv );
	cbMembers->setModel( mMembersModel );
	
	tvMembers->setHeaderHidden( true );
	tvMembers->setModel( mMembersModel );
	
	connect( mSense, SIGNAL( indexingStarted() ), pbIndexing, SLOT( show() ) );
	connect( mSense, SIGNAL( indexingProgress( int, int ) ), this, SLOT( mSense_indexingProgress( int, int ) ) );
	connect( mSense, SIGNAL( indexingFinished() ), pbIndexing, SLOT( hide() ) );
	connect( mSense, SIGNAL( indexingChanged() ), this, SLOT( mSense_indexingChanged() ) );
	
	connect( mLanguagesModel, SIGNAL( ready() ), this, SLOT( mLanguagesModel_ready() ) );
	connect( mFilesModel, SIGNAL( ready() ), this, SLOT( mFilesModel_ready() ) );
	connect( mMembersModel, SIGNAL( ready() ), this, SLOT( mMembersModel_ready() ) );
}

qCtagsSenseBrowser::~qCtagsSenseBrowser()
{
	delete mSense;
}

qCtagsSense* qCtagsSenseBrowser::sense() const
{
	return mSense;
}

void qCtagsSenseBrowser::tagEntry( const QString& fileName )
{
	mSense->tagEntry( fileName );
}

void qCtagsSenseBrowser::tagEntries( const QMap<QString, QString>& entries )
{
	mSense->tagEntries( entries );
}

void qCtagsSenseBrowser::setCurrentFileName( const QString& fileName )
{
	mLanguage = getFileNameLanguageName( fileName.toLocal8Bit().constData() );
	mFileName = fileName;
	
	if ( mSense->indexer()->isRunning() || mLanguagesModel->isRunning() || mFilesModel->isRunning() )
	{
		return;
	}
	
	// update languages combo
	bool languageLocked = cbLanguages->blockSignals( true );
	int lid = mLanguagesModel->indexOf( mLanguage );
	
	cbLanguages->setCurrentIndex( lid );
	cbLanguages->blockSignals( languageLocked );
	
	// update files combo
	bool fileLocked = cbFileNames->blockSignals( true );
	int fid = mFilesModel->indexOf( mFileName );
	
	cbFileNames->setCurrentIndex( fid );
	cbFileNames->blockSignals( fileLocked );
	
	// update view
	if ( fid != -1 )
	{
		mMembersModel->refresh( mFileName );
	}
	else
	{
		mFilesModel->refresh( mLanguage );
	}
}

void qCtagsSenseBrowser::on_cbLanguages_currentIndexChanged( int id )
{
	mLanguage = mLanguagesModel->language( id );
	mFileName.clear();
	mFilesModel->refresh( mLanguage );
}

void qCtagsSenseBrowser::on_cbFileNames_currentIndexChanged( int id )
{
	mFileName = mFilesModel->fileName( id );
	mMembersModel->refresh( mFileName );
}

void qCtagsSenseBrowser::on_cbMembers_currentIndexChanged( int id )
{
	Q_UNUSED( id );
	QModelIndex index = cbMembers->view()->currentIndex();
	qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
	emit memberActivated( entry );
}

void qCtagsSenseBrowser::on_tvMembers_activated( const QModelIndex& index )
{
	qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
	emit memberActivated( entry );
}

void qCtagsSenseBrowser::mSense_indexingProgress( int value, int total )
{
	pbIndexing->setValue( value );
	pbIndexing->setMaximum( total );
}

void qCtagsSenseBrowser::mSense_indexingChanged()
{
	if ( mLanguage.isEmpty() || mFileName.isEmpty() )
	{
		mLanguage = mLanguagesModel->language( cbLanguages->currentIndex() );
		mFileName = mFilesModel->fileName( cbFileNames->currentIndex() );
	}
	
	mLanguagesModel->refresh();
}

void qCtagsSenseBrowser::mLanguagesModel_ready()
{
	bool languageLocked = cbLanguages->blockSignals( true );
	int id = mLanguagesModel->indexOf( mLanguage );
	
	if ( id == -1 && cbLanguages->count() != 0 )
	{
		id = 0;
		mLanguage = mLanguagesModel->language( 0 );
	}
	
	//mLanguage = mLanguagesModel->language( id );
	cbLanguages->setCurrentIndex( id );
	mFilesModel->refresh( mLanguage );
	cbLanguages->blockSignals( languageLocked );
}

void qCtagsSenseBrowser::mFilesModel_ready()
{
	bool fileLocked = cbFileNames->blockSignals( true );
	int id = mFilesModel->indexOf( mFileName );
	
	if ( id == -1 && !cbFileNames->count() == 0 )
	{
		id = 0;
		mFileName = mFilesModel->fileName( 0 );
	}
	
	//mFileName = mFilesModel->fileName( id );
	cbFileNames->setCurrentIndex( id );
	mMembersModel->refresh( mFileName );
	cbFileNames->blockSignals( fileLocked );
}

void qCtagsSenseBrowser::mMembersModel_ready()
{
	qobject_cast<QTreeView*>( cbMembers->view() )->expandAll();
	tvMembers->expandAll();
}

void qCtagsSenseBrowser::on_tvMembers_customContextMenuRequested( const QPoint& pos )
{
	QModelIndex index = tvMembers->currentIndex();
	qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
	QMenu menu( this );
	
	switch ( entry->kind )
	{
		case qCtagsSense::Prototype:
		case qCtagsSense::Function:
		{
			QAction* aDeclaration = menu.addAction( "Go to declaration" );
			aDeclaration->setData( qCtagsSense::Prototype );
			QAction* aImplementation = menu.addAction( "Go to implementation" );
			aImplementation->setData( qCtagsSense::Function );
			break;
		}
		default:
		{
			QAction* aDeclaration = menu.addAction( "Go to declaration" );
			aDeclaration->setData( entry->kind );
			break;
		}
	}
	
	QAction* aTriggered = menu.exec( tvMembers->mapToGlobal( pos ) );
	
	if ( aTriggered )
	{
		qCtagsSense::Kind kind = (qCtagsSense::Kind)aTriggered->data().toInt();
		
		if ( entry->kind == kind )
		{
			emit memberActivated( entry );
		}
		else
		{
			qCtagsSenseKindFinder* cpp = new qCtagsSenseKindFinder( mSense->sql() );
			connect( cpp, SIGNAL( memberActivated( qCtagsSenseEntry* ) ), this, SIGNAL( memberActivated( qCtagsSenseEntry* ) ) );
			cpp->goTo( kind, entry );
		}
	}
}
