/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Andrei KOPATS aka hlamer <hlamer@tut.by>
** Project   : Monkey Studio Base Plugins
** FileName  : SearchResultsDock.cpp
** Date      : 
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
	\file SearchResultsDock.cpp
	\date 
	\author Andrei KOPATS aka hlamer <hlamer@tut.by>
	\brief Dock with search results
*/
#include "SearchResultsDock.h"

#include <QTreeWidget>

/*!
	Create UI
	\param w Pointer to parent widget
*/
SearchResultsDock::SearchResultsDock( QWidget* w )
	: pDockWidget( w )
{
	// restrict areas
	setAllowedAreas( Qt::BottomDockWidgetArea );
	
	layout()->setMargin( 0 );
	
	// files view
	mTree = new QTreeWidget;
	setWidget ( mTree );
	
	mTree->setHeaderHidden( true );
	
	connect( mTree, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( itemPressed( QTreeWidgetItem* ) ) );
	
	// NOTE we sometimes disconnect this signal
	connect( mTree, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( itemChanged( QTreeWidgetItem* ) ) );
}

/*!
	Remove all items from search results
	
	Remove all items from search results
*/
void SearchResultsDock::clearSearchResults()
{
	mTree->clear();
}

/*!
	Append search result to Search Results dock
	
	Search result representing by same structure, as Build Step
	\param s Search result to append
*/
void SearchResultsDock::appendSearchResult( const pConsoleManager::Step& s )
{
	disconnect( mTree, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( itemChanged( QTreeWidgetItem* ) ) );
	
	QTreeWidgetItem* it = NULL;
	QTreeWidgetItem* parentItem = NULL;
	if (s.mType == pConsoleManager::stResultForReplace)
	{
		QString parentItemFile = QString::null;
		if (mTree->topLevelItemCount())
		{
			parentItem = mTree->topLevelItem(mTree->topLevelItemCount()-1);
			parentItemFile = parentItem->data (0, Qt::UserRole + 2).toString();
		}
		if (s.mFileName != parentItemFile) // it's a next file, need create next item (or it's just a first search result)
		{
			parentItem = new QTreeWidgetItem (mTree);
			parentItem->setData( 0, FILE_NAME, s.mFileName ); // filename
			parentItem->setFlags (parentItem->flags() | Qt::ItemIsUserCheckable);
			parentItem->setCheckState (0, Qt::Checked);
		}
		it = new QTreeWidgetItem( parentItem );
	}
	else
	{
		it = new QTreeWidgetItem( mTree );
	}
	
	if (parentItem) // stResultForReplace
		parentItem->setText (0, QString ("%1 (%2)").arg (s.mFileName).arg(parentItem->childCount()));
	
	// set item infos
	it->setText( 0, s.mText );
	it->setToolTip( 0, s.mFullText );
	it->setData( 0, FILE_NAME, s.mFileName ); // filename
	it->setData( 0, POSITION, s.mPosition ); // position
	
	if (s.mType == pConsoleManager::stResultForReplace)
	{
		it->setFlags (it->flags() | Qt::ItemIsUserCheckable);
		it->setCheckState (0, Qt::Checked);
	}
	
	connect( mTree, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( itemChanged( QTreeWidgetItem* ) ) );
}

/*!
	Handler of pressing on item in the Search Results dock

	Opens file/line in the editor
	\param it Item, which was pressed
*/
void SearchResultsDock::itemPressed ( QTreeWidgetItem* it )
{
	// get filename
	//qDebug () << "pressed";
	QString s = it->data( 0, FILE_NAME ).toString();
	QPoint position = it->data( 0, POSITION ).toPoint();
	//qDebug () << s << position;
	emit resultActivated (s, position);
}

/*!
	Handler of change of item. Will be executed, when user will check/uncheck item

	Need check/uncheck children according with parent, of check-uncheck parent according with state of children
*/
void SearchResultsDock::itemChanged( QTreeWidgetItem* it )
{
	disconnect( mTree, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( twSearchResults_itemChanged( QTreeWidgetItem* ) ) );
	if (it->childCount()) // It's a parent item (File name). Need update children checked state
	{
		for (int i = 0; i < it->childCount(); i++)
		{
			it->child (i)->setCheckState (0, it->checkState (0));
		}
	}
	else // It's a child (occurence). Need update parent ceched state
	{
		QTreeWidgetItem* parent = it->parent ();
		if (parent)
		{
			bool haveChecked = false;
			bool haveUnChecked = false;
			for (int i = 0; i < parent->childCount(); i++)
			{
				if (parent->child(i)->checkState (0) == Qt::Checked)
				{
					haveChecked = true;
				}
				else
				{
					haveUnChecked = true;
				}
			}
			if (haveChecked && ! haveUnChecked)
				parent->setCheckState (0, Qt::Checked);
			else if (! haveChecked && haveUnChecked)
				parent->setCheckState (0, Qt::Unchecked);
			else // both
				parent->setCheckState (0, Qt::PartiallyChecked);
		}
	}
	connect( mTree, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( itemChanged( QTreeWidgetItem* ) ) );
}
