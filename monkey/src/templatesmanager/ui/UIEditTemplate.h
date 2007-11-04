/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIEditTemplate.h
** Date      : 2007-11-04T22:46:22
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef UIEDITTEMPLATE_H
#define UIEDITTEMPLATE_H

#include "MonkeyExport.h"
#include "ui_UIEditTemplate.h"

class QTreeWidget;
class QTreeWidgetItem;

class Q_MONKEY_EXPORT UIEditTemplate : public QDialog, public Ui::UIEditTemplate
{
	Q_OBJECT

public:
	static void edit( QTreeWidget*, QTreeWidgetItem* );

private:
	UIEditTemplate( QWidget*, QTreeWidget*, QTreeWidgetItem* );
	~UIEditTemplate();

	bool canAddFile( const QString& );

	QTreeWidget* mTree;
	QTreeWidgetItem* mItem;

private slots:
	void on_tbIcon_clicked();
	void on_pbAdd_clicked();
	void on_pbEdit_clicked();
	void on_pbRemove_clicked();
	void accept();

};

#endif // UIEDITTEMPLATE_H
