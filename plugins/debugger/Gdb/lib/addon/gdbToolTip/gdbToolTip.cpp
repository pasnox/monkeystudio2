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
    \file gdbToolTip.cpp
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

#include "gdbToolTip.h"
#include <QToolTip>

#include <QtDebug>
/*!
    \details Create new object
    \param parent of this object
*/
GdbToolTip::GdbToolTip(QObject * parent) : GdbAddonBase(parent),mLatence(500)
{

    GdbCore::KernelDispatcher()->add(this);
    GdbCore::Interpreter()->remove(name());

    setEnabled(true);
    setWaitEndProcess(false);
    enteredShowVar = false;

    interpreterValue = GdbCore::Interpreter()->add(
        name(),
        QRegExp("^print .*"), 
        QRegExp("^\\$\\d+\\s+=\\s+.*"),
        "^done,className=\"" + name() + "\",event=\"WatchValue\",answerGdb=\"");

    // connect interpreters to functions
    Connect.add(this, interpreterValue, &GdbToolTip::onValue);

    interpreterType = GdbCore::Interpreter()->add(
    name(),
    QRegExp("^whatis .*"), 
    QRegExp("^type\\s+=\\s+.*"),
        "^done,className=\"" + name() + "\",event=\"WatchType\",answerGdb=\"");

    // connect interpreters to functions
    Connect.add(this, interpreterType, &GdbToolTip::onType);


    // create sequencer 
    Sequencer = new GdbSequencer(this);

    timer.setSingleShot(true);
    connect(&timer,SIGNAL(timeout()), this, SLOT(onTimer()));
}

//

/*!
    \details None.
*/
GdbToolTip::~GdbToolTip()
{
}

//

QString GdbToolTip::name()
{
    return "GdbToolTip";
}

//

QPointer<QWidget> GdbToolTip::widget()
{
    return (QPointer<QWidget>)( NULL );
}

//

QIcon GdbToolTip::icon()
{
    return QIcon();
}

//

/**
 * \details Calling when Parser class found an interpreter.
 *
 * An interpreter is a small parser for one command and one answer from Gdb.
 * For example when you to send a command has Gdb, Gdb answers by a specific string.
 * If you want to be on this answer and to call a function of your class, then creates an interpreter and connect your function has it.
 *
 * In your constructor class :
 * \code
 *  interpreterValue = GdbCore::Parser()->addInterpreter(
 *  name(),
 *  QRegExp("^print .*"), 
 *  QRegExp("^\\$\\d+\\s+=\\s+.*"),
 *  "^info,interpreter=\"" + name() + "\",event=\"WatchValue\",answerGdb=\"");
 *
 * Connect.add(this, interpreterValue, &GdbToolTip::onValue);
 * \endcode
 * 
 * In interpreter function :
 * \code
 * Connect.call(interpreter, id, string);
 * \endcode
 *
 * onValue function is only and automatically calls, when you send your command and that the answer of Gdb is correctly parsed.
 *
 * \note Connect is GdbConnectTemplate class.
 *
 * An interpreter can also modify the answer of Gdb.
 * For example when you remove a breakpoint, Gdb answer just by prompt event and you have any information on what it passed. 
 * You can modify the answer of Gdb by specifying this one in answerExtention string.
*/

void GdbToolTip::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
    Connect.call( i, id, s);
}

// Gdb status

void GdbToolTip::gdbStarted()
{
    setWaitEndProcess(false);
}

//

void GdbToolTip::gdbFinished()
{
    setWaitEndProcess(false);
}

//

void GdbToolTip::gdbError(){}

// Taget status

void GdbToolTip::targetLoaded(const int &, const QString &){}
void GdbToolTip::targetNoLoaded(const int &, const QString &){}
void GdbToolTip::targetRunning(const int & , const QString & ){}
void GdbToolTip::targetStopped(const int & , const QString & ){}
void GdbToolTip::targetExited(const int & , const QString & ){}

