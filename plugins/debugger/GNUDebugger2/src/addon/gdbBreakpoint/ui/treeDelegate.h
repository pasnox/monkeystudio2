
#ifndef GDBTREEDELEGATE_H
#define GDBTREEDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QComboBox>

class TreeDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    TreeDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

};

#endif
