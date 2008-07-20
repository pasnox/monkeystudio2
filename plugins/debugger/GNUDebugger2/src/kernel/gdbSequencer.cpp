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
	\file gdbSequencer.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Execute commands step by step
*/

#include "gdbSequencer.h"

/*!
	\details Create new object
	\param parent of this object
*/
GdbSequencer::GdbSequencer(QObject * parent) : QObject(parent)
{
}

//
/*!
	\details None.
*/
GdbSequencer::~GdbSequencer()
{
}

//
/*!
	\details  Add new command to the end of command list
	\param cmd is the command 
	\sa remove()
*/
void GdbSequencer::add(const SequencerCmd & cmd)
{
	mCmdList << cmd;
}

//
/*!
	\details Add new command list
	\note The last command list is cleared.
	\param className is the cleaa name
	\param cmdList is the command list
	\sa remove()
*/
void GdbSequencer::add(const QString & className, const QList<SequencerCmd> & cmdList)
{
	mCmdList.clear();
	mCmdList = cmdList;
	mClassName = className;
}

//
/*!
	\details Send the first command to gdb. 
	Use loop() function for send the next command to gdb.
	\sa start(const int &)

*/
void GdbSequencer::start()
{
	start(0);
}

//
/*!
	\details Same as start() function , but start sequence at index
	\param i is the index of the first command that you start
	\sa start()
*/
void GdbSequencer::start(const int & i)
{
	mCurrentIndex = i;

	if(mCurrentIndex < mCmdList.count())
	{
		GdbParser::instance()->setNextCommand(mClassName, mCmdList.at(i).cmd );
		GdbProcess::instance()->sendRawData( mCmdList.at(i).cmd );
	}
}

//
/*!
	\details remove all commands in list
	\sa remove(const SequencerCmd &)
*/
void GdbSequencer::remove()
{
	mCmdList.clear();
}

//
/*!
	\details remove just one command in list
	\param cmd is the command that you want removed
	\sa remove()
*/
void GdbSequencer::remove(const SequencerCmd & cmd)
{
	for(int i = 0 ; i <mCmdList.count(); i++)
		if(mCmdList.at(i).name == cmd.name)
			mCmdList.removeAt(i);
}

//
/*!
	\details execute next command
	\sa skipLoop()
*/
void GdbSequencer::loop()
{
	mCurrentIndex++;
	start(mCurrentIndex);
}

//
/*!
	\details skip current command
	\sa loop()
*/
void GdbSequencer::skipLoop()
{
	mCurrentIndex++;
}

//
/*!
	\details Change command by an other
	\param before is the command to be change
	\param after is the new command 
	\sa change(const QString & , const QString & )
*/
void GdbSequencer::change(const SequencerCmd & before, const SequencerCmd & after )
{
	for(int i = 0 ; i <mCmdList.count(); i++)
		if(mCmdList.at(i).name == before.name)
			mCmdList.replace(i, after);
}

//
/*!
	\details Same as change(const SequencerCmd &, const SequencerCmd &)
	\param cmdName is the name of the command that you want change
	\param newCmd is the new command string
	\sa change(const SequencerCmd & , const SequencerCmd &  )
*/
void GdbSequencer::change(const QString & cmdName, const QString & newCmd)
{
	for(int i = 0 ; i <mCmdList.count(); i++)
	{
		if(mCmdList.at(i).name == cmdName)
		{
			SequencerCmd s(cmdName, newCmd);
			mCmdList.replace(i, s);
		}
	}
}

//
/*!
	\details Return the current command
	\retval Current command string or empty string if not have.
*/
QString GdbSequencer::currentCmd()
{
	if(mCurrentIndex < mCmdList.count())
		return mCmdList.at(mCurrentIndex).name;
	else return QString();
}
