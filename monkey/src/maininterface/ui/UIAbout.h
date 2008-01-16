/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UIAbout.h
** Date      : 2008-01-14T00:36:54
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
