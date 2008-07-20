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
	\file gdbSequencer.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Execute commands step by step
*/
/*

	GdbSequencer class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/


#ifndef GDBSEQUENCER_H
#define GDBSEQUENCER_H

#include "gdbParser.h"
#include "gdbProcess.h"

#include <QObject>
#include <QPointer>

/*!
	\brief Store in list all commands 
	\details This class store your command in list
*/
class SequencerCmd
{
	friend class GdbSequencer;

public:
	/*!
		\details Store only one command
		\param cmdName is the name of command sush as "my command"
		\param command is the command has to send to Gdb
	*/
	SequencerCmd(QString cmdName, QString command)
	{
		name = cmdName;
		cmd = command;
	}

private:
	QString name;
	QString cmd;
};

/*!
	\brief Execute commands step by step
	\details Store your commands, for example if you have a sequence of commands, 
	you can add this in GdbSequencer class

	\code
	// create list
	QList<SequencerCmd> s ;
	s << SequencerCmd("processWathis","whatis varName") << SequencerCmd("processPrint","p varName");
	// store list 
	GdbSequencer::add(ClassName, s);
	// Start command at the begining
	GdbSequencer::start();
	\endcode

	This code send "whatis varName", Gdb answer, when you have finish process whatis,
	 call GdbSequencer::loop() for send now "p varName" to Gdb.

	You can start Sequencer at the second command if you want by call GdbSequencer::start(1). 
	If you want no execute a command, use GdbSequencer::skipLoop(),
	the current command is skiped.
*/
class GdbSequencer : public QObject
{
	Q_OBJECT

public :

	GdbSequencer(QObject * parent);
	~GdbSequencer();

	void add(const SequencerCmd & );
	void add(const QString & , const QList<SequencerCmd> &);
	void start();
	void start(const int &);
	void remove();
	void remove(const SequencerCmd & );

	void loop();
	void skipLoop();
	void change(const SequencerCmd &, const SequencerCmd &);
	void change(const QString &, const QString &);
	
	QString currentCmd();

private :

	/*!
		\details Current command index
	*/
	int mCurrentIndex; 
	/*!
		\details Pointer to GdbParser class
	*/
	QPointer<GdbParser> mParser;
	/*!
		\details Pointer to GdbProcess class
	*/
	QPointer<GdbProcess> mProcess;

	/*! 
		\details Current Class name
	*/
	QString mClassName;

	/*!
		\details List of all commands
	*/
	QList<SequencerCmd> mCmdList;
};
#endif
