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
    \file gdbBacktrace.h 
    \date 15/05/09
    \author Xiantia
    \brief Implements all functions for parse backtrace commands. 
    This class is an AddOn for GNU debugger
    \version 1.4.0
*/

/*

    GdbBacktrace class


    Xiantia@gmail.com

    for Debugger v1.4.0
*/

#ifndef GDBBACKTRACE_H
#define GDBBACKTRACE_H

#include <QObject>
#include <QTreeWidgetItem>
#include "../../kernel/gdbCore.h"
#include "../../kernel/gdbAddonBase.h"
#include "../../kernel/gdbSequencer.h"
#include "../../kernel/gdbKernelDispatcher.h"
#include "../../kernel/gdbInterpreter.h"
class UIGdbBacktrace;


/*!
    \brief Implements all functions for parse backtrace commands.
    This class is an AddOn for GNU debugger
    \details  When target is stopped, this class search where is Gdb has stopped.
    For that it send "bt" and "info source" commands for extract the line number of file and the absolue path of file.

    GdbBacktrace thus will start by adding two interpreters has GdbParser:
    - interpreterBacktrace which goes parser the line " ^#\\d+\\s.*\\sat\\s.*:\\d+", i.e. all the lines which starts with #.
    \code
    #0 main (arc, arg) at src/main.cpp:23
    \endcode

    - interpreterInfosource which goes parser all the lines which starts with "^Located\\sin\\s.*"
    \code
    Located in \usr\local\bin\dev\test.cpp
    \endcode

    \note These two interpreters are not active which if their respective command are in court.

    Once created, this interpreters are connected has two functions:
    - onBacktrace which will extract from the string the number of the line where Gdb it is stopped.
    - onInfosource which will extract from the string the file name where Gdb is stopped.

    \note The two commands has to send has Gdb are directly stored in Sequencer

    When the sequence is finished the signal toggleBacktrace (mCurrentFile, mCurrentLine) is emitted.

    Another function is to add has GdbBacktrace class. When a new editor is opened, you can call onRequestBacktrace, those the purpose of which is to ask whether the editor had the backtrace just before closing/opening.
    If the name of the open file is the same one that mCurrentFile, then the signal toggleBacktrace is emitted.
*/

class GdbBacktrace : public GdbAddonBase
{
    Q_OBJECT

public:

    GdbBacktrace(QObject * parent = 0);
    ~GdbBacktrace();

public slots:

    void onBacktrace( int , QString );
    void onInfoSource( int , QString );
    void onRequestBacktrace(const QString &);

    QString name();
    QPointer<QWidget> widget();
    QIcon icon();

    /*!
        \sa GdbInterpreter
    */
    void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

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

    void onItemDoubleClicked ( QTreeWidgetItem * , int  );
private:

    /*!
        \sa GdbConnectTemplate
    */
    GdbConnectTemplate<GdbBacktrace> Connect;

    /*!
        \sa BaseInterpreter, GdbInterpreter
    */
    QPointer<BaseInterpreter> interpreterBacktrace;
    
    /*!
        \sa BaseInterpreter, GdbInterpreter
    */
    QPointer<BaseInterpreter> interpreterInfoSource;

    /*!
        \sa GdbSequencer
    */
    QPointer<GdbSequencer> Sequencer;

    QPointer<UIGdbBacktrace> mWidget;

    /*!
    * \details Current line where Gdb is stopped.
    */
    int mCurrentLine;
    /*!
    * \details Current file name where Gdb is stopped.
    */
    QString mCurrentFile;
    QString mRawData;
    
signals:

    /*!
     * \details This signal is emit for post backtrace icon on editor margin.
     * \param fileName of file
     * \param line number of file
    */
    void toggleBacktrace(const QString &fileName, const int &line);
    void removeBacktrace();
    void gotoBacktrace(QString,int);
};

#endif
