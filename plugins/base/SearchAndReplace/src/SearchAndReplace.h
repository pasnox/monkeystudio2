/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Andrei KOPATS aka hlamer, <hlamer at tut by>
** Project   : Monkey Studio Base Plugins
** FileName  : SearchAndReplace.h
** Date      : 2008-01-14T00:39:59
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
	\file SearchAndReplace.h
	\date 
	\author Andrei KOPATS
	\brief Header file for SearchAndReplace plugin
*/
#ifndef SEARCHANDREPLACE_H
#define SEARCHANDREPLACE_H

#include <BasePlugin.h>

class SearchThread;
class SearchResultsDock;
class SearchWidget;

/*!
	Main class of the SearchAndReplace plugin
	Consists implementation of plugin-system interface, and functionality of plugin
*/
class SearchAndReplace : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	/*! Active mode. Plugin can be used for search/replace in file/dirrectory
	*/
	enum Mode
	{
		SEARCH_FILE = 0,
		REPLACE_FILE = 1,
#if 0
		SEARCH_PROJECT = 2,
		REPLACE_PROJECT = 3
#endif
		SEARCH_DIRECTORY = 4,
		REPLACE_DIRECTORY = 5
	};
	
	/*! Used for transfer results of search in dirrectory from search thread to dock,
		and, in replace in dirrectory mode, for apply replacements
	*/
	struct Occurence
	{
		Mode mode;				/*! Active mode. Can be SEARCH_DIRRECTORY or REPLACE_DIRRECTORY */
		QString fileName;		/*! Name of file, where occurence found. With full path */
		QPoint position;		/*! Position in the file x - column, y - line */
		QString codec;			/*! The text codec use to decode/encode the file */
		QString text;			/*! Text for display occurence on UI */
		QString fullText;		/*! Full text. For display on hint */
		QString searchText; 	/*! Search pattern, which was used for searching. Filled only for 'replace in dirrectory' */
		bool isRegExp; 			/*! Is search pattern regular expression. Filled only for 'replace in dirrectory' */
		bool isCaseSensetive; 	/*! Is search case sensetive. Filled only for 'replace in dirrectory' */
		bool checked;			/*! Do user checked occurence for replacement on search results dock */
	};
	
protected:	
	void fillPluginInfos();
public:
	SearchAndReplace();
	
	virtual bool setEnabled( bool );

protected:
	Mode mMode;
	SearchWidget* mWidget;
	SearchResultsDock* mDock;
	SearchThread* mSearchThread;
	
protected:

	bool isSearchTextValid ();
	bool isReplaceTextValid ();
	bool isPathValid ();
	bool isMaskValid ();
	
	void showMessage (const QString& status);
	void updateSearchTextOnUI ();
	
	QString eolForContent (const QString& content);
	bool searchFile (bool next, bool incremental, bool enableLooping);
	int replace(bool all);
	
	void replaceInDirectory();
	
protected slots:
	// from system
	void showSearchFile ();
	void showReplaceFile ();
	//void showSearchProject ();
	//void showReplaceProject ();
	void showSearchFolder ();
	void showReplaceFolder ();
	
	// from widget	
	void onPreviousClicked();
	void onNextClicked();
	void onReplaceClicked();
	void onReplaceAllClicked();
	void onSearchTextEdited();
	
	// from search results dock
	void makeGoTo (const QString& file, const QPoint& position, const QString& codec);
	
	// from search thread
	void threadFinished ();
	void readThreadResults ();
};

#endif