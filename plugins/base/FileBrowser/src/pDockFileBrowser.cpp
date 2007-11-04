/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pDockFileBrowser.cpp
** Date      : 2007-11-04T22:50:37
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
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
	
	// if only one drive, disable it and root it ( linux/mac )
	if ( mComboBox->count() == 1 )
		mComboBox->setEnabled( false );
	
	// set root index
	mComboBox->setRootModelIndex( QModelIndex() );
	mListView->setRootIndex( mDirModel->index( mComboBox->currentText() ) );
	
	// set lineedit path
	mLineEdit->setText( mDirModel->filePath( mListView->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
	
	// connections
	connect( tbUp, SIGNAL( clicked() ), this, SLOT( tbUp_clicked() ) );
	connect( tbRefresh, SIGNAL( clicked() ), this, SLOT( tbRefresh_clicked() ) );
	connect( mComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( cb_currentIndexChanged( const QString& ) ) );
	connect( mListView, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( lv_doubleClicked( const QModelIndex& ) ) );
}

pDockFileBrowser::~pDockFileBrowser()
{
	// delete model
	delete mDirModel;
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
	mListView->setRootIndex( mListView->rootIndex().parent() );
	// set lineedit path
	mLineEdit->setText( mDirModel->filePath( mListView->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
}

void pDockFileBrowser::tbRefresh_clicked()
{
	// refresh current parent folder
	mDirModel->refresh( mListView->rootIndex().parent() );
}

void pDockFileBrowser::lv_doubleClicked( const QModelIndex& i )
{
	// if dir, set root index to it
	if ( mDirModel->isDir( i ) )
		mListView->setRootIndex( i );
	// open file
	else
		pFileManager::instance()->openFile( mDirModel->filePath( i ) );
	// set lineedit path
	mLineEdit->setText( mDirModel->filePath( mListView->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
	// select correct drive in combo if needed
	if ( QDir::drives().contains( mDirModel->fileName( i ) ) )
		mComboBox->setCurrentIndex( mComboBox->findText( mDirModel->fileName( i ).remove( -1, 1 ) ) );
}

void pDockFileBrowser::cb_currentIndexChanged( const QString& s )
{
	// move drive
	mListView->setRootIndex( mDirModel->index( s ) );
	// set lineedit path
	mLineEdit->setText( mDirModel->filePath( mListView->rootIndex() ) );
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
	mLineEdit->setText( mDirModel->filePath( mListView->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
}

QString pDockFileBrowser::currentPath() const
{
	// return current path
	return mDirModel->filePath( mListView->rootIndex() );
}

void pDockFileBrowser::setCurrentPath( const QString& s )
{
	// set current path
	mListView->setRootIndex( mDirModel->index( s ) );
	// set lineedit path
	mLineEdit->setText( mDirModel->filePath( mListView->rootIndex() ) );
	mLineEdit->setToolTip( mLineEdit->text() );
}

