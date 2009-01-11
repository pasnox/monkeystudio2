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
	\file UIManageDelegate.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Delegate of UIGbdManageFile
*/

#ifndef UIMANAGEDELEGATE_H
#define UIMANAGEDELEGATE_H

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>

/*!
	\brief Delegate of UIGbdManageFile
*/
class UIManageDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    UIManageDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
#endif
