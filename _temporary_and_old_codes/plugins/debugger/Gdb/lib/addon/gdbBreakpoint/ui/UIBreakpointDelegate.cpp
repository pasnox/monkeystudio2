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
	\file UIBreakpointDelegate.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Delegate for QTabWiget. It used by UIGdbBreakpoint class. 
*/

#include "UIBreakpointDelegate.h"
#include <QMessageBox>


UIBreakpointDelegate::UIBreakpointDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget * UIBreakpointDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem & option ,
    const QModelIndex & index ) const
{

    if(index.column() == 1)
	{
		QComboBox *editor = new QComboBox(parent);
		editor->addItem(tr("True"));
		editor->addItem(tr("False"));
		return editor;
	}

	if(index.column() == 2 ) return QItemDelegate::createEditor(parent, option, index);

	return NULL;
}

//

// want change value 
void UIBreakpointDelegate::setEditorData(QWidget *editor,  const QModelIndex &index) const
{
	if(index.column() == 1)
	{
	    int value =  index.model()->data(index, Qt::UserRole).toInt();

	    QComboBox * cb = static_cast<QComboBox*>(editor);
	    cb->setCurrentIndex(value);
	}

	if(index.column() == 2 ) QItemDelegate::setEditorData(editor, index);
}

//

void UIBreakpointDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
	if(index.column() == 1)
	{
	    QComboBox *cb = static_cast<QComboBox*>(editor);
		int value = cb->currentIndex();
	    model->setData(index, value, Qt::UserRole);

		switch (value)
		{
		case 0 :  model->setData(index, tr("True"));
			break;
		case 1 :  model->setData(index, tr("False"));
			break;
		}
		return;
	}

	if(index.column() == 2 ) QItemDelegate::setModelData(editor,model,  index);
}

//

void UIBreakpointDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
