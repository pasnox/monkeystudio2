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
    \file gdbBacktrace.cpp 
    \date 12/05/09
    \author Xiantia
    \version 1.4.0
    \brief Implements all functions for parse backtrace commands.
    This class is an AddOn for GNU debugger
*/

/*

    GdbBacktrace class


    Xiantia@gmail.com

    for Debugger v1.4.0
*/

#include "gdbBacktrace.h"
#include "./ui/UIGdbBacktrace.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QtDebug>
/*!
* \details Create a new object
*/
GdbBacktrace::GdbBacktrace(QObject * parent) : GdbAddonBase(parent)
{
    GdbCore::KernelDispatcher()->add(this);
    GdbCore::Interpreter()->remove(name());
    GdbCore::RestoreLine()->remove(name());

    GdbCore::RestoreLine()->add(name(), "^#\\d+\\s.*" , ".*\\sat\\s.*:\\d+$");

    setEnabled(true);
    setWaitEndProcess(false);

    mCurrentLine = -1;
    mCurrentFile.clear();
    

    /*
        create new parser :

        cRegExpCmd = "bt"
        aRegExp = 
    */

    interpreterBacktrace = GdbCore::Interpreter()->add(
        name(),
        QRegExp("^bt"),
        QRegExp("^#\\d+\\s.*\\sat\\s.*:\\d+"),
        "^done,className=\"" + name() + "\",event=\"Backtrace\",answerGdb=\"");

    // connect interpreter to function
    Connect.add(this, interpreterBacktrace, &GdbBacktrace::onBacktrace);


    /*
        create new parser :

        cRegExpCmd = "info source"
        aRegExp = "(gdb) "
    */
    interpreterInfoSource = GdbCore::Interpreter()->add(
        name(),
        QRegExp("^info source"),
        QRegExp("^Located\\sin\\s(.*)"),
        "^done,className=\"" + name() + "\",event=\"Info-source\",answerGdb=\"");

    Connect.add(this, interpreterInfoSource, &GdbBacktrace::onInfoSource);

    Sequencer = new GdbSequencer(this);
    QList<SequencerCmd> s = QList<SequencerCmd>() << SequencerCmd("Backtrace", "bt") << SequencerCmd("Infosource", "info source") ; 
    Sequencer->add( name() , s);

    // Ui
    mWidget = new UIGdbBacktrace(qobject_cast<QWidget*>(this));
    mWidget->treeWidget->setAlternatingRowColors(true);
    connect( mWidget->treeWidget, SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )), this, SLOT(onItemDoubleClicked ( QTreeWidgetItem * , int  )));
}

//
/*!
* \details Delete mWidget
*/
GdbBacktrace::~GdbBacktrace()
{
    // clear backtrace icon in editor
    emit removeBacktrace();
//  GdbCore::BridgeEditor()->removeBacktrace();
}

QString GdbBacktrace::name()
{
    return "GdbBacktrace";
}

QPointer<QWidget> GdbBacktrace::widget()
{
    return (QPointer<QWidget>)( mWidget );
}

QIcon GdbBacktrace::icon()
{
    return QIcon(":/icons/backtrace.png");
}

/*!
* \details This function switches interpreterBacktrace attached to onBacktrace function or
* interpreterInfosource attached to onInfoSource function.
* \param i is a pointer to BaseInterpreter class.
* \param id is an Id of string.
* \param s is a string. 
*/
void GdbBacktrace::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
    Connect.call( i, id, s);
}

// Gdb status

void GdbBacktrace::gdbStarted()
{
    setWaitEndProcess(false);
    mCurrentLine = -1;
    mCurrentFile.clear();
    mWidget->treeWidget->clear();
}

//

void GdbBacktrace::gdbFinished()
{
    setWaitEndProcess(false);
    mCurrentLine = -1;
    mCurrentFile.clear();
    mWidget->treeWidget->clear();

    emit removeBacktrace();
//  GdbCore::BridgeEditor()->removeBacktrace();
}

//

void GdbBacktrace::gdbError()
{
    emit removeBacktrace();
//  GdbCore::BridgeEditor()->removeBacktrace();
}

// Taget status

void GdbBacktrace::targetLoaded(const int &, const QString &){}

//

void GdbBacktrace::targetNoLoaded(const int &, const QString &){}

//

