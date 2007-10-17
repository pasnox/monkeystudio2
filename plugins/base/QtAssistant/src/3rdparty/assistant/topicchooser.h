/****************************************************************************
**
** Copyright (C) 1992-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Assistant of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef TOPICCHOOSER_H
#define TOPICCHOOSER_H

#include "ui_topicchooser.h"

#include <QDialog>
#include <QStringList>

class TopicChooser : public QDialog
{
    Q_OBJECT
public:
    TopicChooser(QWidget *parent, const QStringList &lnkNames,
                  const QStringList &lnks, const QString &title);

    QString link() const;

    static QString getLink(QWidget *parent, const QStringList &lnkNames,
                            const QStringList &lnks, const QString &title);

private slots:
    void on_buttonDisplay_clicked();
    void on_buttonCancel_clicked();
    void on_listbox_itemActivated(QListWidgetItem *item);

private:
    Ui::TopicChooser ui;
    QString theLink;
    QStringList links, linkNames;
};

#endif // TOPICCHOOSER_H
