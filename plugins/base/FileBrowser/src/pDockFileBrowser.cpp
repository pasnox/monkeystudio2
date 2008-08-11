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
/*!
	\file pDockFileBrowser.cpp
	\date 2008-01-14T00:40:08
	\author Filipe AZEVEDO, Andrei KOPATS
	\brief UI of FileBrowser plugin
*/
#include "pDockFileBrowser.h"

#include <workspacemanager.h>
#include <coremanager.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QComboBox>
#include <QLineEdit>
#include <QListView>
#include <QFileSystemModel>
#include <QScrollArea>
#include <QTabWidget>
#include <QTreeView>
#include <QHeaderView>

/*!
	Create UI
	\param w Pointer to parent widget
*/
pDockFileBrowser::pDockFileBrowser( QWidget* w )
	: pDockWidget( w )
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
	mDirsModel = new QFileSystemModel( this );
	mDirsModel->setNameFilterDisables( false );
	mDirsModel->setFilter( QDir::AllDirs | QDir::AllEntries | QDir::CaseSensitive | QDir::NoDotAndDotDot );
	mDirsModel->setRootPath( QString::null ); // need to be called to reset the model
	
	// create proxy model
	mFilteredModel = new FileBrowserFilteredModel( this );
	mFilteredModel->setSourceModel( mDirsModel );
	
	// files view
	mTree = new QTreeView;
	v->addWidget( mTree );
	
	// assign model to views
	mCombo->setModel( mDirsModel );
	mTree->setModel( mFilteredModel );
	
	// custom view
	mCombo->view()->setColumnHidden( 1, true );
	mCombo->view()->setColumnHidden( 2, true );
	mCombo->view()->setColumnHidden( 3, true );
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
	connect( tbRoot, SIGNAL( clicked() ), this, SLOT( tbRoot_clicked() ) );
	connect( mCombo, SIGNAL( currentChanged( const QModelIndex& ) ), this, SLOT( cb_currentChanged( const QModelIndex& ) ) );
	connect( mTree, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( tv_doubleClicked( const QModelIndex& ) ) );
}

/*!
	Handler of click on Up button.

	Moves root of tree up one level
*/
void pDockFileBrowser::tbUp_clicked()
{
	// cd up only if not the root index
	if ( mCombo->currentIndex() != mCombo->rootIndex() )
		setCurrentPath( mDirsModel->filePath( mCombo->currentIndex().parent() ) );
}

/*!
	Handler of click on Root button. 
	
	If there are selected dirrectory in the tree - it will be set as root
*/
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

/*!
	Handler of click on item in the tree
	
	If there are file doubleclicked - it will be opened
	\param idx Index of clicked tree
*/
void pDockFileBrowser::tv_doubleClicked( const QModelIndex& idx )
{
	// open file corresponding to index
	const QModelIndex index = mFilteredModel->mapToSource( idx );
	if ( !mDirsModel->isDir( index ) )
		MonkeyCore::fileManager()->openFile( mDirsModel->filePath( index ) );
}

/*!
	Handler of changing of current dirrectory in combo box. Opens dirrectory 
	in the tree.
	\param i Index of clicked item in the combo box
*/
void pDockFileBrowser::cb_currentChanged( const QModelIndex& i )
{ setCurrentPath( mDirsModel->filePath( i ) ); }

/*!
	Get current path (root of the tree)
	\return Current path (root of the tree)
*/
QString pDockFileBrowser::currentPath() const
{ return mDirsModel->filePath( mCombo->currentIndex() ); }

/*!
	Set current path (root of the tree)
	\param s New path
*/
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

/*!
	Get filter wildcards, which currently using for filtering out unneeded file
	names from tree
	\return List if wildcards for filtering
*/
QStringList pDockFileBrowser::filters() const
{ return mFilteredModel->filters(); }

/*!
	Set filter wildcards for filtering out unneeded files
	\param filters List of wildcards
*/
void pDockFileBrowser::setFilters( const QStringList& filters )
{ mFilteredModel->setFilters( filters ); }
