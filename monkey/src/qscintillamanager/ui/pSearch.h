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

#include "MonkeyExport.h"
#include "ui_pSearch.h"

#include <QPointer>

class QsciScintilla;

class Q_MONKEY_EXPORT pSearch : public pDockWidget, public Ui::pSearch
{
	Q_OBJECT
	friend class MonkeyCore;
	
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
	
public:
	QsciScintilla* editor() const;
	void setEditor( QsciScintilla* );

public slots:
	void showSearchFile ();
	void showReplaceFile ();
	//void showSearchProject ();
	//void showReplaceProject ();
	void showSearchFolder ();
	void showReplaceFolder ();


	bool on_tbPrevious_clicked();
	bool on_tbNext_clicked();
	bool on_tbReplace_clicked();
	void on_tbReplaceAll_clicked();

protected:
	pSearch( QsciScintilla* = 0 );
	bool checkEditor();
	void keyPressEvent( QKeyEvent* );

	void show ();

	QPointer<QsciScintilla> mEditor;

};

#endif // PSEARCH_H
