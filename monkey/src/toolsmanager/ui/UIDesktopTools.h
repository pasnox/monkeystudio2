/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIDesktopTools.h
** Date      : 2007-11-04T22:46:47
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
