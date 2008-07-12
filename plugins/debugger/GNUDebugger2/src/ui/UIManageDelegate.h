
#ifndef UIMANAGEDELEGATE_H
#define UIMANAGEDELEGATE_H

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>

//! Delegate of UIGbdManageFile
/**
\author xiantia.
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
//! [0]

#endif
