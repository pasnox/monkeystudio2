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
#include "pSettings.h"

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

bool pDockFileBrowser::FilteredModel::filterAcceptsRow( int row, const QModelIndex& parent ) const
{
	if ( parent == QModelIndex() ) 
		return true;
	foreach ( const QString s, mWildcards )
		if ( QRegExp( s, Qt::CaseSensitive, QRegExp::Wildcard ).exactMatch( parent.child( row, 0 ).data().toString() ) )
				return false;
	return true;
}

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
	
	// set current path button
	QToolButton* tbRoot = new QToolButton;
	tbRoot->setIcon( QIcon( ":/icons/goto.png" ) );
	tbRoot->setToolTip( tr( "Set selected item as root" ) );
	h->addWidget( tbRoot );
	
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
	
	mFilteredModel = new FilteredModel ( this );
	mFilteredModel->setSourceModel( mDirsModel );
	
	// files view
	mTree = new QTreeView;
	v->addWidget ( mTree );
	
	// assign model to views
	mCombo->setModel( mDirsModel );
	mTree->setModel( mFilteredModel );
	
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
	
	// connections
	connect( tbUp, SIGNAL( clicked() ), this, SLOT( tbUp_clicked() ) );
	connect( tbRefresh, SIGNAL( clicked() ), this, SLOT( tbRefresh_clicked() ) );
	connect( tbRoot, SIGNAL( clicked() ), this, SLOT( tbRoot_clicked() ) );
	connect( mCombo, SIGNAL( currentChanged( const QModelIndex& ) ), this, SLOT( cb_currentChanged( const QModelIndex& ) ) );
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

void pDockFileBrowser::tbRoot_clicked()
{
	// seet root of model to path of selected item
	QModelIndex index = mTree->selectionModel()->selectedIndexes().value( 0 );
	if ( !index.isValid() )
		return;
	index = mFilteredModel->mapToSource( index );
	if ( !mDirsModel->isDir( index ) )
		index = index.parent();
	setCurrentPath( mDirsModel->filePath( index ) );
}

void pDockFileBrowser::tv_doubleClicked( const QModelIndex& i )
{
	// open file corresponding to index
	QModelIndex index = mFilteredModel->mapToSource( i );
	// open file
	if ( !mDirsModel->isDir( index ) )
		pFileManager::instance()->openFile( mDirsModel->filePath( index ) );
}

void pDockFileBrowser::cb_currentChanged( const QModelIndex& i )
{ setCurrentPath( mDirsModel->filePath( i ) ); }

QString pDockFileBrowser::currentPath() const
{ return mDirsModel->filePath( mCombo->currentIndex() ); }

void pDockFileBrowser::setCurrentPath( const QString& s )
{
	// get index
	QModelIndex index = mDirsModel->index( s );
	// set current path
	mCombo->setCurrentIndex( index );
	mFilteredModel->invalidate();
	mTree->setRootIndex( mFilteredModel->mapFromSource( index ) );
	// set lineedit path
	mLineEdit->setText( mDirsModel->filePath( index ) );
	mLineEdit->setToolTip( mLineEdit->text() );
}

QStringList pDockFileBrowser::wildcards() const
{ return mFilteredModel->mWildcards; }

void pDockFileBrowser::setWildcards( const QStringList& l )
{
	const QString s = currentPath();
	mFilteredModel->mWildcards = l;
	mFilteredModel->invalidate();
	setCurrentPath( s );
}
