/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : QFileBrowser Plugin
** FileName  : pFileBrowser.cpp
** Date      : 2007-08-27T23:00:25
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pFileBrowser.h"
#include "pFileManager.h"

#include <QVBoxLayout>
#include <QComboBox>
#include <QListView>
#include <QDirModel>

pFileBrowser::pFileBrowser( QWidget* w )
	: QDockWidget( w )
{
	// restrict areas
	setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	
	// central widget
	QWidget* c = new QWidget;
	setWidget( c );
	
	// vertical layout
	QVBoxLayout* l = new QVBoxLayout( c );
	l->setMargin( 5 );
	l->setSpacing( 3 );
	
	// combo drive
	mComboBox = new QComboBox;
	l->addWidget( mComboBox );
	
	// files/folders view
	mListView = new QListView;
	l->addWidget( mListView );
	
	// dir model
	mDirModel = new QDirModel;
	mDirModel->setFilter( QDir::AllEntries | QDir::Readable | QDir::Hidden | QDir::CaseSensitive );
	mDirModel->setSorting( QDir::DirsFirst | QDir::Name );
	
	// assign model to views
	mComboBox->setModel( mDirModel );
	mListView->setModel( mDirModel );
	
	// if only one drive, hide it and root it ( linux/mac )
	if ( mComboBox->count() == 1 )
	{
		mComboBox->hide();
		mListView->setRootIndex( mDirModel->index( 0, 0 ) );
	}
	
	// connections
	connect( mComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( cb_currentIndexChanged( const QString& ) ) );
	connect( mListView, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( lv_doubleClicked( const QModelIndex& ) ) );
}

pFileBrowser::~pFileBrowser()
{
	delete mDirModel;
}

void pFileBrowser::lv_doubleClicked( const QModelIndex& i )
{
	// if dir, set root index to it
	if ( mDirModel->isDir( i ) )
		mListView->setRootIndex( i );
	// open file
	else
		pFileManager::instance()->openFile( mDirModel->filePath( i ) );

	// select correct drive in combo if needed
	if ( QDir::drives().contains( mDirModel->fileName( i ) ) )
		mComboBox->setCurrentIndex( mComboBox->findText( mDirModel->fileName( i ).remove( -1, 1 ) ) );
}

void pFileBrowser::cb_currentIndexChanged( const QString& s )
{
	// set correct drive in list view
	mListView->setRootIndex( mDirModel->index( s ) );
}
