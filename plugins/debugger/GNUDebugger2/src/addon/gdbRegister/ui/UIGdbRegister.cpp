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
	\file UIGdbRegister.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Ui for show registers values in QTabWiget. It used by GdbRegister class.
*/

#include "UIGdbRegister.h"
//
#include <QCloseEvent>
//
QPointer<UIGdbRegister> UIGdbRegister::_self = 0L;
//
UIGdbRegister* UIGdbRegister::self( QWidget* parent )
{
	if ( !_self )
		_self = new UIGdbRegister( parent );
	return _self;
}
//
UIGdbRegister::UIGdbRegister( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
}
//
void UIGdbRegister::closeEvent( QCloseEvent* e )
{
	e->accept();
}
