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

#include "topicchooser.h"

#include <QLabel>
#include <QListWidget>
#include <QPushButton>

TopicChooser::TopicChooser(QWidget *parent, const QStringList &lnkNames,
                            const QStringList &lnks, const QString &title)
    : QDialog(parent), links(lnks), linkNames(lnkNames)
{
    ui.setupUi(this);

    ui.label->setText(tr("Choose a topic for <b>%1</b>").arg(title));
    ui.listbox->addItems(linkNames);
    if (ui.listbox->count() != 0)
        ui.listbox->setCurrentRow(0);
    ui.listbox->setFocus();
}

QString TopicChooser::link() const
{
    if (ui.listbox->currentRow() == -1)
        return QString();
    QString s = ui.listbox->item(ui.listbox->currentRow())->text();
    if (s.isEmpty())
        return s;
    int i = linkNames.indexOf(s);
    return links[i];
}

QString TopicChooser::getLink(QWidget *parent, const QStringList &lnkNames,
                              const QStringList &lnks, const QString &title)
{
    TopicChooser *dlg = new TopicChooser(parent, lnkNames, lnks, title);
    QString lnk;
    if (dlg->exec() == QDialog::Accepted)
        lnk = dlg->link();
    delete dlg;
    return lnk;
}

void TopicChooser::on_buttonDisplay_clicked()
{
    accept();
}

void TopicChooser::on_buttonCancel_clicked()
{
    reject();
}

void TopicChooser::on_listbox_itemActivated(QListWidgetItem *item)
{
    Q_UNUSED(item);
    accept();
}
