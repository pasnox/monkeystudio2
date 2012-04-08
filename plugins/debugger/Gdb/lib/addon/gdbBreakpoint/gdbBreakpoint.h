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
    \file gdbBreakpoint.h
    \date 15/05/09
    \author Xiantia
    \version 1.4.0
    \brief Implements all functions for remote breakpoints.This class is an AddOn for GNU debugger
*/
/*

    GdbBreakpoint class


    Xiantia@gmail.com

    for Debugger v1.4.0
*/

#ifndef GDBBREAKPOINT_H
#define GDBBREAKPOINT_H

#include <QObject>
#include <QHash>
#include <QList>

#include <QTextEdit>
#include "../../kernel/gdbCore.h"
#include "../../kernel/gdbAddonBase.h"
#include "../../kernel/gdbInterpreter.h"
#include "../../kernel/gdbKernelDispatcher.h"
#include "../../kernel/gdbControler.h"
class UIGdbBreakpoint;


//#include "gdbBreakpointStruct.h"
#include "gdbBreakpointThread.h"

/*!
    \brief Implements all functions for remote breakpoints.This class is an AddOn for GNU debugger

    \details This class can toogle breakpoint when user click in the margin, enable or disable breakpoint from UIGdbBreakpoint and set
condition or not.

It will start by adding seven interpreters has GdbParser for parse only specific line:
- add of a breakpoint
\code
    interpreterAddBreakpoint = GdbCore::Parser()->addInterpreter(
        name(),
        QRegExp("^b\\s.*:\\d+$"),
        QRegExp("^Breakpoint\\s+(\\d+)\\s+at\\s(\\w+):\\s+file\\s+([^,]+),\\s+line\\s+(\\d+)\\.(|\\s+\\(\\d+\\s\\w*\\))"),
        "");
\endcode

- to remove a breakpoint
\code
    interpreterDelBreakpoint = GdbCore::Parser()->addInterpreter(
        name(),
        QRegExp("^delete\\s\\d+"),
        QRegExp("^\\(gdb\\)\\s"),
        "^info,interpreter=\"" + name() + "\",event=\"Breakpoint-delete\",answerGdb=\"");
\endcode

- enable a breakpoint
\code
    interpreterEnabledBreakpoint = GdbCore::Parser()->addInterpreter(
        name(),
        QRegExp("^enable\\s\\d+"),
        QRegExp("^\\(gdb\\)\\s"),
        "");
\endcode

- disable a breakpoint
\code
    interpreterDisabledBreakpoint = GdbCore::Parser()->addInterpreter(
        name(),
        QRegExp("^disable\\s\\d+"),
        QRegExp("^\\(gdb\\)\\s"),
        "");
\endcode

- to condition a breapoint
\code
    interpreterConditionnedBreakpoint = GdbCore::Parser()->addInterpreter(
        name(),
        QRegExp("^condition\\s\\d+.+$"),
        QRegExp("^\\(gdb\\)\\s"),
        "");
\endcode

- uncondition a breakpoint
\code
    interpreterUnConditionnedBreakpoint = GdbCore::Parser()->addInterpreter(
        name(),
        QRegExp("^condition\\s\\d+$"),
        QRegExp("^Breakpoint\\s\\d+\\snow\\sunconditional.$"),
        "");
\endcode

- set breakpoint before load lib
 \code
    interpreterBreakpointPending = GdbCore::Parser()->addInterpreter(
        name(),
        QRegExp("^b\\s.*:\\d+$"),
        QRegExp("^Breakpoint\\s(\\d+)\\s\\((.*):(\\d+)\\)\\spending\\.$"),
        "^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Add-Pending\",answerGdb=\"");
\endcode

\note These interpreters are not active which if their respective command are in court.

Then to connect them to the corresponding functions.

According to the events, the GdbBreakpoint class  modify the struct Breakpoint and emits the signal toggleBreakpoint
*/

class GdbBreakpoint : public GdbAddonBase
{
    Q_OBJECT
    friend class GdbBreakpointThread;

public:

    GdbBreakpoint(QObject * parent = 0 );
    ~GdbBreakpoint();

public slots:

    void restoreBreakpoint(QStringList);

    void onBreakpointEnabled(int, QString);
    void onBreakpointDisabled(int, QString);
    void onToggleEnabledBreakpoint(const QString & ,const int & , const bool &);

    void onBreakpointAdd( int , QString );
    void onBreakpointDelete( int , QString );
    void onToggleBreakpoint(const QString &, const int &);

    void onBreakpointConditionned(int, QString);
    void onBreakpointUnConditionned(int, QString);
    void onToggleConditionnedBreakpoint(const QString &, const int &, const QString &);

    void onRequestBreakpoint(const QString & );
    void onBreakpointPending(int, QString );
    
    /*!
        \sa GdbInterpreter
    */
    void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

    QString name();
    QPointer<QWidget> widget();
    QIcon icon();

    // gdb
    void gdbFinished();
    void gdbStarted();
    void gdbError();

    // target
    void targetLoaded(const int &, const QString &);
    void targetNoLoaded(const int &, const QString &);
    void targetRunning(const int &, const QString &);
    void targetStopped(const int &, const QString &);
    void targetExited(const int &, const QString &);

    // Parser
    void done(const int &, const QString &);
    void prompt(const int &, const QString &);

    
private:


    /*! 
        \sa GdbConnectTemplate
    */
    GdbConnectTemplate<GdbBreakpoint> Connect;
    /*! 
        \details  Define new type
    */
    typedef QHash<QString, QString> BREAKPOINT;

    /*! 
        \details  Define a list for all breakpoints
    */
    QList< BREAKPOINT *> B;

    /*! 
        \sa BaseInterpreter, GdbInterpreter
    */
    QPointer<BaseInterpreter> interpreterAddBreakpoint;
    
    /*! 
        \sa BaseInterpreter, GdbInterpreter
    */
    QPointer<BaseInterpreter> interpreterDelBreakpoint;

    /*! 
        \sa BaseInterpreter, GdbInterpreter
    */
    QPointer<BaseInterpreter> interpreterEnabledBreakpoint;
    /*! 
        \sa BaseInterpreter, GdbInterpreter
    */
    QPointer<BaseInterpreter> interpreterDisabledBreakpoint;

    /*! 
        \sa BaseInterpreter, GdbInterpreter
    */
    QPointer<BaseInterpreter> interpreterBreakpointPending;

    /*! 
        \sa BaseInterpreter, GdbInterpreter
    */
    QPointer<BaseInterpreter> interpreterConditionnedBreakpoint;

    /*! 
        \sa BaseInterpreter, GdbInterpreter
    */
    QPointer<BaseInterpreter> interpreterUnConditionnedBreakpoint;


    BREAKPOINT * findBreakpointByLine(QString, int);
    BREAKPOINT * findBreakpointByIndex(QString, int);
    void clearBreakpoints(QString);
    void clearAllBreakpoints();

    void breakpointMoved(const QString & , const int & , const int & );
    void desableBreakpointHit();

    QPointer<UIGdbBreakpoint> mWidget;
    QString mRawData;
    bool mForce;
    // for restore breakpoint when project is re-open
    QStringList mBreakpointAtOpened;
    GdbBreakpointThread *bThread;

signals:

    /*!
        \details Emit for indicats that this breakpoint is add, deleted or modified.
    */
    void toggleBreakpoint(const QHash<QString, QString> &, const  bool &);
    void removeAllBreakpoints();
    void gotoBreakpoint(QString, int);
    void saveBreakpoint(QStringList);
};

#endif
