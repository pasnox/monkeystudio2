/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbSequencer
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbSequencer class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/


#ifndef GDBSEQUENCER_H
#define GDBSEQUENCER_H

#include "gdbParser.1.3.h"
#include "gdbProcess.1.3.h"

#include <QObject>
#include <QPointer>

//! Store in list all command 
/**
\author xiantia
\version 1.3.2

This class store your command in list
*/
class SequencerCmd
{

	friend class GdbSequencer;

public:
	SequencerCmd(QString n, QString c)
	{
		name = n;
		cmd = c;
	}

private:
	QString name;
	QString cmd;
};

//! Execute commands step by step
/**
\author xiantia
\version 1.3.2

Store your commands, for example is you have a sequence of commands, you can add this in GdbSequencer class

\code
QList<SequencerCmd> s ;
s << SequencerCmd("processWathis","whatis varName") << SequencerCmd("processPrint","p varName");

GdbSequencer::add(ClassName, s);

GdbSequencer::start();
\endcode

This code send "whatis varName", now you have answer from Gdb, when you have finish process whatis,
 call GdbSequencer::loop() for send now "p varName" to Gdb.

You can start Sequencer at the second command if you want by call GdbSequencer::start(1). If you want no execute a command, use GdbSequencer::skipLoop(),
the current command is skiped.
*/
class GdbSequencer : public QObject
{
	Q_OBJECT


public :

	GdbSequencer(QObject * parent);
	~GdbSequencer();

	//! Add command to the end of command list
	void add(const SequencerCmd & );

	//! Add new command list
	/*
	\ note The last command list is cleared.
	*/
	void add(const QString & , const QList<SequencerCmd> &);
	//! Send the first command to gdb 
	/**
	Use loop() function for send the next command to gdb
	*/
	void start();
	//! Same as start() function , but start sequence at index
	void start(const int &);
	//! remove command list
	void remove();
	//! Remove just a command
	void remove(const SequencerCmd & );

	//! execute next command
	void loop();
	//! skip command
	void skipLoop();
	//! Change command by an other
	void change(const SequencerCmd &, const SequencerCmd &);
	//! Same as change(const SequencerCmd &, const SequencerCmd &)
	void change(const QString &, const QString &);
	
	//! Current command sended
	QString currentCmd();

private :

	//! Current command index
	int mCurrentIndex; 
	//! Pointer to GdbParser class
	QPointer<GdbParser> mParser;
	//! Pointer to GdbProcess class
	QPointer<GdbProcess> mProcess;

	//! Current Class name
	QString mClassName;

	//! List of all commands
	QList<SequencerCmd> mCmdList;
};

#endif
