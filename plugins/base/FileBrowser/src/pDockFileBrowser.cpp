/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Kopats Andei aka hlamer <hlamer@tut.by>
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
#include <QLineEdit>
#include <QListView>
#include <QDirModel>
#include <QScrollArea>
#include <QTabWidget>
#include <QTreeView>
#include <QHeaderView>

pDockFileBrowser::pDockFileBrowser( QWidget* w )
	: QDockWidget( w ), mShown( false )
{
	// restrict areas
	setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

	QWidget* wdg = new QWidget (this);
	// vertical layout
	QVBoxLayout* v = new QVBoxLayout( wdg );
	v->setMargin( 5 );
	v->setSpacing( 3 );
	
	// horizontal layout for toolbutton && combobox
	QHBoxLayout* h = new QHBoxLayout;
	h->setMargin( 0 );
	h->setSpacing( 3 );
	
	// cdup toolbutton
	QToolButton* tbUp = new QToolButton;
	tbUp->setIcon( QIcon( ":/icons/up.png" ) );
	tbUp->setToolTip( tr( "Go Up" ) );
	h->addWidget( tbUp );
	
	// refresh toolbutton
	QToolButton* tbRefresh = new QToolButton;
	tbRefresh->setIcon( QIcon( ":/icons/reload.png" ) );
	tbRefresh->setToolTip( tr( "Refresh current view" ) );
	h->addWidget( tbRefresh );
	
	// combo drive
	mComboBox = new QComboBox;
	mComboBox->setToolTip( tr( "Select a drive" ) );
	h->addWidget( mComboBox );
	
	// add horizontal layout into vertical one
	v->addLayout( h );
	
	// lineedit
	mLineEdit = new QLineEdit;
	mLineEdit->setReadOnly( true );
	v->addWidget( mLineEdit );


	// central widget
	QTabWidget* tabs = new QTabWidget (this);
	setWidget( tabs );
	
	// folders view
	mDirs = new QListView;
	tabs->addTab (mDirs, tr("Dirs"));
	
	// dir model
	mDirsModel = new QDirModel;
	mDirsModel->setFilter( QDir::AllEntries | QDir::Readable | QDir::CaseSensitive | QDir::NoDotAndDotDot );
	mDirsModel->setSorting( QDir::DirsFirst | QDir::Name );
	
	// assign model to views
	mComboBox->setModel( mDirsModel );
	mDirs->setModel( mDirsModel );
	
	// if only one drive, disable it and root it ( linux/mac )
	if ( mComboBox->count() == 1 )
		mComboBox->setEnabled( false );
	
	// set root index
	mComboBox->setRootModelIndex( QModelIndex() );
	mDirs->setRootIndex( mDirsModel->index( mComboBox->currentText() ) );
	
	// set lineedit path
	mLineEdit->setText( mDirsModel->filePath( mDirs->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
	
	
	// files view
	mFiles = new QTreeView;
	// file model
	mFilesModel = new QDirModel;
	mFilesModel->setFilter( QDir::AllEntries | QDir::Readable | QDir::CaseSensitive | QDir::NoDotAndDotDot );
	mFilesModel->setSorting( QDir::DirsFirst | QDir::Name );
	
	// assign model to views
	mFiles->setModel( mFilesModel );
	lv_doubleClicked(mDirs->rootIndex ());
	mFiles->setColumnHidden (1, true);
	mFiles->setColumnHidden (2, true);
	mFiles->setColumnHidden (3, true);
	mFiles->header()->hide ();
	tabs->addTab (mFiles, tr("Files"));
	tabs->setCurrentIndex (1);
	
	v->addWidget (tabs);
	
	setWidget( wdg );
	
	// connections
	connect( tbUp, SIGNAL( clicked() ), this, SLOT( tbUp_clicked() ) );
	connect( tbRefresh, SIGNAL( clicked() ), this, SLOT( tbRefresh_clicked() ) );
	connect( mComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( cb_currentIndexChanged( const QString& ) ) );
	connect( mDirs, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( lv_doubleClicked( const QModelIndex& ) ) );
	connect( mFiles, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( tv_doubleClicked( const QModelIndex& ) ) );
}

pDockFileBrowser::~pDockFileBrowser()
{
	// delete model
	delete mDirsModel;
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
		emit restoreSettings();
	}
}

void pDockFileBrowser::hideEvent( QHideEvent* e )
{
	// default event
	QDockWidget::hideEvent( e );
	// save settings
	if ( mShown )
		emit saveSettings();
}

void pDockFileBrowser::tbUp_clicked()
{
	// cd up
	setCurrentPath ( mDirsModel->filePath( mDirs->rootIndex().parent() ) );
}

void pDockFileBrowser::tbRefresh_clicked()
{
	// refresh current parent folder
	mDirsModel->refresh( mDirs->rootIndex().parent() );
}

void pDockFileBrowser::lv_doubleClicked( const QModelIndex& i )
{
	// if dir, set root index to it
	setCurrentPath ( mDirsModel->filePath( i ) );

	// open file
	// set lineedit path
	mLineEdit->setText( mDirsModel->filePath( mDirs->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
	// select correct drive in combo if needed
	if ( QDir::drives().contains( mDirsModel->fileName( i ) ) )
		mComboBox->setCurrentIndex( mComboBox->findText( mDirsModel->fileName( i ).remove( -1, 1 ) ) );
}

void pDockFileBrowser::tv_doubleClicked( const QModelIndex& i )
{
	// if dir, set root index to it
	if ( ! mFilesModel->isDir( i ) ) //possible it's file
		pFileManager::instance()->openFile( mFilesModel->filePath( i ) );
}

void pDockFileBrowser::cb_currentIndexChanged( const QString& s )
{
	// move drive
	mDirs->setRootIndex( mDirsModel->index( s ) );
	// set lineedit path
	mLineEdit->setText( mDirsModel->filePath( mDirs->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
}

QString pDockFileBrowser::currentDrive() const
{
	// return current drive
	return mComboBox->currentText();
}

void pDockFileBrowser::setCurrentDrive( const QString& s )
{
	// set current drive
	mComboBox->setCurrentIndex( mComboBox->findText( s ) );
	// set lineedit path
	mLineEdit->setText( mDirsModel->filePath( mDirs->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
}

QString pDockFileBrowser::currentPath() const
{
	// return current path
	return mDirsModel->filePath( mDirs->rootIndex() );
}

void pDockFileBrowser::setCurrentPath( const QString& s )
{
	// set current path
	mDirs->setRootIndex( mDirsModel->index( s ) );
	mFiles->setRootIndex( mFilesModel->index( s ) );
	// set lineedit path
	mLineEdit->setText( mDirsModel->filePath( mDirs->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
}
