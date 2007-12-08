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

	// central widget
	QWidget* wdg = new QWidget( this );
	setWidget( wdg );
	
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
	
	// dir model
	mDirsModel = new QDirModel( this );
	mDirsModel->setFilter( QDir::AllEntries | QDir::Readable | QDir::CaseSensitive | QDir::NoDotAndDotDot );
	mDirsModel->setSorting( QDir::DirsFirst | QDir::Name );
	
	// tabwidget
	QTabWidget* tabs = new QTabWidget;
	v->addWidget( tabs );
	
	// folders view
	mDirs = new QListView;
	tabs->addTab( mDirs, tr( "Dirs" ) );
	
	// files view
	mFiles = new QTreeView;
	tabs->addTab( mFiles, tr( "Files" ) );
	
	// assign model to views
	mComboBox->setModel( mDirsModel );
	mDirs->setModel( mDirsModel );
	mFiles->setModel( mDirsModel );
	
	// custom view
	mFiles->setColumnHidden( 1, true );
	mFiles->setColumnHidden( 2, true );
	mFiles->setColumnHidden( 3, true );
	mFiles->header()->hide();
	
	// if only one drive, disable it and root it ( linux/mac )
	if ( mComboBox->count() == 1 )
		mComboBox->setEnabled( false );
	
	// set root index
	mComboBox->setRootModelIndex( QModelIndex() );
	mDirs->setRootIndex( mDirsModel->index( mComboBox->currentText() ) );
	mFiles->setRootIndex( mDirs->rootIndex() );
	
	// set lineedit path
	mLineEdit->setText( mDirsModel->filePath( mDirs->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
	
	// set page 1 visible
	tabs->setCurrentIndex( 1 );
	
	// connections
	connect( tbUp, SIGNAL( clicked() ), this, SLOT( tbUp_clicked() ) );
	connect( tbRefresh, SIGNAL( clicked() ), this, SLOT( tbRefresh_clicked() ) );
	connect( mComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( cb_currentIndexChanged( const QString& ) ) );
	connect( mDirs, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( lv_doubleClicked( const QModelIndex& ) ) );
	connect( mFiles, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( tv_doubleClicked( const QModelIndex& ) ) );
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
	setCurrentPath( mDirsModel->filePath( mDirs->rootIndex().parent() ) );
}

void pDockFileBrowser::tbRefresh_clicked()
{
	// refresh current parent folder
	mDirsModel->refresh( mDirs->rootIndex().parent() );
}

void pDockFileBrowser::lv_doubleClicked( const QModelIndex& i )
{
	if ( mDirsModel->isDir( i ) )
	{
		// if dir, set root index to it
		setCurrentPath( mDirsModel->filePath( i ) );
		// select correct drive in combo if needed
		if ( QDir::drives().contains( mDirsModel->fileName( i ) ) )
			mComboBox->setCurrentIndex( mComboBox->findText( mDirsModel->fileName( i ).remove( -1, 1 ) ) );
	}
	else
		pFileManager::instance()->openFile( mDirsModel->filePath( i ) );
}

void pDockFileBrowser::tv_doubleClicked( const QModelIndex& i )
{
	// open file
	if ( !mDirsModel->isDir( i ) )
		pFileManager::instance()->openFile( mDirsModel->filePath( i ) );
}

void pDockFileBrowser::cb_currentIndexChanged( const QString& s )
{
	// set current path
	setCurrentPath( s );
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
	mFiles->setRootIndex( mDirsModel->index( s ) );
	// set lineedit path
	mLineEdit->setText( mDirsModel->filePath( mDirs->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
}
