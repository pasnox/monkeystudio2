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
	\file gdbCore.h
	\date 2008-01-14T00:27:39
	\author xiantia
	\version 1.3.2
	\brief Derived by all Addon
*/

#ifndef GDBCORE_H
#define GDBCORE_H

#include <QTimer>

#include "gdbConnectTemplate.h"
#include "gdbInterpreter.h"
#include "gdbParser.h"
#include "gdbProcess.h"


#include <coremanager.h>
#include <settingsmanager.h>
#include <monkey.h>
#include <queuedstatusbar.h>

/*!
\details Enum for show message to the status bar
*/
enum SHOW{ _WARNING_,  _CRITICAL_, _INFO_};


/*!
 \brief Derived by all AddOn
 \details This class interface of the functions basics allowing the creation of Addon.
 It gathers functions in three big families. Functions associated has the target (targetStopped, targetRunning, targetExited, 
 targetLoaded and targetNoLoaded) , functions associated has Gdb (gdbStarted, gdbFinished, gdbExited) and functions associated with the commands (error, info, done, prompt).
 
 \note All functions are called by GdbParser class, dispatcher by GdbKernelDispatcher via DockGNUDebugger class.

  An addOn adds new functions to the plugin GNU Debugger. For this just derive this class and re-implements all virtual functions.
 
 myAddon.h
 \code
 class myAddon : public GdbCore
 {
	myAddOn(QObject *parent = 0);
	
	// from Target
	void targetLoaded();
	void targetNoLoaded();
	void targetStopped();
	void targetRunning();
	void targetExited();

	// from Command
	void done(...);
	void info(...);
	void error(...);
	void prompt(...);

	// from Gdb
	void gdbStarted();
	void gdbFinished();
	void gdbError();

	QString name();
	...
}
 \endcode
 myAddon.cpp
 \code
 // constructor

 myAddon::myAddon(QOject *parent) : GdbCore(parent){}

 // re-implements virtual functions

 void myAddon::targetLoaded() {}
 void myAddon::targetNoLoaded() {}
 void myAddon::targetStopped() {}
 void myAddon::targetRunning() {}
 void myAddon::targetExited(){ }
 
 void myAddon::done(const int & id, const QString & st) {}
 void myAddon::error(const int & id, const QString & st) {}
 void myAddon::info(const int & id, const QString & st) {}
 void myAddon::prompt(const int & id, const QString & st) {}

 void myAddon::gdbStarted() {}
 void myAddon::gdbFinished() {}
 void myAddon::gdbError() {}
 
 QString myAddon::name()
 {
	return "myAddOnName";
 }

  ...
 \endcode

 After create instance of this AddOn in DockGNUDebugger and add AddOn in GdbKernelDispatcher class.

 In DockGNUDebugger.cpp 
 \code
 	// create instance
	addOn = new myAddon(this);
	// addOn to dispatcher
	Dispatcher->add(addOn);
 \endcode
*/

class GdbCore : public QObject
{
	Q_OBJECT

public:

	GdbCore(QObject * parent = 0);
	~GdbCore();

	/*!
	 * \details Get the GdbParser class.
	 * 
	 * This is used for access to GdbParser class.
	 * For example store the commmand in GdbParser class and send data to Gdb.
	 *
	 * \code
	 * Parser()->setNextCommand( name(), "command has to send"); 
	 * Process()->sendRawData("command has to send");
	 * \endcode
	 * \retval Pointer to GdbParser class
	 * \sa GdbParser
	 */
	QPointer<GdbParser> Parser() {  return mParser;}
	
	/*!
	 * \details Get the GdbProcess class.
	 *
	 * This is used for access to GdbProcess class.
	 * For example store the commmand in GdbParser class and send data to Gdb.
	 *
	 * \code
	 * Parser()->setNextCommand( name(), "command has to send"); 
	 * Process()->sendRawData("command has to send");
	 * \endcode
	 * \retval Pointer to GdbProcess class
	 * \sa GdbProcess
	 */
	QPointer<GdbProcess> Process() {  return mProcess;}


public slots:

	/*!
	 * \details Calling when Gdb is correctly started.
	 * \sa gdbFinished(), gdbError()
	 */
	virtual void gdbStarted() = 0;

	/*! \details Calling when Gdb is correctly finished.
	 * \sa gdbStarted(), gdbError()
	 */
	virtual void gdbFinished() = 0;

	/*!
	 * \details Calling when Gdb has generated an error. Gdb crash for example.
	 * \sa gdbFinished(), gdbStarted()
	 */
	virtual void gdbError() = 0;

	/*!
	 * \details Calling when target is loaded correctly.
	 * \param stId is a unic id corresponding of string.
	 * \param string is a string formated.
	 * \sa targetNoLoaded(), targetRunning(), targetStopped(), targetExited()
	 */
	virtual void targetLoaded(const int & stId, const QString & string) = 0;

