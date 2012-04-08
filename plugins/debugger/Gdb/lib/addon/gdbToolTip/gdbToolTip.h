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
    \file gdbToolTip.h
    \date 16/05/09
    \author Xiantia
    \version 1.4.0
    \brief Show the value of a variable in Tooltip. This class is an AddOn for GNU debugger.
*/
/*

    GdbToolTip class


    Xiantia@gmail.com

    for Debugger v1.4.0
*/

#ifndef GDBTOOLTIP_H
#define GDBTOOLTIP_H

#include <QObject>
#include <QtCore>
#include <Qt>
#include <QtGui>

#include "../../kernel/gdbCore.h"
#include "../../kernel/gdbAddonBase.h"
#include "../../kernel/gdbSequencer.h"
#include "../../kernel/gdbKernelDispatcher.h"
#include "../../kernel/gdbProcess.h"
#include "../../kernel/gdbControler.h"
/*!
    \brief Show the value of a variable in Tooltip. This class is an AddOn for GNU debugger.
    \details When the user selects text in the editor, the function onRequestShowVar is called (use GdbConnectTemplate and GdbInterpreter class). 
With this call there sendings "print valueName" to Gdb (use GdbSequencer class). In answer, onValue function is called and
ToolTip is show with a value of variable.

*/

class GdbToolTip : public GdbAddonBase
{
    Q_OBJECT


public:

    GdbToolTip(QObject * parent = 0);
    ~GdbToolTip();

public slots:

    void onValue( int , QString );
    void onType( int , QString );
    void onRequestShowVar(const QString &);

    QString name();
    QPointer<QWidget> widget();
    QIcon icon();

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

    void onTimer();

private:

    /*!
        \sa GdbConnectTemplate
    */
    GdbConnectTemplate<GdbToolTip> Connect;

    /*!
        \sa BaseInterpreter, GdbParser::addInterpreter()
    */
    QPointer<BaseInterpreter> interpreterValue;

    /*!
        \sa BaseInterpreter, GdbParser::addInterpreter()
    */
    QPointer<BaseInterpreter> interpreterType;

    /**
     * \sa GdbSequencer
    */
    QPointer<GdbSequencer> Sequencer;

    bool enteredShowVar;
    QString mRawData;
    QString mValue;
    QString mType;
    QString mToolTipString;
    QString mCurrentVarName;
    QTimer timer;
    int mLatence;
};

#endif
