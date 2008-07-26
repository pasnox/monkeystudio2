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
	\file UIGdbRegister.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Ui for show registers values in QTabWiget. It used by GdbRegister class.
*/

#ifndef UIGDBREGISTER_H
#define UIGDBREGISTER_H
//
#include "ui_UIGdbRegister.h"
//
#include <QPointer>
//
/*!
	\brief Ui for show registers values in QTabWiget. It used by GdbRegister class.
*/
class UIGdbRegister : public QWidget, public Ui::UIGdbRegister
{
	Q_OBJECT
	//
public:
	static UIGdbRegister* self( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	UIGdbRegister( QWidget* = 0 );
	static QPointer<UIGdbRegister> _self;
	//
};
//
#endif // UIGDBBACKTRACE_H
