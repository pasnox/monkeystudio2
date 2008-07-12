/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbToolTip
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbToolTip class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBTOOLTIP_H
#define GDBTOOLTIP_H

#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"

//! Show the value of a variable in Tooltip. This class is an AddOn for GNU debugger.

/**
\author xiantia
\version 1.3.2

When the user selects text in the editor, the function onRequestShowVar is called (use GdbConnectTemplate and GdbInterpreter class). 
With this call there sendings "print valueName" to Gdb (use GdbSequencer class). In answer, onValue function is called and
ToolTip is show with a value of variable.
*/

class GdbToolTip : public GdbCore
{
	Q_OBJECT


public:

	GdbToolTip(QObject * parent = 0);
	~GdbToolTip();

public slots:

	/**
	 * \brief This function is call when Gdb answered. This function show ToopTip at cursor post.
	 * \param Value of variable. 
	*/
	void onValue( int , QString );

	/**
	 * \brief This function is call when user select text in editor. This function send "print varName" to Gdb.
	 * \param var Name from editor 
	*/
	void onRequestShowVar(const QString &);

	QString name();
	QPointer<QWidget> widget();
	QIcon icon();

	/**
	 * \brief Calling when Parser class found an interpreter.
	 *
	 * An interpreter is a small parser for one command and one answer from Gdb.
	 * For example when you to send a command has Gdb, Gdb answers by a specific string.
	 * If you want to be on this answer and to call a function of your class, then creates an interpreter and connect your function has it.
	 *
	 * In your constructor class :
	 * \code
	 *	interpreterValue = GdbCore::Parser()->addInterpreter(
	 *	name(),
	 *	QRegExp("^print .*"), 
	 *	QRegExp("^\\$\\d+\\s+=\\s+.*"),
	 *	"^info,interpreter=\"" + name() + "\",event=\"WatchValue\",answerGdb=\"");
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
	void error(const int &, const QString &);
	void done(const int &, const QString &);
	void info(const int &, const QString &);
	void prompt(const int &, const QString &);

private:

	/**
	 * \brief This variable is a template for connect GdbToolTip::onValue() and interpreterValue.
	 * When onRequestShowVar function is call. it send "print variableName" to Gdb.
	 * The answer from Gdb is parse in GdbParser class, this class search an interpreter and found interpreterValue.
	 * Parser Class emit interpreter signal and call GdbToolTip::interpreter(..) function, in this it call Connect.call() function 
	 * and found onValue function attached with this interpreter.
	*/
	GdbConnectTemplate<GdbToolTip> Connect;
	QPointer<BaseInterpreter> interpreterValue;

	/**
	 * \brief This variable store all commands to send to Gdb. In this case Sequencer has just one command, "print variableName".
	*/
	QPointer<GdbSequencer> Sequencer;

	bool enteredShowVar;
	bool isGdbStarted;
	bool isTargetStopped;
};

#endif
