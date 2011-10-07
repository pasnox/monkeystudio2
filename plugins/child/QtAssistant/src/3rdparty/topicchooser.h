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
#ifndef TOPICCHOOSER_H
#define TOPICCHOOSER_H

#include "ui_topicchooser.h"

#include <QUrl>
#include <QMap>
#include <QString>

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class TopicChooser : public QDialog
{
    Q_OBJECT

public:
    TopicChooser(QWidget *parent, const QString &keyword,
        const QMap<QString, QUrl> &links);

    QUrl link() const;    
    
private:
    Ui::TopicChooser ui;
    QMap<QString, QUrl> m_links;
};

QT_END_NAMESPACE

#endif // TOPICCHOOSER_H
