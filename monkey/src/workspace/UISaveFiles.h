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
#ifndef UISAVEFILES_H
#define UISAVEFILES_H

#include <MonkeyExport.h>

#include <QDialog>

class pAbstractChild;
class QListWidget;
class QAbstractButton;
class QDialogButtonBox;

class Q_MONKEY_EXPORT UISaveFiles : public QDialog
{
    Q_OBJECT

public:
    enum Buttons { bSaveSelected = 0, bDiscardAll, bCancelClose };
    static UISaveFiles::Buttons saveDocuments( QWidget*, QList<pAbstractChild*>, bool );
    static UISaveFiles::Buttons saveDocument( QWidget*, pAbstractChild*, bool );

private:
    UISaveFiles( QWidget*, bool );
    void addFile( pAbstractChild* document );

    QListWidget* lwFiles;
    QDialogButtonBox* dbbButtons;

private slots:
    void clicked( QAbstractButton* );

};

#endif // UISAVEFILES_H