void GdbBacktrace::targetRunning(const int & , const QString & )
{
    // just delete bactrace marker in editor when target is running
    //emit onToggleBacktrace("", -1);
//  GdbCore::BridgeEditor()->removeBacktrace();
    emit removeBacktrace();
}

//

void GdbBacktrace::targetStopped(const int & , const QString & )
{
    setWaitEndProcess(true);
    mWidget->treeWidget->clear();
    Sequencer->start();
}

//

void GdbBacktrace::targetExited(const int & , const QString & )
{
    // just delete backtrace marcker when target exited
//  emit onToggleBacktrace("", -1);
    emit removeBacktrace();
//  GdbCore::BridgeEditor()->removeBacktrace();
    mWidget->treeWidget->clear();
}

// Parser status


void GdbBacktrace::done(const int & id, const QString & st)
{
    if(id>0) mRawData += findValue(st,"answerGdb");
}

//

void GdbBacktrace::prompt(const int &, const QString & )
{
    if(isWaitEndProcess())
    {
        emit errorMessage("Error : " + mRawData, 5000);
//      GdbAddonBase::showMessage("Error : " + mRawData, 2500);
    }

    mRawData.clear();
    
    // promt from bactrace command
    if(Sequencer->currentCmd() == "Backtrace")
    {
        setWaitEndProcess(true);
        Sequencer->next();
    }

    if(Sequencer->currentCmd() == "Infosource")
    {
        setWaitEndProcess(false);
    }
}

// Interpreters

/*!
* \details Calling when interpreter() function found onBacktrace event attached with interpreterBacktrace
* \param s is a string from GdbParser class.
*/
void GdbBacktrace::onBacktrace(int , QString s)
{
    /*
        see all backtrace in Ui
    */
    
    if(interpreterBacktrace->getAnswerRegExp().exactMatch(findValue(s,"answerGdb")))
    {
        QTreeWidgetItem * i = new QTreeWidgetItem(mWidget->treeWidget);
        i->setText(0,findValue(s,"answerGdb"));
    }

    /*
        just extract current backtrace
        #0  qMain (argc=1, argv=0x3d4c20) at src/main.cpp:65
    */

    // fixed 1.4.0
    //  QRegExp r("^#0.*\\sat\\s([^:]+):(\\d+)$");
    QRegExp r("^#0\\s.*\\sat\\s(.*):(\\d+)$");

    if(r.exactMatch(findValue(s,"answerGdb")))
    {
        setWaitEndProcess(false);
        QStringList l = r.capturedTexts();
        mCurrentLine = l.at(2).toInt();
        mCurrentFile = l.at(1);
        setSharedData("file", mCurrentFile);
        setSharedData("line", QString::number(mCurrentLine));
    }

}

/*!
* \details Calling when interpreter) function found onInfoSource event attached with interpreterInfosource
* \param s is a string from GdbParser class.
*/
void GdbBacktrace::onInfoSource(int , QString s)
{
    /*
        Info source receiver
    */
    QRegExp r = interpreterInfoSource->getAnswerRegExp();

    if(r.exactMatch(findValue(s,"answerGdb")))
    {
        setWaitEndProcess(false);
        //get the full path of file
        QStringList l = r.capturedTexts();
        mCurrentFile = l.at(1);
        setSharedData("file", mCurrentFile);
        setSharedData("line", QString::number(mCurrentLine));
        emit toggleBacktrace(mCurrentFile, mCurrentLine);
    }
}

/*!
* \details Calling when an editor is opened. Restore backtrace icon in this editor. 
* If mCurrentfile is a same as the name of the opened file, toggleBacktrace signal is 
* emitted indicating that Gdb is stopped on this file. The editor show icon backtrace on the margin.
* \param s is name of file that request backtrace icon.
*/
void GdbBacktrace::onRequestBacktrace(const QString & s)
{
    if(s == mCurrentFile)
        emit toggleBacktrace(mCurrentFile, mCurrentLine);
}

void GdbBacktrace::onItemDoubleClicked ( QTreeWidgetItem *i , int  c)
{
    QRegExp r("^#\\d+\\s.*\\sat\\s(.*):(\\d+)$");
    if(r.exactMatch(i->text(c)))
    {
        QStringList l = r.capturedTexts();
        emit gotoBacktrace(l.at(1), l.at(2).toInt());
    }
}