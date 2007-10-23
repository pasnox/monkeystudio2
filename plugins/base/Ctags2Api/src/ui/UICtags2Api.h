/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UICtags2Api
** FileName  : UICtags2Api.h
** Date      : 2007-10-23T21:26:13
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UICTAGS2API_H
#define UICTAGS2API_H

#include "ui_UICtags2Api.h"

class UICtags2Api : public QDialog, public Ui::UICtags2Api
{
	Q_OBJECT

public:
	UICtags2Api( QWidget* = 0 );
	~UICtags2Api();

protected slots:
	void on_tbCtagsBinary_clicked();
	void on_tbBrowse_clicked();

};

#endif // UICTAGS2API_H
