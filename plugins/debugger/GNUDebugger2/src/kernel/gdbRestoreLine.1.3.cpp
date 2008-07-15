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
	\file gdbRestoreLine.1.3.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief  Restore line if it is splited in more lines
*/


#include "gdbRestoreLine.1.3.h"
#include <QMessageBox>

//
/*!
	\details Create new object
	\param parent of this object
*/
GdbRestoreLine::GdbRestoreLine(QObject *parent ) : QObject(parent)
{
	GdbRestoreLineList.clear();
}

//
/*!
	\details Clear list
*/
GdbRestoreLine::~GdbRestoreLine()
{
	GdbRestoreLineList.clear();
}

//
/*!
	\details Add new restoring line
	\param className is the name of class
	\param l1 is the first line
	\param l2 is the last line
*/
void GdbRestoreLine::add(const QString & className ,const QRegExp & l1, const QRegExp & l2) 
{
	GdbLines l={className, l1, l2 };
	GdbRestoreLineList << l;
}

//
/*!
	\details Add new restoring line
	\param className is the name of class
	\param l1 is the first line
	\param l2 is the last line
*/
void GdbRestoreLine::add(const QString & className ,const QString & l1, const QString & l2) 
{
	GdbLines l={ className, QRegExp(l1), QRegExp(l2) };
	GdbRestoreLineList << l;
}

//
/*!
	\details Get the first line in string block (QStringList)
	\param b is index of the first line to start
	\param l is a list of string
	\param r is QRegExp that you find. 
	\retval index of the first line found else return -1 if not found.
*/
int GdbRestoreLine::begin(/*const QString & className ,*/const int & b , const QStringList & l, const QRegExp &r)
{
	for(int i=b; i<l.count() ; i++)
		if(r.exactMatch(l.at(i))) return i;
	return -1;
}

//
/*!
	\details Get the last line
	\param b is index of the first line to start
	\param l is a list of string
	\param r is QRegExp that you find. 
	\retval index of the first line found else return -1 if not found.
*/
int GdbRestoreLine::end(/*const QString & className ,*/const int & b, const QStringList & l, const QRegExp &r)
{
	for(int i=b; i<l.count() ; i++)
		if(r.exactMatch(l.at(i))) return i;
	return -1;
}

//
/*
#0  qMain (argc=1, argv=0x3d4c30)
 at src/main.cpp:65
#1  0x004578d4 in WinMain (instance=0x400000, prevInstance=0x0, cmdShow=10)
    at qtmain_win.cpp:140
#2  0x0045730a in main ()
(gdb) 
*/
/*!
	\details try restore line
	\param className is the name of current class
	\param list is the list of string (QStringList)
	\retval true if the string is restored, else false
*/
bool GdbRestoreLine::tryRestore(const QString & className , QStringList * list)
{
	bool r = false;

	// new version 1.4
	foreach(GdbLines l , GdbRestoreLineList)
	{
		for(int i=0; i< list->count(); i++)
		{
			int lBegin = begin(/*className, */i, *list, l.l1);
			if(lBegin != -1 && l.className == className)
			{
				int lEnd = end(/*className, */lBegin, *list , l.l2);
				if(lEnd != -1)
				{
					r = true;
					// ok found
					QString s;
					for(int j=lBegin; j<=lEnd; j++)
					{
						// create line
						s.append( list->at(lBegin));
						list->removeAt(lBegin);
					}
					list->insert(lBegin, s);
					//				return true;
				}
			}
		}
	}
	return r;	
}
