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
	\file gdbCore.cpp
	\date 2008-01-14T00:27:39
	\author xiantia
	\version 1.3.2
	\brief Derived by all Addon
*/

#include "gdbCore.h"

/*!
* \details Create new object, and get GdbParser and GdbProcess class
* \param parent of this object
*/
GdbCore::GdbCore(QObject * parent) : QObject (parent)
{
	mParser = GdbParser::instance(); 
	mProcess = GdbProcess::instance(); 

	watchDog.setSingleShot(true);
	connect(&watchDog, SIGNAL(timeout()), this , SLOT(onTimer()));
	
	setWantAllMessages(false);
	setEnabled(true);
	setWaitEndProcess(false);
	mTime = 5000;
}

/*!
* \details None
*/
GdbCore::~GdbCore()
{
}

/*!
 * \details Show message in Monkey Studio status bar.
 * \param s is string to show
 * \param t is time in seconde
 * \param c is brush color
 */
void GdbCore::showMessage(QString s, int t, SHOW c)
{
	QColor b;
	switch(c)
	{
	case _WARNING_ : b.setRgb(240,240,100); break;
	case _INFO_ : b.setRgb(120,250,100); break;
	case _CRITICAL_ : b.setRgb(255,35,35);

	}

	MonkeyCore::statusBar()->appendMessage( s, t ,QPixmap(), QBrush(b));
}

/*!
 * \details Calling when GdbParser class found an interpreter.
 *
 * An interpreter is a small parser for one command and one answer from Gdb.
 * For example when you to send a command has Gdb, Gdb answers by a specific string.
 * If you want to be on this answer and to call a function of your class, then creates an 
 * interpreter and connect your function has it.
 *
 * In your constructor class :
 * \code
 * interpreterBacktrace = Parser()->addInterpreter(
 * 	name(), // name of AddOn
 * 	QRegExp("^bt"), // command send to Gdb
 * 	QRegExp("^#\\d+\\sw+\\sat\\s.*:\\d+"), // parse just your line, in this case "#1 0x345FD at main.cpp:34"
 * 	"^info,interpreter=\"" + name() + "\",event=\"Backtrace\",answerGdb=\""); // answerExtention
 *
 * Connect.add(this, interpreterBacktrace, &AddOnClassName::onValue);
 * \endcode
 * 
 * In interpreter function :
 * \code
 * Connect.call(interpreter, id, string);
 * \endcode
 *
 * onValue function is only and automatically calls, when you send your 
 * command and that the answer of Gdb is correctly parsed.
 *
 * \note Connect is GdbConnectTemplate class.
 *
 * An interpreter can also modify the answer of Gdb.
 * For example when you remove a breakpoint, Gdb answer just by prompt event and you have any information on what it passed. 
 * You can modify the answer of Gdb by specifying this one in answerExtention string.
 *
 */
void GdbCore::interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & ){}

/*!
 * \details Set enabled or not this AddOn.
 *
 * AddOn can be enabled or disable, in this case AddOn is not show in GNU debugger TabWidget,
 * and AddOn not receive data from external class.
 * \param b is true, the AddOn is enable, else not.
 */
void GdbCore::setEnabled(const bool & b ) { mEnabled = b;}

/*!
 * \details Return the current state of AddOn.
 * \retval true if AddOn is enable, else false
 */
bool GdbCore::isEnabled() { return mEnabled;}

/*!
 * \details Set wait end process (using by watchDog).
 *
 * when you send a command has Gdb, this one must answer you.
 * To control that Gdb answered well, GdbCore uses a watchDog.
 * If no answer of Gdb occure in the 5 seconds, you have the message "Kernel panic : AddOn name" in Monkey status bar.
 *
 * Default is false
 *
 * \code 
 * setWaitEndProcess(true); // use watchDog
 * send data to Gdb
 * ...
 * ...
 * data from Gdb
 * setWaitEndProcess(false); // stop watch before X secondes, default is 5 seconds.
 * \endcode
 * You can change the time by setWatchDogTime() function
 * \param p is true, this indicate AddOn has send data to Gdb and wait answer from it.
*/
void GdbCore::setWaitEndProcess(const bool & p){ if(p ) watchDog.start(mTime); else watchDog.stop();/* qDebug() << "set wait " + QString::number(p) + " " +  name(); */ mWaitEndProcess = p;}

/*!
 * \details Return the current state of wait end process
 * \retval true if AddOn wait answer from Gdb
 */
bool GdbCore::isWaitEndProcess() { return mWaitEndProcess;}

/*!
 * \details Slot for watchDog.
 *
 * This function is calling automatly by the internal watchDog if setWaitEndProcess is true and the time out occure.
 */
void GdbCore::onTimer()
{
	if(isWaitEndProcess())
	{
		showMessage("Detecting kernel panic : " + name() , 2500, _WARNING_);
		setWaitEndProcess(false);
	}
}

/*!
 * \details Return value in Gdb string
 *
 * \code
 * QString string = ^info,interpreter="AddOnName",event="Breakpoint-deleted",answerGdb="Error, not have breakpoint number 3";
 * finValue(string, "answerGdb"); // return "Error, not have breakpoint number 3"
 * \endcode
 *
 * \note The quotation marks ' " ' are not turned over and they are removed if the string comprises some.
 *
 * \param st is a format string from GdbParser Class
 * \param key is the tag that you find
 * \retval QString contains the value of key
*/
QString GdbCore::findValue(const QString & st , const QString & key)
{
	QStringList l = st.split("\",");
	
	foreach(QString s, l)
	{
		QString k = s.left(s.indexOf("="));
		QString v = s.right(s.length() - s.indexOf("=") - 1);
		if(k == key ) return v.remove("\"");
	}
	return QString();
}

/*!
	\details Set the time for watchDog
*/
void GdbCore::setWatchDogTime(int t)
{
	mTime = t;
}
