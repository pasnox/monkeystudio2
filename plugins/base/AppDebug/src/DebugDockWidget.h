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
#ifndef DEBUGDOCKWIDGET_H
#define DEBUGDOCKWIDGET_H

#include "ui_DebugDockWidget.h"

class XUPItem;

class DebugDockWidget : public pDockWidget, public Ui::DebugDockWidget
{
    Q_OBJECT
    
public:
    DebugDockWidget( QWidget* parent = 0 );
    virtual ~DebugDockWidget();

public slots:
    void qtMessageHandler( QtMsgType type, const char* msg );

protected:
    void createAllScopes( XUPItem* parent );
    void createScope( XUPItem* parent, bool nested1, bool nested2, bool nested3, bool nested4 = false, bool nested5 = false );

protected slots:
    void currentProjectChanged();
    void showXml();
    void showNativeString();
    void generateFakeProject();
    void childInfos();
};

#endif // DEBUGDOCKWIDGET_H
