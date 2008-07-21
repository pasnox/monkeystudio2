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
	\file gdbBreakpointStruct.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief It is a base struct for store some informations for breakpoint
*/

#ifndef GDBBREAKPOINTSTRUCT
#define GDBBREAKPOINTSTRUCT

/*!
	\brief 	Base Struct for one breakpoint 
	\details 	GdbBreakpoint class uses two structs to store all informations of the beakpoints.
	
	The first struct store only information for one breakpoint.
	The second struct is Breakpoint.
*/
	/*typedef, fixe for 64 bits*/
	struct BaseBreakpoint
	{
		/*!
			\details Contains the line where the breakpoint is set
		*/
		int line;

		/*!
			\details Contains the index from Gdb, number returned by Gdb
		*/
		int index;

		/*!
			\details If index is true, it indicate that Gdb has breaked in this breakpoint
		*/
		bool hit;

		/*!
			\details Breakpoint is  : 0 = unconditionned, 1 = contidionned
		*/
		int type;

		/*!
			\details Breakpoint is enable if enable is true
		*/
		bool enable;

		/*!
			\details Condition for this breakpoint, for example "i==3". Gdb break only 
			if condition is true
		*/
		QString condition;
	};

	/*!
		\brief It is a struct for store all breakpoints for one file
		\details Struct of all Breakpoints strored for one file
		\note One file can contains more breakpoints.
	*/
	/*typedef, fixe for 64 bits*/
	struct Breakpoint
	{
		/*!
			\details Contains the fileName 
		*/
		QString fileName;
		
		/*!
			\details Contains all breakpoints informations (list)
		*/
		QList<BaseBreakpoint> bp;
	};

#endif
