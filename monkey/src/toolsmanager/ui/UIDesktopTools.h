/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UIDesktopTools.h
** Date      : 2008-01-14T00:37:15
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
#ifndef UIDesktopTools_H
#define UIDesktopTools_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "ui_UIDesktopTools.h"
#include "pDesktopApplications.h"

class QTreeWidgetItem;
class pDesktopFolder;

class Q_MONKEY_EXPORT UIDesktopTools : public QDialog, public Ui::UIDesktopTools, public QSingleton<UIDesktopTools>
{
	Q_OBJECT
	friend class QSingleton<UIDesktopTools>;

private:
	UIDesktopTools( QWidget* = QApplication::activeWindow() );
	~UIDesktopTools();

protected:
	pDesktopApplications mStartMenu;

	void showEvent( QShowEvent* );
	void closeEvent( QCloseEvent* );

protected slots:
	void populateTree( QTreeWidgetItem* i, pDesktopFolder* f );
	void populateList();
	void on_leNameFilter_returnPressed();
	void on_leCategoriesFilters_returnPressed();
	void on_tbRight_clicked();
	void on_tbLeft_clicked();
	void on_pbUp_clicked();
	void on_pbDown_clicked();
	void accept();

};

#endif // UIDesktopTools_H
