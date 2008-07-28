/****************************************************************************
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
****************************************************************************/
/*!
	\file pShortcutsEditor.h
	\date 2008-01-14T00:27:48
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief A shortcut dialog editor
*/
#ifndef PSHORTCUTSEDITOR_H
#define PSHORTCUTSEDITOR_H

#include "../objects/MonkeyExport.h"
#include "../objects/QSingleton.h"

#include <QDialog>
#include <QHash>

class QTreeWidget;
class QTreeWidgetItem;
class QPushButton;
class QLineEdit;

/*!
	\brief A shortcut dialog editor
	\details This dialog allow to edit shortcut of pAction actions
*/
class Q_MONKEY_EXPORT pShortcutsEditor : public QDialog, public QSingleton<pShortcutsEditor>
{
	Q_OBJECT
	friend class QSingleton<pShortcutsEditor>;

public:
	pShortcutsEditor( QWidget* parent = 0 );

private:
	QHash<QString, QTreeWidgetItem*> mItems;
	QTreeWidget* twShortcuts;
	QPushButton* pbRestore;
	QPushButton* pbClear;
	QLineEdit* leShortcut;
	QPushButton* pbSet;
	QPushButton* pbClose;

private slots:
	void on_twShortcuts_itemSelectionChanged();
	void pbRestore_pbSet_clicked();
	void on_leShortcut_textChanged( const QString& text );
};

#endif // PSHORTCUTSEDITOR_H
