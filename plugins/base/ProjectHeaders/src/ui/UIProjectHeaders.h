/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIProjectHeaders.h
** Date      : 2007-11-04T22:50:47
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UIPROJECTHEADERS_H
#define UIPROJECTHEADERS_H

#include "ui_UIProjectHeaders.h"

class UIProjectHeaders : public QDialog, public Ui::UIProjectHeaders
{
	Q_OBJECT
	
public:
	UIProjectHeaders( QWidget* = 0 );
	~UIProjectHeaders();

protected slots:
	void on_tbDirectory_clicked();
	void accept();

};

#endif // UIPROJECTHEADERS_H
