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

#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QTextBrowser>

class MainWindow;
class QKeyEvent;
class QMime;
class QMouseEvent;
class QMenu;

class HelpWindow : public QTextBrowser
{
    Q_OBJECT
public:
    HelpWindow( MainWindow *m, QWidget *parent = 0);
    void setSource( const QUrl &name );
    void blockScrolling( bool b );
    void openLinkInNewWindow( const QString &link );
    void openLinkInNewPage( const QString &link );
    void addMimePath( const QString &path );

    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *);

signals:
    void chooseWebBrowser();
    void choosePDFReader();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

protected slots:
    void ensureCursorVisible();

private slots:
    void openLinkInNewWindow();
    void openLinkInNewPage();

    bool isKDERunning() const;

private:
    bool hasAnchorAt(const QPoint& pos);

    MainWindow *mw;
    QString lastAnchor;
    bool blockScroll;
    bool shiftPressed;
    bool newWindow;
    bool fwdAvail;
    bool backAvail;
};

#endif // HELPWINDOW_H
