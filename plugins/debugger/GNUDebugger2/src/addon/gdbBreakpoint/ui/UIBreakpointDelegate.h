
#ifndef UIBREAKPOINTDELEGATE_H
#define UIBREAKPOINTDELEGATE_H

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>

//! [0]
class UIBreakpointDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    UIBreakpointDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

	void onCurrentIndexChanged(int);

};
//! [0]

#endif
