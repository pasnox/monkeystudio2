/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer  <hlamer@tut.by>
** Project   : 
** FileName  : pStringListEditor.cpp
** Date      : 2007-11-08T14:57:05
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pStringListEditor.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QListWidget>

pStringListEditor::pStringListEditor( QWidget* p, const QString& t )
	: QGroupBox( t, p )
{
	// create layout
    QVBoxLayout* vbox = new QVBoxLayout( this );
	vbox->setMargin( 5 );
	vbox->setSpacing( 3 );
	
	// create toolbar
	QToolBar* tb = new QToolBar;
	tb->layout()->setMargin( 0 );
	tb->layout()->setSpacing( 0 );
	tb->setIconSize( QSize( 16, 16 ) );
	vbox->addWidget( tb );
	
	// create listwidget
	mList = new QListWidget;
	mList->setMinimumHeight( 40 );
    vbox->addWidget( mList );
	
	// create actions
	QAction* aAdd = new QAction( QIcon( ":/listeditor/icons/listeditor/add.png" ), tr( "Add Item" ), tb );
	QAction* aRemove = new QAction( QIcon( ":/listeditor/icons/listeditor/remove.png" ), tr( "Remove Item" ), tb );
	QAction* aClear = new QAction( QIcon( ":/listeditor/icons/listeditor/clear.png" ), tr( "Clear Items" ), tb );
	QAction* aUp = new QAction( QIcon( ":/listeditor/icons/listeditor/up.png" ), tr( "Move Item Up" ), tb );
	QAction* aDown = new QAction( QIcon( ":/listeditor/icons/listeditor/down.png" ), tr( "Move Item Down" ), tb );
	QAction* aEdit = new QAction( QIcon( ":/listeditor/icons/listeditor/edit.png" ), tr( "Edit Item" ), tb );
	
	// add actions to toolbar
	tb->addAction( aAdd );
	tb->addAction( aRemove );
	tb->addAction( aClear );
	tb->addAction( aUp );
	tb->addAction( aDown );
	tb->addAction( aEdit );
	
	// connections
	connect( aAdd, SIGNAL( triggered() ), this, SLOT( onAddItem() ) );
	connect( aRemove, SIGNAL( triggered() ), this, SLOT( onRemoveItem() ) );
	connect( aClear, SIGNAL( triggered() ), this, SLOT( onClearItem() ) );
	connect( aUp, SIGNAL( triggered() ), this, SLOT( onMoveUpItem() ) );
	connect( aDown, SIGNAL( triggered() ), this, SLOT( onMoveDownItem() ) );
	connect( aEdit, SIGNAL( triggered() ), this, SLOT( onEditItem() ) );
	connect( mList, SIGNAL( itemChanged( QListWidgetItem* ) ), this, SIGNAL( edited() ) );
}

void pStringListEditor::setValues( const QStringList& l )
{
	mList->clear();
	foreach ( QString s, l )
	{
		QListWidgetItem* it = new QListWidgetItem( s, mList );
		it->setFlags( it->flags() | Qt::ItemIsEditable );
		mList->setCurrentItem( it );
		mList->scrollToItem( it );
	}
	emit edited();
}

QStringList pStringListEditor::values() const
{
	QStringList l;
	foreach ( QListWidgetItem* it, mList->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->text();
	return l;
}

void pStringListEditor::onAddItem()
{
	QListWidgetItem* it = new QListWidgetItem( tr( "New item" ), mList );
	it->setFlags( it->flags() | Qt::ItemIsEditable );
	mList->setCurrentItem( it );
	mList->scrollToItem( it );
	mList->editItem( it );
	emit edited();
}

void pStringListEditor::onRemoveItem()
{
	if ( QListWidgetItem* it = mList->selectedItems().value( 0 ) )
	{
		delete it;
		emit edited();
	}
}

void pStringListEditor::onClearItem()
{
	if ( mList->count() )
	{
		mList->clear();
		emit edited();
	}
}

void pStringListEditor::onMoveUpItem()
{
	if ( QListWidgetItem* it = mList->selectedItems().value( 0 ) )
	{
		int i = mList->row( it );
		if ( i != 0 )
			mList->insertItem( i -1, mList->takeItem( i ) );
		mList->setCurrentItem( it );
		emit edited();
	}
}

void pStringListEditor::onMoveDownItem()
{
	if ( QListWidgetItem* it = mList->selectedItems().value( 0 ) )
	{
		int i = mList->row( it );
		if ( i != mList->count() -1 )
			mList->insertItem( i +1, mList->takeItem( i ) );
		mList->setCurrentItem( it );
		emit edited();
	}
}

void pStringListEditor::onEditItem()
{
	if ( QListWidgetItem* it = mList->selectedItems().value( 0 ) )
		mList->editItem( it );
}
