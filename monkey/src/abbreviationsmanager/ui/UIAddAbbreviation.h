/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
****************************************************************************/
#ifndef UIADDABBREVIATION_H
#define UIADDABBREVIATION_H

#include <MonkeyExport.h>

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
