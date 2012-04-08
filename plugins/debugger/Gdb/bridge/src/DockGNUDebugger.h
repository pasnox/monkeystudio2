/****************************************************************************
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
    \file DockGNUDebugger.h
    \date 14/08/08
    \author Xiantia
    \version 1.3.2
    \brief This class is a main class container. 
*/

#ifndef PDOCKGNUDEBUGGER_H
#define PDOCKGNUDEBUGGER_H

// Pasnox include
#include <fresh.h>
#include "XUPProjectManager.h"

// Xiantia include
#include <QTextEdit>
#include <QFileInfo>
#include <QTabWidget>
#include <QPointer>
#include <QHash>

#include "../../lib/gdb.h"


#include "BridgeQsci.h"
#include "UISetting/UIGNUDebuggerSetting.h"

/*!
    \brief This class is a main class container. 
    \details 
    * DockGNUDebugger is created by GNUDebugger.
    * Those is the main container class. It creates all AddOn and the connect together.
    * It has under its GdbParser order which parse warp the data coming from Gdb, 
    * GdbProcess which controls Gdb, GdbBridgeEditor which control the editor Qsci and GdbKernelDispatcher which control AddOn.
    * The creation of AddOn is very simple.
    * AddOn are class which adds news function to the debugger such as GdbBreakpoint and GdbWatch.
    * For that it has pointers on AddOn and add them to GdbKernelDispatcher so that AddOn receives the messages coming from GdbParser.
    *
    * in .h
    *
    * \code
    *   // declare pointer to AddOn
    *   class GdbBreakpoint *Breakpoint;
    * \endcode
    *
    * in .cpp
    *
    * \code
    *   // create new class
    *   Breakpoint = new GdbBreakpoint(this);
    *   // add them to Dispatcher
    *   Dispatcher->add(Breakpoint);
    * \endcode
    *
    * Connect signals
    *
    * \code
    *   connect(Bridge, SIGNAL(requestBreakpoint(const QString &)), Breakpoint , SLOT(onRequestBreakpoint(const QString &)));
    * \endcode
    *
    * Once creates AddOn are visible in QTabWidget of DockGNUDebugger if the function widget() return a pointer not NULL.
    *
    *
    *\note All AddOn must derive from GdbCore.
*/
    
class DockGNUDebugger : public pDockWidget
{
        Q_OBJECT
        
public:

    DockGNUDebugger( QWidget* = 0 );
    ~DockGNUDebugger();

    void loadSettings();
    void saveSettings();

private:


    QHash<QString, QAction*> mActionList;
    QMenu *mMenu;

        /*!
        * \details Main container for GNU debugger is QTabWidget
        */
        QTabWidget *mainTabWidget;

    /*!
        * \details Add raw log in QTabWidget
        */
        QTextEdit *rawLog;


    /*!
        * \details End of line
        */
        QString crlf;

public slots:

    void projectOpened( XUPProjectItem* project );
    void saveBreakpoint(QStringList);
    void commandReadyRead(  const QString& d);

    // Target
    void onTargetLoaded(int , QString);
    void onTargetNoLoaded(int , QString);
    void onTargetRunning(int , QString);
    void onTargetExited(int , QString);
    void onTargetStopped(int , QString);

    // from parser
    void onDone(int , QString);
    void onPrompt(int , QString);

    void onErrorMessage(QString, int);
    void onDefaultMessage(QString);
signals:
void setBreakpointOpened(QStringList);

};

#endif 
