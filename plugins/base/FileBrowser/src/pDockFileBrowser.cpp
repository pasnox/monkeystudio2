/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : FileBrowser Plugin
** FileName  : pDockFileBrowser.cpp
** Date      : 2007-08-27T23:00:25
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pDockFileBrowser.h"
#include "pFileManager.h"
#include "pMonkeyStudio.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QComboBox>
#include <QListView>
#include <QDirModel>

pDockFileBrowser::pDockFileBrowser( QWidget* w )
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
	
	// combo dir model
	mComboDirModel = new QDirModel;
	mComboDirModel->setFilter( QDir::Dirs | QDir::Readable | QDir::Hidden | QDir::CaseSensitive | QDir::NoDotAndDotDot );
	mComboDirModel->setSorting( QDir::Name );
	
	// list dir model
	mListDirModel = new QDirModel;
	mListDirModel->setFilter( QDir::AllEntries | QDir::Readable | QDir::Hidden | QDir::CaseSensitive | QDir::NoDotAndDotDot );
	mListDirModel->setSorting( QDir::DirsFirst | QDir::Name );
	
	// assign model to views
	mComboBox->setModel( mComboDirModel );
	mListView->setModel( mListDirModel );
	
	// if only one drive, disable it and root it ( linux/mac )
	if ( mComboBox->count() == 1 )
		mComboBox->setEnabled( false );
	
	// set root index
	mComboBox->setRootModelIndex( QModelIndex() );
	mListView->setRootIndex( mListDirModel->index( mComboDirModel->fileName( mComboBox->rootModelIndex() ).append( mComboBox->currentText() ) ) );
	
	// connections
	connect( tb, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( mComboBox->view(), SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( cb_clicked( const QModelIndex& ) ) );
	connect( mListView, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( lv_doubleClicked( const QModelIndex& ) ) );
}

pDockFileBrowser::~pDockFileBrowser()
{
	// delete models
	delete mComboDirModel;
	delete mListDirModel;
}

void pDockFileBrowser::showEvent( QShowEvent* e )
{
	// default event
	QDockWidget::showEvent( e );
	// check if first showEvent
	if ( !mShown )
	{
		mShown = true;
		// restore settings
		//emit restoreSettings();
	}
}

void pDockFileBrowser::hideEvent( QHideEvent* e )
{
	// default event
	QDockWidget::hideEvent( e );
	// save settings
	//if ( mShown )
		//emit saveSettings();
}

void pDockFileBrowser::tb_clicked()
{
	//const QString p = mComboDirModel->filePath( mComboBox->rootModelIndex() );
	//pMonkeyStudio::warning( "", p );
	mComboBox->setRootModelIndex( mComboBox->rootModelIndex().parent() );
	//mListView->setRootIndex( mListDirModel->index( p ) );
}

void pDockFileBrowser::lv_doubleClicked( const QModelIndex& i )
{
	/*
	// if dir, set root index to it
	if ( mDirModel->isDir( i ) )
		mListView->setRootIndex( i );
	// open file
	else
		pFileManager::instance()->openFile( mDirModel->filePath( i ) );

	// select correct drive in combo if needed
	if ( QDir::drives().contains( mDirModel->fileName( i ) ) )
		mComboBox->setCurrentIndex( mComboBox->findText( mDirModel->fileName( i ).remove( -1, 1 ) ) );
	*/
}

void pDockFileBrowser::cb_clicked( const QModelIndex& i )
{
	pMonkeyStudio::warning( "", mComboDirModel->filePath( i ) );
	mComboBox->setRootModelIndex( i );
	mListView->setRootIndex( mListDirModel->index( mComboDirModel->filePath( i ) ) );
}

QString pDockFileBrowser::currentDrive() const
{
	return mComboBox->currentText();
}

void pDockFileBrowser::setCurrentDrive( const QString& s )
{
	//mComboBox->setRootModelIndex( mDirModel->index( s ) );
}

QString pDockFileBrowser::currentPath() const
{
	return mListDirModel->filePath( mListView->rootIndex() );
}

void pDockFileBrowser::setCurrentPath( const QString& s )
{
	//mListView->setRootIndex( mDirModel->index( s ) );
}

