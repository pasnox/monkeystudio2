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
	\file UIManageDelegate.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Delegate of UIGbdManageFile
*/

#include "UIManageDelegate.h"


UIManageDelegate::UIManageDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

/*!
	\details Create editor of delegate
*/
QWidget * UIManageDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem & option ,
    const QModelIndex & index ) const
{
    if(index.column() == 1)
	{
		QComboBox *editor = new QComboBox(parent);
		editor->addItem(tr("No use"));
		editor->addItem(tr("Allway use"));
		editor->addItem(tr("Show message"));
	    return editor;
	}

	if(index.column() == 2 || index.column() == 3 || index.column() == 0) return QItemDelegate::createEditor(parent, option, index);

	return NULL;
}


void UIManageDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
	if(index.column() == 1)
	{
	    int value =  index.model()->data(index, Qt::UserRole).toInt();

	    QComboBox * cb = static_cast<QComboBox*>(editor);
	    cb->setCurrentIndex(value);
	}

	if(index.column() == 2 || index.column() == 3 || index.column() == 0) QItemDelegate::setEditorData(editor, index);
}

void UIManageDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
	if(index.column() == 1)
	{
	    QComboBox *cb = static_cast<QComboBox*>(editor);
		int value = cb->currentIndex();
	    model->setData(index, value, Qt::UserRole);

		switch (value)
		{
		case 0 :  model->setData(index, tr("No use"));
			break;
		case 1 :  model->setData(index, tr("Allway use"));
			break;
		case 2 :  model->setData(index, tr("Show message"));
			break;
		}

		return;
	}

	if(index.column() == 2 || index.column() == 3 || index.column() == 0) QItemDelegate::setModelData(editor,model,  index);
}

/*!
	\details Update Geometry
*/
void UIManageDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
