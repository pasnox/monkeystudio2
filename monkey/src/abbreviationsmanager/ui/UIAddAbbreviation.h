/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIAddAbbreviation.h
** Date      : 2007-11-04T22:45:02
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UIADDABBREVIATION_H
#define UIADDABBREVIATION_H

#include "MonkeyExport.h"
#include "ui_UIAddAbbreviation.h"

class QTreeWidget;

class Q_MONKEY_EXPORT UIAddAbbreviation : public QDialog, public Ui::UIAddAbbreviation
{
	Q_OBJECT

public:
	static void edit( QTreeWidget* );

private:
	UIAddAbbreviation( QWidget*, QTreeWidget* );
	~UIAddAbbreviation();

	QTreeWidget* mTree;

private slots:
	void accept();

};

#endif // UIADDABBREVIATION_H
