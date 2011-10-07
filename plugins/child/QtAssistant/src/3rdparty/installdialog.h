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
#ifndef INSTALLDIALOG_H
#define INSTALLDIALOG_H

#include <QtCore/QQueue>
#include <QtGui/QDialog>
#include <QtNetwork/QHttpResponseHeader>
#include "ui_installdialog.h"

#ifndef QT_NO_HTTP

QT_BEGIN_NAMESPACE

class QHttp;
class QBuffer;
class QFile;
class QHelpEngineCore;

class InstallDialog : public QDialog
{
    Q_OBJECT

public:
    InstallDialog(QHelpEngineCore *helpEngine, QWidget *parent = 0,
        const QString &host = QString(), int port = -1);
    ~InstallDialog();

    QStringList installedDocumentations() const;

private slots:
    void init();
    void cancelDownload();
    void install();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);
    void updateInstallButton();
    void browseDirectories();

private:
    void downloadNextFile();
    void updateDocItemList();
    void installFile(const QString &fileName);

    Ui::InstallDialog m_ui;
    QHelpEngineCore *m_helpEngine;
    QHttp *m_http;
    QBuffer *m_buffer;
    QFile *m_file;
    bool m_httpAborted;
    int m_docInfoId;
    int m_docId;
    QQueue<QListWidgetItem*> m_itemsToInstall;
    QString m_currentCheckSum;
    QString m_windowTitle;
    QStringList m_installedDocumentations;
    QString m_host;
    int m_port;
};

QT_END_NAMESPACE

#endif

#endif // INSTALLDIALOG_H
