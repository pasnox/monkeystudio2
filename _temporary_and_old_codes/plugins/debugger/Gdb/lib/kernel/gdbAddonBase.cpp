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
	\file gdbAddonBase.cpp
	\date 15/05/09
	\author xiantia
	\version 1.4.0
	\brief Derived by all Addons
*/

#include "gdbAddonBase.h"
#include <QtDebug>
/*!
* \details Create new object
* \param parent of this object
*/
GdbAddonBase::GdbAddonBase(QObject * parent) : QObject (parent)
{
	watchDog.setSingleShot(true);
	connect(&watchDog, SIGNAL(timeout()), this , SLOT(onTimer()),Qt::DirectConnection);
	
	setWantAllMessages(false);
	setEnabled(true);
	setWaitEndProcess(false);
	mTime = 15000; // 15 seconds
}

/*!
* \details None
*/
GdbAddonBase::~GdbAddonBase()
{
}

/*!
 * \details Show message in Monkey Studio status bar.
 * \param string is string to show
 * \param time is time in seconde
 * \param brush is brush color
 */
/*void GdbAddonBase::showMessage(QString string, int time)
{
	MonkeyCore::statusBar()->appendMessage( string, time );
}

void GdbAddonBase::setMessage(QString s)
{
	MonkeyCore::statusBar()->setMessage( s);
}
*/	
	
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
 * Connect.call(pInter, stId, string);
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
 * \param pInter is a pointer to the interpreter found.
 * \param stId is the unic id of string.
 * \param string is the string formated from GdbParser class.
 * \sa GdbInterpreter class.
 */
void GdbAddonBase::interpreter(const QPointer<BaseInterpreter> & pInter , const int & stId , const QString & string)
{
	Q_UNUSED(pInter);
	Q_UNUSED(stId);
	Q_UNUSED(string);
}

/*!
 * \details Set enabled or not this AddOn.
 *
 * AddOn can be enabled or disable, in this case AddOn is not show in GNU debugger TabWidget,
 * and AddOn not receive data from external class.
 * \param b is true, the AddOn is enable, else not.
 */
void GdbAddonBase::setEnabled(const bool & b ) 
{ 
	mEnabled = b;
}

/*!
 * \details Return the current state of AddOn.
 * \retval true if AddOn is enable, else false
 */
bool GdbAddonBase::isEnabled() 
{ 
	return mEnabled;
}

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
 * // send data to Gdb
 * ...
 * ...
 * // data from Gdb
 * setWaitEndProcess(false); // stop watch before X secondes, default is 5 seconds (default).
 * \endcode
 * You can change the time by setWatchDogTime() function
 * \param b is true, this indicate AddOn has send data to Gdb and wait answer from it.
 * \sa isWaitEndProcess(), setWatchDogTime(), onTimer()
*/
void GdbAddonBase::setWaitEndProcess(const bool & b)
{ 
	if(b ) 
		watchDog.start(mTime); 
	else watchDog.stop();
	mWaitEndProcess = b;
}

/*!
 * \details Return the current state of wait end process
 * \retval true if AddOn wait answer from Gdb
 * \sa setWaitEndProcess(), setWatchDogTime(), onTimer()
 */
bool GdbAddonBase::isWaitEndProcess() { return mWaitEndProcess;}

/*!
 * \details Slot for watchDog.
 *
 * This function is calling automatly by the internal watchDog if the time out occure.
 * \sa isWaitEndProcess(), setWaitEndProcess(), setWatchDogTime()
 */
void GdbAddonBase::onTimer()
{
	if(isWaitEndProcess())
	{
		emit errorMessage("Detecting kernel panic : " + name() , 5000);
//		showMessage("Detecting kernel panic : " + name() , 2500);
		setWaitEndProcess(false);
	}
}

/*!
 * \details Return value in Gdb string
 *
 * \code
 * QString string = ^done,interpreter="AddOnName",event="Breakpoint-deleted",answerGdb="Error, not have breakpoint number 3";
 * finValue(string, "answerGdb"); // return "Error, not have breakpoint number 3"
 * \endcode
 *
 * \param st is a format string from GdbParser Class
 * \param key is the tag that you find
 * \retval QString contains the value of key
*/

QString check(QString s)
{
	bool c=0;
	for(int i=0; i<s.length()-1; i++)
	{
		if(s.at(i) == '"') c = !c;
		if(!c && s.at(i+1) == ',') return s.left(i);
	}
	return s;
}

QString GdbAddonBase::findValue(const QString & st , const QString & key)
{
/* fixed bug in v1.4.0 */
//	QString k = s.left(s.indexOf("="));
	if(st.indexOf(key) == -1) return QString();
	QString v = st.right(st.length() - st.indexOf(key) - key.length() - 1);
/*	qDebug()  << "================";
	qDebug() << key;
	qDebug() << v;
	qDebug() << check(v).remove(0,1);
*/
	return check(v).remove(0,1);

/* not work bug */
/*	QStringList l = st.split("\",");
	
	foreach(QString s, l)
	{
		QString k = s.left(s.indexOf("="));
		QString v = s.right(s.length() - s.indexOf("=") - 1);
		if(k == key )
		{
			
		}//	return v.remove("\"");
	}
	return QString();
*/
}

/*!
	\details Set the time for watchDog
	\param t is the time for watchDog. If no answer occure from GdbParser and time out, a message is show Monkey status bar. 
	\sa setWaitEndProcess(), isWaitEndProcess(), onTimer()
*/
void GdbAddonBase::setWatchDogTime(int t)
{
	mTime = t;
}
