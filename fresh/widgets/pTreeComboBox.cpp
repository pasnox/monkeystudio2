/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : pTreeComboBox
** FileName  : pTreeComboBox.cpp
** Date      : 2007-09-28T14:20:27
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pTreeComboBox.h"

#include <QTreeView>
#include <QAbstractItemModel>
#include <QHeaderView>

pTreeComboBox::pTreeComboBox( QWidget* w )
	: QComboBox( w ), mView( 0 ), mModel( 0 )
{
	addItem( "toto" );
	setView( new QTreeView );
	mView->header()->hide();
	mView->resize( width(), 200 );
}

pTreeComboBox::~pTreeComboBox()
{ delete mView; }

void pTreeComboBox::hidePopup()
{
	if ( mView )
		mView->hide();
}

void pTreeComboBox::showPopup()
{
	if ( mView )
	{
		mView->expandAll();
		mView->move( mapToGlobal( geometry().topLeft() ) );
		mView->resize( width(), mView->height() );
		mView->show();
	}
}

QTreeView* pTreeComboBox::view() const
{ return mView; }

void pTreeComboBox::setView( QTreeView* t )
{
	if ( mView == t )
		return;
	delete mView;
	mView = t;
	//mView->setWindowFlags( Qt::Popup );
	setModel( mModel );
	connect( mView, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( internal_clicked( const QModelIndex& ) ) );
}

QAbstractItemModel* pTreeComboBox::model() const
{ return mModel; }

void pTreeComboBox::setModel( QAbstractItemModel* m )
{
	if ( mView && mView->model() != m )
	{
		mModel = m;
		mView->setModel( mModel );
		connect( mView->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( internal_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	}
}

QModelIndex pTreeComboBox::currentIndex() const
{ return mView ? mView->currentIndex() : QModelIndex(); }

void pTreeComboBox::setCurrentIndex( const QModelIndex& i )
{
	if ( mView )
		mView->setCurrentIndex( i );
}

void pTreeComboBox::internal_currentChanged( const QModelIndex& c, const QModelIndex& )
{
	setItemIcon( 0, c.data( Qt::DecorationRole ).value<QIcon>() );
	setItemText( 0, c.data( Qt::DisplayRole ).toString() );
	emit selected( c );
}

void pTreeComboBox::internal_clicked( const QModelIndex& i )
{
	emit clicked( i );
	hidePopup();
}
