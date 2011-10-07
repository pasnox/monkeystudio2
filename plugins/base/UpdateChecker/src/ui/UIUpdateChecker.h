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
#ifndef UIUPDATECHECKER_H
#define UIUPDATECHECKER_H

#include "ui_UIUpdateChecker.h"

#include <pVersion.h>

#include <QDomDocument>
#include <QDateTime>

class QNetworkAccessManager;
class QNetworkReply;
class UpdateChecker;

class UpdateItem
{
public:
    enum Type
    {
        Updated,
        Id,
        Link,
        Title,
        Author,
        Content
    };
    
    UpdateItem( const QDomElement& element = QDomElement() );
    
    bool operator<( const UpdateItem& other ) const;
    bool operator>( const UpdateItem& other ) const;
    bool operator<( const pVersion& other ) const;
    bool operator>( const pVersion& other ) const;
    
    QDateTime updated() const;
    QString id() const;
    QUrl link() const;
    QString title() const;
    QString author() const;
    QString content() const;
    QString toolTip() const;
    bool isFeatured() const;
    QString displayText() const;
    QString versionString() const;
    pVersion version() const;
    bool isValid() const;

protected:
    QMap<UpdateItem::Type, QString> mDatas;
};

class UIUpdateChecker : public QDialog, public Ui::UIUpdateChecker
{
    Q_OBJECT

public:
    UIUpdateChecker( UpdateChecker* plugin, QWidget* = 0 );
    ~UIUpdateChecker();

protected:
    UpdateChecker* mPlugin;
    static const QString mDownloadsUrl;
    QNetworkAccessManager* mAccessManager;

protected slots:
    void accessManager_finished( QNetworkReply* reply );
    void on_lwVersions_itemSelectionChanged();
    virtual void accept();
};

Q_DECLARE_METATYPE( UpdateItem )

#endif // UIUPDATECHECKER_H
