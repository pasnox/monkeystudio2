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
#include "pTreeComboBox.h"

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
	: pDockWidget( w ), mShown( false )
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
	mCombo = new pTreeComboBox;
	mCombo->setToolTip( tr( "Quick Navigation" ) );
	h->addWidget( mCombo );
	
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
	mList = new QListView;
	tabs->addTab( mList, tr( "List View" ) );
	
	// files view
	mTree = new QTreeView;
	tabs->addTab( mTree, tr( "Tree View" ) );
	
	// assign model to views
	mCombo->setModel( mDirsModel );
	mList->setModel( mDirsModel );
	mTree->setModel( mDirsModel );
	
	// custom view
	mCombo->view()->setColumnHidden( 1, true );
	mCombo->view()->setColumnHidden( 2, true );
	mCombo->view()->setColumnHidden( 3, true );
	mTree->setColumnHidden( 1, true );
	mTree->setColumnHidden( 2, true );
	mTree->setColumnHidden( 3, true );
	mTree->header()->hide();
	
	// set root index
#ifndef Q_OS_WIN
	mCombo->setRootIndex( mDirsModel->index( "/" ) );
#else
	mCombo->setRootIndex( QModelIndex() );
#endif
	
	// set lineedit path
	setCurrentPath( mDirsModel->filePath( mCombo->rootIndex() ) );
	
	// set page 1 visible
	tabs->setCurrentIndex( 1 );
	
	// connections
	connect( tbUp, SIGNAL( clicked() ), this, SLOT( tbUp_clicked() ) );
	connect( tbRefresh, SIGNAL( clicked() ), this, SLOT( tbRefresh_clicked() ) );
	connect( mCombo, SIGNAL( currentChanged( const QModelIndex& ) ), this, SLOT( cb_currentChanged( const QModelIndex& ) ) );
	connect( mList, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( lv_doubleClicked( const QModelIndex& ) ) );
	connect( mTree, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( tv_doubleClicked( const QModelIndex& ) ) );
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
	// cd up only if not the root index
	if ( mCombo->currentIndex() != mCombo->rootIndex() )
		setCurrentPath( mDirsModel->filePath( mCombo->currentIndex().parent() ) );
}

void pDockFileBrowser::tbRefresh_clicked()
{
	// refresh current parent folder
	mDirsModel->refresh( mCombo->currentIndex() );
}

void pDockFileBrowser::lv_doubleClicked( const QModelIndex& i )
{
	if ( mDirsModel->isDir( i ) )
		setCurrentPath( mDirsModel->filePath( i ) );
	else
		pFileManager::instance()->openFile( mDirsModel->filePath( i ) );
}

void pDockFileBrowser::tv_doubleClicked( const QModelIndex& i )
{
	// open file
	if ( !mDirsModel->isDir( i ) )
		pFileManager::instance()->openFile( mDirsModel->filePath( i ) );
}

void pDockFileBrowser::cb_currentChanged( const QModelIndex& i )
{
	// set current path
	setCurrentPath( mDirsModel->filePath( i ) );
}

QString pDockFileBrowser::currentPath() const
{
	// return current path
	return mDirsModel->filePath( mCombo->currentIndex() );
}

void pDockFileBrowser::setCurrentPath( const QString& s )
{
	// get index
	QModelIndex i = mDirsModel->index( s );
	// set current path
	mCombo->setCurrentIndex( i );
	mList->setRootIndex( i );
	mTree->setRootIndex( i );
	// set lineedit path
	mLineEdit->setText( mDirsModel->filePath( i ) );
	mLineEdit->setToolTip( mLineEdit->text() );
}
