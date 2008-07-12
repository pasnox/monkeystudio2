/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbCli
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbCli class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBCLI_H
#define GDBCLI_H

#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"
#include "./ui/UIGdbCli.h"

//! Implements all functions for send user command to Gdb.This class is an AddOn for GNU debugger

/**
\author xiantia
\version 1.3.2

This class can send user command to Gdb, this is a console for Gdb.
*/

class GdbCli : public GdbCore
{
	Q_OBJECT


public:

	GdbCli(QObject * parent = 0);
	~GdbCli();

public slots:

	/**
	 * \brief Calling when user click on "Send" button. This function send user command to Gdb.
	*/
	void onSend();

	QString name();
	QPointer<QWidget> widget();
	QIcon icon();

	/**
	 * \brief Not interpreter implemented for this class.
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

	QPointer<UIGdbCli> mWidget;
};

#endif
