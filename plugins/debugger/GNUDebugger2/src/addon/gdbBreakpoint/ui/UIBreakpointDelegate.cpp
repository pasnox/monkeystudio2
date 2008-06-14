

#include "UIBreakpointDelegate.h"
#include <QMessageBox>


//! [0]
UIBreakpointDelegate::UIBreakpointDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}
//! [0]

//! [1]

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
