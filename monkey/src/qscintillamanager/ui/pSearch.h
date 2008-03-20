/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pSearch.h
** Date      : 2008-01-14T00:37:03
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
#ifndef PSEARCH_H
#define PSEARCH_H

#include <QWidget>
#include <QStringList>

#include "MonkeyExport.h"
#include "pConsoleManager.h"
#include "pDockWidget.h"

class QsciScintilla;
class SearchThread;

class QGridLayout;
class QToolButton;
class QPushButton;
class QLineEdit;
class QLabel;
class QCheckBox;
class QComboBox;

class Q_MONKEY_EXPORT pSearch : public QWidget
{
	Q_OBJECT
	friend class MonkeyCore;
	friend class pWorkspace;
	
private:
	enum OperType 
	{
		SEARCH = 0, 
		REPLACE = 1
	};
	enum WhereType
	{
		FILE = 0,
		//PROJECT = 1, 
		FOLDER = 2
	};

	OperType mOperType;
	WhereType mWhereType;
	
	// Used for counting of occurences when searching in folder/project
	int mOccurencesFinded;
	int mFilesProcessed;
	pDockWidget* fake;
	
public slots:
	void showSearchFile ();
	void showReplaceFile ();
	//void showSearchProject ();
	//void showReplaceProject ();
	void showSearchFolder ();
	void showReplaceFolder ();


	bool on_tbPrevious_clicked();
	bool on_tbNext_clicked();
	void on_tbReplace_clicked();
	void on_tbReplaceAll_clicked();

protected slots:
	void threadFinished ();
	// slot calling, when thread emits search result
	void occurenceFinded ();
	// slog calling when thread processed file. 
	// Parameter - total count of files
	void fileProcessed (int);

	void on_tbPath_clicked ();

protected:

	QGridLayout* layout;

	//search
	QLabel* lSearchText;
	QComboBox* cobSearch; // FIXME make protected

	QPushButton* tbNext;
	QPushButton* tbPrevious;
	QCheckBox* cbCaseSensitive;
	QCheckBox* cbRegExp;

	//replace
	QLabel* lReplaceText;
	QComboBox* cobReplace;
	QPushButton* tbReplace;
	QPushButton* tbReplaceAll;

	//folder
	QLabel* lPath;
	QComboBox* cobPath;
	QToolButton* tbPath;
	QLabel* lMask;
	QComboBox* cobMask;

	void addSearchToLayout (int row);
	void addReplaceToLayout (int row);
	void addFolderToLayout (int row);

	void removeSearchFromLayout ();
	void removeReplaceFromLayout ();
	void removeFolderFromLayout ();

	bool isEditorAvailible ();
	bool isProjectAvailible ();

	SearchThread* mSearchThread;

	/* Search text in the file. 
	 * Search Next, if next==true;  Search Previous, if next = false
	 * Continue from start, when end riched, if wrap = true;
     */
	bool searchFile (bool next, bool wrap = true);

	int replace (bool all);

	pSearch( QWidget* parent = 0 );
	virtual ~pSearch(){};
	void keyPressEvent( QKeyEvent* );

	void show ();
    
    void showMessage (QString status);
	
	QStringList searchRecents;
	QStringList replaceRecents;
	QStringList maskRecents;
	QStringList pathRecents;
	
	void searchAddToRecents (QString);
	void replaceAddToRecents (QString);
	void pathAddToRecents (QString);
	void maskAddToRecents (QString);

signals:
	void clearSearchResults ();

};

#endif // PSEARCH_H