	/*!
	 * \details Calling when target is not loaded (format of target not correct).
	 * \param stId is a unic id corresponding of string.
	 * \param string is a string formated.
	 * \sa targetLoaded(), targetRunning(), targetStopped(), targetExited()
 	 */
	virtual void targetNoLoaded(const int & stId, const QString & string) = 0;

	/*!
	 * \details Calling when target running.
	 * \param stId is a unic id corresponding of string.
	 * \param string is a string formated.
	 * \sa targetLoaded(), targetNoLoaded(), targetStopped(), targetExited()
 	 */
	virtual void targetRunning(const int & stId, const QString & string) = 0;
	
	/*!
	 * \details Calling when target is stopped. Reason of stopped can be breakpoint or step over command.
	 * \param stId is a unic id corresponding of string.
	 * \param string is a string formated.
	 * \sa targetLoaded(), targetNoLoaded(), targetRunning(), targetExited()
 	 */
	virtual void targetStopped(const int & stId, const QString & string) = 0;
	
	/*!
	 * \details Calling when target exited.
	 * \param stId is a unic id corresponding of string.
	 * \param string is a string formated.
	 * \sa targetLoaded(), targetNoLoaded(), targetRunning(), targetStopped()
 	 */
	virtual void targetExited(const int & stId, const QString & string) = 0;

	/*!
	 * \details Calling when GdbParser class found an error.
	 * \param stId is a unic id corresponding of string.
	 * \param string is a string formated.
	 * \sa done(), info(), prompt()
	 */
 	virtual void error(const int & stId, const QString & string) = 0;
	
	/*!
	 * \details Calling when GdbParser class can't found string in parse file
	 * \param stId is a unic id corresponding of string.
	 * \param string is a string formated.
	 * \sa error(), info(), prompt()
 	 */
	virtual void done(const int & stId, const QString & string) = 0;

	/*!
	 * \details Calling when GdbParser class found an information. Generaly this function is call when GdbParser 
	 * class has found string in parse file.
	 * \param stId is a unic id corresponding of string.
	 * \param string is a string formated.
	 * \sa error(), done(), prompt()
 	 */
	virtual void info(const int & stId, const QString & string) = 0;

	/*!
	 * \details Calling when GdbParser class found prompt from Gdb.
	 * \param stId is a unic id corresponding of string. In this case id egal 0
	 * \param string is a string formated.
 	 */
	virtual void prompt(const int & stId, const QString & string) = 0;

	/*!
	 * \details Calling when AddOn is created. 
	 * \retval Name of your AddOn, it's can show in GNU debugger TabWidget
	*/
	virtual QString name() = 0; 

	/*!
	 * \details Calling when AddOn is created. 
	 * \retval Widget container of AddOn, it's can show in GNU debugger TabWidget
	 * \note If AddOn not have widget container, this funtion can return NULL.
	*/
	virtual QPointer<QWidget> widget() = 0;

	/*!
	 * \details Calling when AddOn is created. 
	 * \retval Icon of your AddOn, it's can show in GNU debugger TabWidget
	 * \note If widget() function return NULL, this function is not call.
	*/
	virtual QIcon icon() = 0;
	virtual void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & ) = 0;
	void setEnabled(const bool &  );
	bool isEnabled() ;
	static QString findValue(const QString & , const QString &);
	static void showMessage(QString, int, SHOW);
	void setWatchDogTime(int);

	/*!
	 * \details Return the current state wantAllMessages.
	 * \sa setWantAllMessages()
	 */
	bool wantAllMessages(){ return mWantAllMessages;}
	
	/*!
	 * \details Set Addon want all datas from all AddOn
	 *
	 * By default AddOn does not receive that the data in answer has your command. 
	 * You can receive the data of other AddOn by specifying it by this function. 
	 * That can be interesting if you want create a log.
	 *
	 * Default is false
	 * \param b is true if you want receive all data from Gdb
	 * \sa wantAllMessages()
	 */
	void setWantAllMessages(bool b){ mWantAllMessages = b;}

protected :

	void setWaitEndProcess(const bool & );
	bool isWaitEndProcess();


private slots:

	void onTimer();
	
private : 
	/*!
		\details Private var, pointer to GdbParser.
		\sa Parser()
	*/
	QPointer<GdbParser> mParser;

	/*!
		\details Private var, pointer to GdbProcess.
		\sa Process()
	*/
	QPointer<GdbProcess> mProcess;
	
	/*!
		\details Private var, AddOn is enbaled.
		\sa isEnabled()
	*/
	bool mEnabled;

	/*!
		\details Private var, AddOn is waiting answer.
		\sa setWaitEndProcess(), isWaitEndProcess()
	*/
	bool mWaitEndProcess;


	/*!
		\details QTimer for watchDog
		\sa onTimer(), setWatchDogTime(),setWaitEndProcess(), isWaitEndProcess()
	*/
	QTimer watchDog;

	/*!
		\details Private var, AddOn wan all data from Gdb.
		\sa setWantAllMessages(), wantAllMessages()
	*/
	bool mWantAllMessages;

	/*!
		\details Time for watchDog
		\sa setWatchDogTime()
	*/
	int mTime;
};

#endif
