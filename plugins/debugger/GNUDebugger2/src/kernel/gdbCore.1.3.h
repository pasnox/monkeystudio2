/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbKernelDispatcher
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/


#ifndef GDBCORE_H
#define GDBCORE_H

#include <QTimer>

#include "gdbConnectTemplate.1.3.h"
#include "gdbInterpreter.1.3.h"
#include "gdbParser.1.3.h"
#include "gdbProcess.1.3.h"


#include <coremanager.h>
#include <settingsmanager.h>
#include <monkey.h>
#include <queuedstatusbar.h>

//! Enum for show message to the status bar
enum SHOW{ _WARNING_,  _CRITICAL_, _INFO_};


//! Derived by all Addon
/** 
\author xiantia
\version 1.3.2

This class is derived by all Addon. It is the basic class proposing the basic functions.
*/

class GdbCore : public QObject
{
	Q_OBJECT

public:

	GdbCore(QObject * parent = 0);
	~GdbCore();

	
	/**
	 * \brief Return a pointer to Parser class
	 *
	 *
	 * \return Pointer to Parser class
	 * 
	 */
	QPointer<GdbParser> Parser() {  return mParser;}
	
	/**
	 * \brief Return a pointer to Process class
	 *
	 *
	 * \return Pointer to Process class
	 * 
	 */
	QPointer<GdbProcess> Process() {  return mProcess;}


public slots:

	/**
	 * \brief Calling when Gdb is started 
	 */
	virtual void gdbStarted() = 0;


	/**
	 * \brief Calling when Gdb is finished
	 */
	virtual void gdbFinished() = 0;

	/**
	 * \brief Calling when Gdb has generate an error. Gdb crash for example.
	 */
	virtual void gdbError() = 0;

	/**
	 * \brief Calling when target is loaded correctly 
	 */
	virtual void targetLoaded(const int & , const QString & ) = 0;

	/**
	 * \brief Calling when target is not loaded (format of target not correct).
 	 */
	virtual void targetNoLoaded(const int &, const QString &) = 0;

	/**
	 * \brief Calling when target running.
 	 */
	virtual void targetRunning(const int &, const QString &) = 0;
	
	/**
	 * \brief Calling when target is stopped.
	 Reason of stopped can be breakpoint or step over command
 	 */
	virtual void targetStopped(const int &, const QString &) = 0;
	
	/**
	 * \brief Calling when target exited.
 	 */
	virtual void targetExited(const int &, const QString &) = 0;

	/**
	 * \brief Calling when Parser class found an error.
	 * \param id is an id of error and s is a string error from Parser file
 	 */
 	virtual void error(const int &id, const QString &s)= 0;
	
	/**
	 * \brief Calling when Parser class can't found string in parser file
	 * \param id egal -1 and s is a string Done from Parser file
 	 */
	virtual void done(const int &id, const QString &s) = 0;

	/**
	 * \brief Calling when Parser class found an information
	 Generaly this function is call when Parser class has found string in parser file
	 * \param id is id of information and s is a string information from Parser file
 	 */
	virtual void info(const int &, const QString &) = 0;

	/**
	 * \brief Calling when Parser class found prompt from Gdb
	 * \param id egal 0 and s is a string Promp from Parser file
 	 */
	virtual void prompt(const int &, const QString &) = 0;

	/**
	 * \brief Calling when AddOn is created. 
	 * \retval name of your AddOn, it show in GNU debugger TabWidget
	*/
	virtual QString name() = 0; 

	/**
	 * \brief Calling when AddOn is created. 
	 * \retval widget container of AddOn, it is show in GNU debugger TabWidget
	 * \note If AddOn not have widget container, this funtion can return NULL
	*/
	virtual QPointer<QWidget> widget() = 0;

	/**
	 * \brief Calling when AddOn is created. 
	 * \retval icon of your AddOn, it is show in GNU debugger TabWidget
	*/
	virtual QIcon icon() = 0;

	/**
	 * \brief Calling when Parser class found an interpreter.
	 *
	 * An interpreter is a small parser for one command and one answer from Gdb.
	 * For example when you to send a command has Gdb, Gdb answers by a specific string.
	 * If you want to be on this answer and to call a function of your class, then creates an interpreter and connect your function has it.
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
	 * onValue function is only and automatically calls, when you send your command and that the answer of Gdb is correctly parsed.
	 *
	 * \note Connect is GdbConnectTemplate class.
	 *
	 * An interpreter can also modify the answer of Gdb.
	 * For example when you remove a breakpoint, Gdb answer just by prompt event and you have any information on what it passed. 
	 * You can modify the answer of Gdb by specifying this one in answerExtention string.
	*/
	virtual void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & ) = 0;

	/**
	 * \brief Set enabled AddOn.
	 *
	 * AddOn can be enabled or disable, in this case AddOn is not show in GNU debugger TabWidget,
	 * and AddOn not recieve data from external.
 	 */
	void setEnabled(const bool &  );
	
	/**
	 * \brief Return the current state of AddOn.
	 */
	bool isEnabled() ;

	/**
	 * \brief Return value in Gdb string
	 * \param string is a format string from Parser Class, key is a tag that you find.
 	 *
	 * \code
	 * QString string = ^info,interpreter="AddOnName",event="Breakpoint-deleted",answerGdb="Error, not have breakpoint number 3";
	 * finValue(string, "answerGdb"); // return "Error, not have breakpoint number 3"
	 * \endcode
	 *
	 * \note The quotation marks ' " ' are not turned over and they are removed if the string comprises some.
	 *
	*/
	static QString findValue(const QString & string , const QString & key);
	
	/**
	 * \brief Show message in Monkey Studio status bar.
	 * \param string to show, t is time in seconde, SHOW is brush color
	 */
	static void showMessage(QString, int, SHOW);

	/**
	 * \brief Return the current state wantAllMessages
	 */
	bool wantAllMessages(){ return mWantAllMessages;}
	
	/**
	 * \brief Set Addon want all datas from all AddOn
	 *
	 * By default AddOn does not receive that the data in answer has your command. 
	 * You can receive the data of other AddOn by specifying it by this function. 
	 * That can be interesting if you want create a log.
	 *
	 * Default is false
	 */
	void setWantAllMessages(bool b){ mWantAllMessages = b;}

protected :

	/**
	 * \brief Set wait end process (using by watchDog).
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
	 * setWaitEndProcess(false); // stop watch before 5 secondes
	 * \endcode
	*/
	void setWaitEndProcess(const bool & );

	/**
	 * \brief Return the current state of wait end process
	 */
	bool isWaitEndProcess();



private slots:

	/**
	 * \brief Slot for watchDog.
	 *
	 * This function is calling automatly by the internal watchDog if setWaitEndProcess is true.
	 */
	void onTimer();
	
private : 

	QPointer<GdbParser> mParser;
	QPointer<GdbProcess> mProcess;
	bool mEnabled;
	bool mWaitEndProcess;
	QTimer watchDog;
	bool mWantAllMessages;
};

#endif
