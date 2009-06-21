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
	\file UIGdbBacktrace.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Ui for show backtrace in QTabWiget. It used by GdbBacktrace class. 
*/

#include "UIGdbBacktrace.h"
//
#include <QCloseEvent>
//
QPointer<UIGdbBacktrace> UIGdbBacktrace::_self = 0L;
//
/*!
	\details Create new object 
	\param parent of this object
	\return Pointer to this class
*/
UIGdbBacktrace* UIGdbBacktrace::self( QWidget* parent )
{
	if ( !_self )
		_self = new UIGdbBacktrace( parent );
	return _self;
}
//
/*!
	\details Setup Ui
*/
UIGdbBacktrace::UIGdbBacktrace( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
}
//
/*!
	\details Close event of this object 
*/
void UIGdbBacktrace::closeEvent( QCloseEvent* e )
{
	e->accept();
}
