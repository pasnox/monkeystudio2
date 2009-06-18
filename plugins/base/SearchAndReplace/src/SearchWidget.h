/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : SearchWidget.h
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
#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QStringList>

#include "SearchAndReplace.h"

#include <fresh.h>

class QsciScintilla;
class SearchThread;

class QGridLayout;
class QToolButton;
class QPushButton;
class QLineEdit;
class QLabel;
class QCheckBox;
class QComboBox;

/*!
	Class implements UI of search system
*/
class SearchWidget : public QWidget
{
	Q_OBJECT
	
public:
	enum LineEditColor 
	{
		GREEN,
		RED,
		DEFAULT
	};
	
	SearchWidget( QWidget* parent = NULL);

	void show (SearchAndReplace::Mode mode);
	
	void setSearchText (const QString& text);
	void setNextButtonText (const QString& text);
	void setNextButtonIcon (const QIcon& icon);
	
	bool isRegExp ();
	bool isCaseSensetive ();
	
	QString searchText();
	QString replaceText();
	QString path();
	QString mask();
	
	void searchAddToRecents (QString);
	void replaceAddToRecents (QString);
	void pathAddToRecents (QString);
	void maskAddToRecents (QString);
	
	void setSearchLineEditColor (LineEditColor);
protected:
	SearchAndReplace::Mode mMode;
	
	QColor mDefaultEditColor;
	
	QGridLayout* layout;

	//search
	QLabel* lSearchText;
	QComboBox* cobSearch;

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

	QStringList searchRecents;
	QStringList replaceRecents;
	QStringList maskRecents;
	QStringList pathRecents;
	
	void addSearchToLayout (int row);
	void addReplaceToLayout (int row);
	void addFolderToLayout (int row);

	void removeSearchFromLayout ();
	void removeReplaceFromLayout ();
	void removeFolderFromLayout ();

	void keyPressEvent( QKeyEvent* );

protected slots:
	void onPathClicked ();
	
signals:
	bool previousClicked();
	bool nextClicked();
	void replaceClicked();
	void replaceAllClicked();
	
	void searchTextEdited();
};

#endif // SEARCHWIDGET_H
