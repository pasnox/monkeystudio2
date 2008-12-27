/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Andrei KOPATS aka hlamer <hlamer@tut.by>
** Project   : Monkey Studio Base Plugins
** FileName  : SearchResultsDock.h
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
	\file SearchResultsDock.h
	\date 
	\author Andrei KOPATS aka hlamer <hlamer@tut.by>
	\brief Search results dock
*/

#ifndef SEARCHRESULTSDOCK_H
#define SEARCHRESULTSDOCK_H

#include <fresh.h>
#include "SearchAndReplace.h"

class QTreeWidget;

/*!
	Search results dock
	
	Can display results of searching in the dirrectory
	For replace function, results can be checkable
*/
class SearchResultsDock : public pDockWidget
{
	Q_OBJECT
	enum DataRole
	{
		FILE_NAME = Qt::UserRole +1,
		POSITION,
		// for replacement
		SEARCH_TEXT,
		IS_REG_EXP,
		IS_CASE_SENSETIVE,
		REPLACE_TEXT
	};
	
public:
	SearchResultsDock( QWidget* parent = 0 );
	
	void clearSearchResults();
	
	int filesWithOccurencesCount () const;
	int oCcurencesCount (int fileIndex) const;
	SearchAndReplace::Occurence occurence (int fileIndex, int occurenceIndex) const;
	
protected:
	QTreeWidget* mTree;

signals:
	void resultActivated (const QString& fileName, const QPoint& position);
	
public slots:
	void appendSearchResult( const SearchAndReplace::Occurence& );
	
protected slots:
	void itemPressed( QTreeWidgetItem* );
	void itemChanged( QTreeWidgetItem* );
};

#endif // SEARCHRESULTSDOCK_H
