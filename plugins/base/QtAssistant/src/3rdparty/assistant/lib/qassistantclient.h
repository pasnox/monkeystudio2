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

#ifndef QASSISTANTCLIENT_H
#define QASSISTANTCLIENT_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/qglobal.h>
#include <QtAssistant/qassistantclient_global.h>

QT_BEGIN_HEADER

class QTcpSocket;

class QT_ASSISTANT_CLIENT_EXPORT QAssistantClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool open READ isOpen )

public:
    QAssistantClient( const QString &path, QObject *parent = 0);
    ~QAssistantClient();

    bool isOpen() const;

    void setArguments( const QStringList &args );

public Q_SLOTS:
    virtual void openAssistant();
    virtual void closeAssistant();
    virtual void showPage( const QString &page );

Q_SIGNALS:
    void assistantOpened();
    void assistantClosed();
    void error( const QString &msg );

private Q_SLOTS:
    void socketConnected();
    void socketConnectionClosed();
    void readPort();
    void procError(QProcess::ProcessError err);
    void socketError();
    void readStdError();

private:
    QTcpSocket *socket;
    QProcess *proc;
    quint16 port;
    QString host, assistantCommand, pageBuffer;
    bool opened;
};

QT_END_HEADER
#endif
