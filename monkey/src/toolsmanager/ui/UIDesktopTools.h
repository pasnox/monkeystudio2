/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox
** Project   : UIDesktopTools
** FileName  : UIDesktopTools.h
** Date      : lun. août 13 19:14:07 2007
** License   : GPL
** Comment   : Your comment here
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
