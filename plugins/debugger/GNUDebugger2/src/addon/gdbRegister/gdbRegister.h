/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbRegister
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbResgiter class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBREGISTER_H
#define GDBREGISTER_H

#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"
#include "./ui/UIGdbRegister.h"

//! Implements all functions for show values of cpu registers.This class is an AddOn for GNU debugger

/**
\author xiantia
\version 1.3.2

This class show in TabWidget all values of all cpu registers.
*/
class GdbRegister : public GdbCore
{
	Q_OBJECT


public:

	GdbRegister(QObject * parent = 0);
	~GdbRegister();

public slots:

	/**
	 * \brief Calling when GdbParser class found correct interpreter (in this case interpreterRegister)
	 * and call onRegister function.
	 */
	void onRegister( int , QString );

	QString name();
	QPointer<QWidget> widget();
	QIcon icon();

	/**
	 * \brief This function call onRegister function.
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
	 * \brief This variable is a template for connect interpreterRegister and onRegister function
	 */
	GdbConnectTemplate<GdbRegister> Connect;
	/**
	 * \brief Pointer to interpreterRegister
	 */
	QPointer<BaseInterpreter> interpreterRegister;

	/**
	 * \brief Pointer to GdbSequencer contains all commands.
	 */
	QPointer<GdbSequencer> Sequencer;
	QPointer<UIGdbRegister> mWidget;

	int numRegister;

	/**
	 * \brief Toogle color if new value of register has changed.
	 */
	void showColor(QTreeWidgetItem *, QString );
};

#endif
