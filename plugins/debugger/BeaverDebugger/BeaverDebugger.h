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
/*!
    \file BeaverDebugger.h
    \date 2008-01-14T00:40:08
    \author Filipe AZEVEDO, Andrei KOPATS
    \brief Header file for BeaverDebugger plugin
*/
#ifndef BEAVER_DEBUGGER
#define BEAVER_DEBUGGER

#include <pluginsmanager/DebuggerPlugin.h>

#include <QPointer>
#include <QProcess>
#include <QLabel>

class XUPProjectItem;

/*!
    Main class of BeaverDebugger plugin
    
    Plugin allows to use Beaver Debugger with MkS
*/
class BeaverDebugger : public DebuggerPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin DebuggerPlugin )
    
    enum TryFindResult
    {
        OK,
        NOT_FINISHED,
        FAILED_TO_START,
        CRASHED,
        UNKNOWN_ERROR,
        NOT_BEAVER,
    };
    
protected:  
    void fillPluginInfos();
    virtual bool install(); // FIXME make protected for all plugins
    virtual bool uninstall();
public:
    virtual QWidget* settingsWidget() const;
    
    // plugin <-> settings API
    QString beaverPath(); //FIXME make const
    void setBeaverPath(const QString& path);
    
protected slots:
    void explainWhyCannot();
    void runBeaver();
    void beaverStateChanged(QProcess::ProcessState);
    // action can have text "run" and "stop" and be enabled and disabled
    void updateRunAction();
    
protected:
    QString mBeaverPath;
    QPointer<QAction> mWhyCannot;
    QPointer<QAction> mRunBeaver;
    QPointer<QProcess> mBeaverProcess;
    QPointer<QLabel> mStatusLabel;
    // Returns QString::null if found, or error, if not
    TryFindResult tryFindBeaver() const;
};

#endif
