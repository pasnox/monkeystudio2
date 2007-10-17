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

#include "helpwindow.h"
#include "mainwindow.h"
#include "tabbedbrowser.h"
#include "helpdialog.h"
#include "config.h"

#include <QApplication>
#include <QClipboard>
#include <QUrl>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QMenu>
#include <QAction>
#include <QFileInfo>
#include <QFont>
#include <QtEvents>
#include <QTextStream>
#include <QTextCodec>
#include <QStatusBar>
#include <QTextCursor>
#include <QTextObject>
#include <QTextLayout>
#include <QtDebug>
#include <qdesktopservices.h>

#if defined(Q_OS_WIN32)
#  include <windows.h>
#endif

HelpWindow::HelpWindow(MainWindow *w, QWidget *parent)
    : QTextBrowser(parent), mw(w), blockScroll(false),
      shiftPressed(false), newWindow(false)
{
    QFont f = font();
    f.setPointSizeF(Config::configuration()->fontPointSize());
    setFont(f);
}

void HelpWindow::setSource(const QUrl &name)
{
    if (!name.isValid())
        return;

    shiftPressed = shiftPressed & hasFocus();

    if (newWindow || shiftPressed) {
        shiftPressed = false;
        QTextCursor c = textCursor();
        c.clearSelection();
        setTextCursor(c);
        mw->saveSettings();
        MainWindow *nmw = new MainWindow;

        nmw->setup();
        nmw->showLink(name.toString());
        nmw->move(mw->geometry().topLeft());
        if (mw->isMaximized())
            nmw->showMaximized();
        else
            nmw->show();
        return;
    }

    if (name.scheme() == QLatin1String("http") || name.scheme() == QLatin1String("ftp") || name.scheme() == QLatin1String("mailto")
        || name.path().endsWith(QLatin1String("pdf"))) {
        bool launched = QDesktopServices::openUrl(name);
        if (!launched) {
            QMessageBox::information(mw, tr("Help"),
                         tr("Unable to launch web browser.\n"),
                         tr("Ok"));
        }
        return;
    }

    if (name.scheme() == QLatin1String("file")) {
        QFileInfo fi(name.toLocalFile());
        if (!fi.exists()) {
            mw->statusBar()->showMessage(tr("Failed to open link: '%1'").arg(fi.absoluteFilePath()), 5000);
            setHtml(tr("<div align=\"center\"><h1>The page could not be found</h1><br>"
                "<h3>'%1'</h3></div>").arg(fi.absoluteFilePath()));
            mw->browsers()->updateTitle(tr("Error..."));
            return;
        }

        /*
        setHtml(QLatin1String("<body bgcolor=\"")
            + palette().color(backgroundRole()).name()
            + QLatin1String("\">"));
            */

        QTextBrowser::setSource(name);

        return;
    }
    mw->statusBar()->showMessage(tr("Failed to open link: '%1'").arg(name.toString()), 5000);
    setHtml(tr("<div align=\"center\"><h1>The page could not be found</h1><br>"
        "<h3>'%1'</h3></div>").arg(name.toString()));
    mw->browsers()->updateTitle(tr("Error..."));
}

void HelpWindow::openLinkInNewWindow()
{
    if (lastAnchor.isEmpty())
        return;
    newWindow = true;
    setSource(lastAnchor);
    newWindow = false;
}

void HelpWindow::openLinkInNewWindow(const QString &link)
{
    lastAnchor = link;
    openLinkInNewWindow();
}

void HelpWindow::openLinkInNewPage()
{
    if(lastAnchor.isEmpty())
        return;
    mw->browsers()->newTab(lastAnchor);
    lastAnchor.clear();
}

void HelpWindow::openLinkInNewPage(const QString &link)
{
    lastAnchor = link;
    openLinkInNewPage();
}

bool HelpWindow::hasAnchorAt(const QPoint& pos)
{
    lastAnchor = anchorAt(pos);
    if (lastAnchor.isEmpty()) 
        return false;
    lastAnchor = source().resolved(lastAnchor).toString();
    if (lastAnchor.at(0) == QLatin1Char('#')) {
        QString src = source().toString();
        int hsh = src.indexOf(QLatin1Char('#'));
        lastAnchor = (hsh>=0 ? src.left(hsh) : src) + lastAnchor;
    }
    return true;
}

void HelpWindow::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *m = new QMenu(0);
    if (hasAnchorAt(e->pos())) {
        m->addAction(tr("Open Link in New Window\tShift+LMB"),
                       this, SLOT(openLinkInNewWindow()));
        m->addAction(tr("Open Link in New Tab"),
                       this, SLOT(openLinkInNewPage()));
    }
    mw->setupPopupMenu(m);
    m->exec(e->globalPos());
    delete m;
}

void HelpWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::MidButton && hasAnchorAt(e->pos())) {
        openLinkInNewPage();
        return;
    }
    QTextBrowser::mouseReleaseEvent(e);
}

void HelpWindow::blockScrolling(bool b)
{
    blockScroll = b;
}

void HelpWindow::ensureCursorVisible()
{
    if (!blockScroll)
        QTextBrowser::ensureCursorVisible();
}

void HelpWindow::mousePressEvent(QMouseEvent *e)
{
    shiftPressed = e->modifiers() & Qt::ShiftModifier;
    QTextBrowser::mousePressEvent(e);
}

void HelpWindow::keyPressEvent(QKeyEvent *e)
{
    shiftPressed = e->modifiers() & Qt::ShiftModifier;
    QTextBrowser::keyPressEvent(e);
}

bool HelpWindow::isKDERunning() const
{
    return !qgetenv("KDE_FULL_SESSION").isEmpty();
}

