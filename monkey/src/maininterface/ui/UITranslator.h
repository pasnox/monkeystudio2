/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UITranslator.h
** Date      : 2007-11-04T22:45:19
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UITRANSLATOR_H
#define UITRANSLATOR_H

#include "ui_UITranslator.h"
#include "MonkeyExport.h"
#include "QSingleton.h"

class Q_MONKEY_EXPORT UITranslator : public QDialog, public Ui::UITranslator, public QSingleton<UITranslator>
{
	Q_OBJECT
	friend class QSingleton<UITranslator>;

private:
	UITranslator( QWidget* = 0 );

	QString mTranslationsPath;

public slots:
	virtual void accept();
	void on_tbLocate_clicked();
	void on_tbReload_clicked();

};

#endif // UITRANSLATOR_H
