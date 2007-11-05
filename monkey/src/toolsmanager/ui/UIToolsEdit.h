/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIToolsEdit.h
** Date      : 2007-11-04T22:46:49
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UITOOLSEDIT_H
#define UITOOLSEDIT_H

#include "MonkeyExport.h"
#include "ui_UIToolsEdit.h"
#include "QSingleton.h"

#include <QApplication>

class Q_MONKEY_EXPORT UIToolsEdit : public QDialog, public Ui::UIToolsEdit, public QSingleton<UIToolsEdit>
{
	Q_OBJECT
	friend class QSingleton<UIToolsEdit>;

public:
	enum ItemData { idCaption = Qt::DisplayRole, idFileIcon = Qt::UserRole +1, idFilePath, idWorkingPath };

private:
	UIToolsEdit( QWidget* = QApplication::activeWindow() );

protected:
	void closeEvent( QCloseEvent* );
	bool eventFilter( QObject*, QEvent* );

public slots:
	void on_lwTools_itemSelectionChanged();
	void on_pbNew_clicked();
	void on_pbDelete_clicked();
	void on_pbUp_clicked();
	void on_pbDown_clicked();
	void helpRequested();
	void on_leCaption_editingFinished();
	void on_tbFileIcon_clicked();
	void on_leFilePath_editingFinished();
	void on_tbFilePath_clicked();
	void on_tbUpdateWorkingPath_clicked();
	void on_leWorkingPath_editingFinished();
	void on_tbWorkingPath_clicked();
	void accept();

};

#endif // UITOOLSEDIT_H
