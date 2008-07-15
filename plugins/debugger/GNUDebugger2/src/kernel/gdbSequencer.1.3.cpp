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
	\file gdbSequencer.1.3.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Execute commands step by step
*/

#include "gdbSequencer.1.3.h"

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
	\param c is the command 
*/
void GdbSequencer::add(const SequencerCmd & c)
{
	mCmdList << c;
}

//
/*!
	\details Add new command list
	\note The last command list is cleared.
	\param className is the cleaa name
	\param s is the command list
*/
void GdbSequencer::add(const QString & className, const QList<SequencerCmd> & s)
{
	mCmdList.clear();
	mCmdList = s;
	mClassName = className;
}

//
/*!
	\details Send the first command to gdb. 
	Use loop() function for send the next command to gdb.
*/
void GdbSequencer::start()
{
	start(0);
}

//
/*!
	\details Same as start() function , but start sequence at index
	\param i is the index of the first command that you start
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
*/
void GdbSequencer::remove()
{
	mCmdList.clear();
}

//
/*!
	\details remove just one commands in list
	\param a is the command that you want removed
*/
void GdbSequencer::remove(const SequencerCmd & a)
{
	for(int i = 0 ; i <mCmdList.count(); i++)
		if(mCmdList.at(i).name == a.name)
			mCmdList.removeAt(i);
}

//
/*!
	\details execute next command
*/
void GdbSequencer::loop()
{
	mCurrentIndex++;
	start(mCurrentIndex);
}

//
/*!
	\details skip current command
*/
void GdbSequencer::skipLoop()
{
	mCurrentIndex++;
}

//
/*!
	\details Change command by an other
	\param a is the command to be change
	\param b is the nex command 
*/
void GdbSequencer::change(const SequencerCmd & a, const SequencerCmd & b )
{
	for(int i = 0 ; i <mCmdList.count(); i++)
		if(mCmdList.at(i).name == a.name)
			mCmdList.replace(i, b);
}

//
/*!
	\details Same as change(const SequencerCmd &, const SequencerCmd &)
	\param name is the name of the command that you want change
	\param v is the new command string
*/
void GdbSequencer::change(const QString & name, const QString & v)
{
	for(int i = 0 ; i <mCmdList.count(); i++)
	{
		if(mCmdList.at(i).name == name)
		{
			SequencerCmd s(name, v);
			mCmdList.replace(i, s);
		}
	}
}

//
/*!
	\details Return the current command
	\retval Command string
*/
QString GdbSequencer::currentCmd()
{
	if(mCurrentIndex < mCmdList.count())
		return mCmdList.at(mCurrentIndex).name;
	else return QString();
}
