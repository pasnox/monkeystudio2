
#include "treeDelegate.h"
#include <QMessageBox>

TreeDelegate::TreeDelegate(QObject *parent)
    : QItemDelegate(parent)
{}


QWidget * TreeDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem & option ,
    const QModelIndex & index ) const
{

	if(index.column() == 1) // enable breakpoint
	{
		QComboBox *cbEnable  = new QComboBox(parent);
		cbEnable->addItem ("True", true);
		cbEnable->addItem ("False", false);
		return cbEnable;
	}

//	if(index.column() == 2) // condition 
//		return QItemDelegate::createEditor(parent, option, index);

	return NULL;
}

//

void TreeDelegate::setEditorData(QWidget *editor,
           const QModelIndex &index) const
{
	if(index.column() == 1)
	{
		QString value = index.model()->data(index, Qt::DisplayRole).toString();
	    QComboBox *cb = static_cast<QComboBox*>(editor);
		cb->setCurrentIndex( (value == "True") ? 0 : 1 );
	}

//	if(index.column() == 2) 
//		QItemDelegate::setEditorData(editor, index);
}

//

void TreeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
	if(index.column() == 1)
	{
	    QComboBox *cb = static_cast<QComboBox*>(editor);

		model->setData(index,  ! cb->currentIndex() ? "True" : "False");
	}

//	if(index.column() == 2) 
//		QItemDelegate::setModelData(editor, model , index);
}

