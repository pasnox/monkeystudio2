/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIAbout.h
** Date      : 2007-11-04T22:45:08
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UIABOUT_H
#define UIABOUT_H

#include "ui_UIAbout.h"
#include "MonkeyExport.h"
#include "QSingleton.h"

struct pDatas
{
	pDatas( const QString& n, const QString& l, const QString& p, const QString& e, const QString& c )
		: Name( n ), Login( l ), Pays( p ), Email( e ), Comment( c )
	{}

	QString Name;
	QString Login;
	QString Pays;
	QString Email;
	QString Comment;
};

class Q_MONKEY_EXPORT UIAbout : public QDialog, public Ui::UIAbout, public QSingleton<UIAbout>
{
	Q_OBJECT
	friend class QSingleton<UIAbout>;

public slots:
	void anchorClicked( const QUrl& );

private:
	UIAbout( QWidget* = 0 );

};

#endif // UIABOUT_H
