/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UITranslator.h
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

	QStringList mTranslationsPath;

public slots:
	virtual void accept();
	void on_tbLocate_clicked();
	void on_tbReload_clicked();

};

#endif // UITRANSLATOR_H
