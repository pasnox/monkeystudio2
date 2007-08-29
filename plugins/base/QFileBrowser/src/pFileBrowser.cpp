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

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QComboBox>
#include <QListView>
#include <QDirModel>

pFileBrowser::pFileBrowser( QWidget* w )
	: QDockWidget( w ), mShown( false )
{
	// restrict areas
	setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	
	// central widget
	QWidget* c = new QWidget;
	setWidget( c );
	
	// vertical layout
	QVBoxLayout* v = new QVBoxLayout( c );
	v->setMargin( 0 );
	v->setSpacing( 3 );
	
	// horizontal layout for toolbutton && combobox
	QHBoxLayout* h = new QHBoxLayout;
	h->setMargin( 0 );
	h->setSpacing( 3 );
	
	// toolbutton
	QToolButton* tb = new QToolButton;
	tb->setIcon( QIcon( ":/icons/up.png" ) );
	h->addWidget( tb );
	
	// combo drive
	mComboBox = new QComboBox;
	h->addWidget( mComboBox );
	
	// add horizontal layout into vertical one
	v->addLayout( h );
	
	// files/folders view
	mListView = new QListView;
	v->addWidget( mListView );
	
	// dir model
	mDirModel = new QDirModel;
	mDirModel->setFilter( QDir::AllEntries | QDir::Readable | QDir::Hidden | QDir::CaseSensitive | QDir::NoDotAndDotDot );
	mDirModel->setSorting( QDir::DirsFirst | QDir::Name );
	
	// assign model to views
	mComboBox->setModel( mDirModel );
	mListView->setModel( mDirModel );
	
	// if only one drive, hide it and root it ( linux/mac )
	if ( mComboBox->count() == 1 )
	{
		mComboBox->setEnabled( false );
		mListView->setRootIndex( mDirModel->index( 0, 0 ) );
	}
	
	// connections
	connect( tb, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( mComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( cb_currentIndexChanged( const QString& ) ) );
	connect( mListView, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( lv_doubleClicked( const QModelIndex& ) ) );
}

pFileBrowser::~pFileBrowser()
{
	// delete model
	delete mDirModel;
}

void pFileBrowser::showEvent( QShowEvent* e )
{
	// default event
	QDockWidget::showEvent( e );
	// check if first showEvent
	if ( !mShown )
	{
		mShown = true;
		// restore drive and path
		emit restoreSettings();
	}
}

void pFileBrowser::hideEvent( QHideEvent* e )
{
	// default event
	QDockWidget::hideEvent( e );
	// save drive and path
	if ( mShown )
		emit saveSettings();
}

void pFileBrowser::tb_clicked()
{
	mListView->setRootIndex( mListView->rootIndex().parent() );
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

QString pFileBrowser::currentDrive() const
{
	return mComboBox->currentText();
}

void pFileBrowser::setCurrentDrive( const QString& s )
{
	mComboBox->setEditText( s );
}

QString pFileBrowser::currentPath() const
{
	return mDirModel->filePath( mListView->rootIndex() );
}

void pFileBrowser::setCurrentPath( const QString& s )
{
	mListView->setRootIndex( mDirModel->index( s ) );
}

