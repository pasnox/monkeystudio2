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
	\file gdbWatchStruct.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class implements all functions for unformated structure from Gdb. Used by GdbWatch class 
*/

#ifndef GDBWATCHSTRUCT_H
#define GDBWATCHSTRUCT_H

#include <QString>
#include <QStringList>
#include <QTreeWidgetItem>

/*!
	\brief This class implements all functions for unformated structure from Gdb. Used by GdbWatch class 
*/

class WatchStruct
{

private:

	QStringList extractMember(QString val);
	int memberType(QString val);
	QStringList formatMember(QString val);

public:

	void decompilStrut(QTreeWidgetItem *parentItem, QString val);
};

#endif
