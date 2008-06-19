

#include "UIManageDelegate.h"


//! [0]
UIManageDelegate::UIManageDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}
//! [0]

//! [1]

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
//! [1]

//! [2]

// want change value 
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

//! [2]

//! [3]
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

//! [3]

//! [4]
void UIManageDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
//! [4]
