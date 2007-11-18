/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UIProjectHeaders
** FileName  : UIProjectHeaders.h
** Date      : 2007-11-04T02:21:34
** License   : GPL
** Comment   : Your comment here
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
	void on_cbLanguages_currentIndexChanged( const QString& );
	void on_tbDirectory_clicked();
	void accept();

};

#endif // UIPROJECTHEADERS_H
