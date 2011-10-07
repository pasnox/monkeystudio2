/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
#include <QtCore/QMap>
#include <QtCore/QUrl>

#include "topicchooser.h"

QT_BEGIN_NAMESPACE

TopicChooser::TopicChooser(QWidget *parent, const QString &keyword,
                         const QMap<QString, QUrl> &links)
    : QDialog(parent)
{
    ui.setupUi(this);
    ui.label->setText(tr("Choose a topic for <b>%1</b>:").arg(keyword));

    m_links = links;
    QMap<QString, QUrl>::const_iterator it = m_links.constBegin();
    for (; it != m_links.constEnd(); ++it)
        ui.listWidget->addItem(it.key());
    
    if (ui.listWidget->count() != 0)
        ui.listWidget->setCurrentRow(0);
    ui.listWidget->setFocus();

    connect(ui.buttonDisplay, SIGNAL(clicked()),
        this, SLOT(accept()));
    connect(ui.buttonCancel, SIGNAL(clicked()),
        this, SLOT(reject()));
    connect(ui.listWidget, SIGNAL(itemActivated(QListWidgetItem*)),
        this, SLOT(accept()));
}

QUrl TopicChooser::link() const
{
    QListWidgetItem *item = ui.listWidget->currentItem();
    if (!item)
        return QUrl();

    QString title = item->text();
    if (title.isEmpty() || !m_links.contains(title))
        return QUrl();

    return m_links.value(title);
}

QT_END_NAMESPACE
