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
	\file gdbWatchStruct.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class implements all functions for unformated structure from Gdb. Used by GdbWatch class 
*/


#include "gdbWatchStruct.h"
#include <QMessageBox>

/*!
	\details Extract member from sturct
	\param val is the string of struct.
	\code
	example : "rtf = {art = {0}, a = 5 , b = 6 , rtf  = {t = 12 , r = 34}}"
	return  :
	1 : art = {0}
	2 : a = 5
	3 : b = 6
	4 : rtf = {t = 12 , r = 34}
	\endcode
	\retval List of all members.
*/
QStringList WatchStruct::extractMember(QString val)
{
int count =-1;
QString member;
QStringList memberList;

	for(int i = val.indexOf("{")+1; i<val.length(); i++)
	{
		if(val.at(i) == '{')
		{
			do
			{
				if(val.at(i) == '}')
					count--;
				if(val.at(i) == '{')
					count==-1 ? count=1 : count++;
		
				 member.append(val.at(i));
				
				i++;
			}
			while(count !=0);
		
			memberList << member;
			member.clear();
		}
		else
		{
			if(val.at(i) == ',' || val.at(i) == '}')
			{
				if(!member.isEmpty())
				{
					memberList << member;
				 }
				member.clear();
			}
			else
				member.append(val.at(i));
		}
	}
	return memberList;
}


/*!
	\details Return type of member
	\param val is the member name
	\retval  2 if member is a other struct
	\retval  1 if member is egality "x = 2"
	\retval  0 if member is orphelin "0x00"
*/
int WatchStruct::memberType(QString val)
{
QString name;
QStringList arg;

	arg = val.split("{");
	if(arg.at(0).isEmpty()) return 0;
	
	if(val.contains(" = {"))
		 return 2;

	if(val.contains("="))
		return 1;
	return 0; // member orphelin
}


/**
	\details Return the member correctly formated.
	\param val is the string
	\retval Member correctly formated.
*/
QStringList WatchStruct::formatMember(QString val)
{
	if(val.contains("="))
	{
		QStringList list = val.split(" = ");
		return QStringList() <<list.at(0) <<list.at(1);
	}	
	if(val.contains(" = {"))
	{
		return QStringList()<< val <<"";
	}	
	return QStringList()<< val << val;
}


// recursive function
// fixed 1.3.2 07/07/08
/*!
 * \details Unformated structure from Gdb and create child of QTreeWidget.
 *
 * Recursive function.
 * For example Gdb send myStruct = { a = 3, b = { x = 4, y = 6}}.
 * After formated you see correctly this struture in QTreeWidget
 *
 * \code 
 * Var Name			Value
 * myStruct
 *	|_ a			3
 *	|_ b			
 *		|_ x		4
 *		|_ y		6
 * \endcode
 * \param parentItem is the parent item
 * \param val is the value has decompil
*/
void WatchStruct::decompilStrut(QTreeWidgetItem *parentItem, QString val)
{
QString memberBlock;
int indexTab=0;
int index = 0;

	QStringList memberList = extractMember(val);
	for(int i=0; i<memberList.count();i++)
	{
		memberBlock = memberList.at(i);
		while(memberBlock.startsWith(" ")) memberBlock.remove(0,1);

		QTreeWidgetItem *child;	
		child = parentItem->child(index);
		if(child == NULL) 
			child = new QTreeWidgetItem(parentItem);
		switch(memberType(memberBlock))
		{
			case 0: // orphelin
				child->setText(0, "[0x" + QString::number(indexTab++,16) +"]");
//				child->setText(4, "$ = " + memberBlock);
				child->setText(4, memberBlock);
				decompilStrut(child, memberBlock);
			break;

			case 1 :
				child->setText(0, formatMember(memberBlock).at(0));
//				child->setText(4, "$ = " + formatMember(memberBlock).at(1));
				child->setText(4, formatMember(memberBlock).at(1));
			break;
			case 2:
				child->setText(0, formatMember(memberBlock).at(0));
//				child->setText(4, "$ = " + memberList.at(i).right(memberList.at(i).length() - memberList.at(i).indexOf(" = {") - 3));
				decompilStrut(child, memberBlock);
		} // end switch
		index++;
	}// end for
}