// Parser status

void GdbToolTip::done(const int & id, const QString & st )
{
    if(id>0) mRawData += findValue(st,"answerGdb");
}

void GdbToolTip::prompt(const int &, const QString & )
{

    // promt from watch print command
    if(Sequencer->currentCmd() == "WatchValue")
    {
        if(isWaitEndProcess())
        {
            //possible error
            mToolTipString = "<u><center><b>" + mCurrentVarName + "</b></center></u><br><b><img src=\":/icons/func.png\" />";
            mToolTipString += " Value :</b><br>" + Qt::escape(mRawData);
        }
        else
        {
            mToolTipString = "<u><center><b>" + mCurrentVarName + "</b></center></u><br><b> <img src=\":/icons/var.png\" />";
            mToolTipString += " Value :</b><br>" + Qt::escape(mValue);
        }
        mRawData.clear();
        setWaitEndProcess(true);
        Sequencer->next();
        return;
    }


    if(Sequencer->currentCmd() == "WatchType")
    {
        if(isWaitEndProcess())
        {
            //possible error
            mToolTipString += "<br><br><b><img src=\":/icons/func.png\" />";
            mToolTipString += " Type :</b><br>" + Qt::escape(mRawData);
        }
        else
        {
            mToolTipString += "<br><br><b><img src=\":/icons/var.png\" />";
            mToolTipString += " Type :</b><br>" + Qt::escape(mType);
        }
        QToolTip::showText(QCursor::pos (), mToolTipString , NULL);
        mRawData.clear();
        enteredShowVar = false;
        mToolTipString.clear();
        setWaitEndProcess(false);
        Sequencer->remove();
    }
}

// Interpreters

/*!
 * \details This function is call when Gdb answered. This function show ToopTip at cursor post.
 * \param s is the value from GdbParser. 
*/
void GdbToolTip::onValue(int, QString s)
{
    QString v = findValue(s,"answerGdb");
    mValue = v.right(v.length() - v.indexOf(" = ") - 3); // delete -> $x = ....
    setWaitEndProcess(false);
}



/*!
 * \details This function is call when Gdb answered. This function show ToopTip at cursor post.
 * \param s is the value from GdbParser. 
*/
void GdbToolTip::onType(int, QString s)
{
    QString v = findValue(s,"answerGdb");
    mType = v.right(v.length() - v.indexOf(" = ") - 3); // delete -> type = ....
    setWaitEndProcess(false);
}

// slot from Qsci

/**
 * \details This function is call when user select text in editor. This function send "print varName" to Gdb.
 * \param n is the name of var from editor.
 * \sa GdbBridgeEditor::requestShowVar()
*/
void GdbToolTip::onRequestShowVar(const QString & n)
{
    if(!enteredShowVar && GdbCore::Controler()->isGdbStarted() && GdbCore::Controler()->isTargetStopped())
    {
        mCurrentVarName = n;
/* fixed 1.4.0 use now latency */
/*      enteredShowVar = true;
        mToolTipString.clear();
        QToolTip::hideText();

        QList<SequencerCmd> s;
        s  << SequencerCmd("WatchValue", "print " + n) <<  SequencerCmd("WatchType", "whatis " + n) ; 
        Sequencer->add(name() , s);
        Sequencer->start();
        setWaitEndProcess(true);
*/
        timer.stop();
        timer.start(mLatence);
    }
}

/**
 * \details Latency
 * 
*/
void GdbToolTip::onTimer()
{
    /* since v1.4.0 */

    enteredShowVar = true;
    mToolTipString.clear();
    QToolTip::hideText();

    QList<SequencerCmd> s;
    s  << SequencerCmd("WatchValue", "print " + mCurrentVarName) <<  SequencerCmd("WatchType", "whatis " + mCurrentVarName) ; 
    Sequencer->add(name() , s);
    Sequencer->start();
    setWaitEndProcess(true);
}
