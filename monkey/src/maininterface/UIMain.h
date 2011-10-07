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
#ifndef UIMAIN_H
#define UIMAIN_H

#include <MonkeyExport.h>
#include <pMainWindow.h>

#include <QUrl>

class pStylesActionGroup;

class Q_MONKEY_EXPORT UIMain : public pMainWindow
{
    Q_OBJECT
    friend class MonkeyCore;

public:
    QMenu* createPopupMenu();
    bool updateMenuVisibility( QMenu* menu );

protected:
    pStylesActionGroup* agStyles;

    UIMain( QWidget* = 0 );
    
    virtual void dragEnterEvent( QDragEnterEvent* event );
    virtual void dropEvent( QDropEvent* event );
    
    void initGui();
    void closeEvent( QCloseEvent* );
    void initMenuBar();
    void initToolBar();
    void initConnections();
    void finalyzeGuiInit();

public slots:
    void menu_Docks_aboutToShow();
    void menu_CustomAction_aboutToShow();
    void changeStyle( const QString& style );

signals:
    void aboutToClose();
    void urlsDropped( const QList<QUrl>& urls );
};

#endif // UIMAIN_H
