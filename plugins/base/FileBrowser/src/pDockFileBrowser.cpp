/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : pDockFileBrowser.cpp
** Date      : 2008-01-14T00:39:57
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
#include "pDockFileBrowser.h"
#include "pFileManager.h"
#include "pMonkeyStudio.h"
#include "pTreeComboBox.h"
#include "pSettings.h"
#include "MonkeyCore.h"

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
		MonkeyCore::fileManager()->openFile( mDirsModel->filePath( index ) );
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
