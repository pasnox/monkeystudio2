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
#ifndef UIIRCMAIN_H
#define UIIRCMAIN_H

#include "ui_UIIrcMain.h"


class IrcStatus : public QWidget, public Ui::UIIrcMain
{
    Q_OBJECT

public:

    IrcStatus(QWidget *p=0);
    void appendLog(QString);

public slots :

    void onConnect();
    void onJoin();

private :
    bool bConnected;

signals:
    void ircConnect(QString ,bool);
    void ircJoinChannel(QString);


};

#endif
