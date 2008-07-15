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
	\file gdbRestoreLine.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief  Restore line if it is splited in more lines
*/
#ifndef GDBRESTORELINE_H
#define GDBRESTORELINE_H



#include <QObject>
#include <QRegExp>
#include <QStringList>

/*!
	\brief  Restore line if it is splited in more lines
	\details This class can restore line if it is splited in more lines
	\code
	Breakpoint 2, 0x3ed4a5 at
		main.cpp,line 12
	(gdb)
	\endcode

	For restore this line just add a new configuration by

	\code
	GdbRestoreLine::add(yourClassName, QRegExp("^Breakpoint\\s+\\d+,\\s+.*"), QRegExp("*.,\\s+line\\s+\\d+"));
	\endcode


	Now if this line is found, GdbRestoreLine can restore it and you have now

	\code
	Breakpoint 2, 0x3ed4a5 at main.cpp,line 12
	(gdb)
	\endcode
*/

class GdbRestoreLine : public QObject
{

	/*!
		\brief Struct for store data
	*/
	typedef struct GdbLines
	{
		/*!
			\details class name
		*/
		QString className;
		/*! 
			\details Your line must starts with ...
		*/
		QRegExp l1;
		/*! 
			\details Your line must ends with
		*/
		QRegExp l2;
	};


public:

	GdbRestoreLine(QObject *parent = 0);
	~GdbRestoreLine();
	
	void add(const QString & ,const QRegExp &, const QRegExp &);
	void add(const QString &, const QString &, const QString &);
	bool tryRestore(const QString &, QStringList * );

	int begin(/*const QString &,*/ const int &, const QStringList & , const QRegExp &);
	int end(/*const QString &, */const int & , const QStringList &, const QRegExp &);

private:

	/*! 
		\details List of all restore line
	*/
	QList<GdbLines> GdbRestoreLineList;

};

#endif
