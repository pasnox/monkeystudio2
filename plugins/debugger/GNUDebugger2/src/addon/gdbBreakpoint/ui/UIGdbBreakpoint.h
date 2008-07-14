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
	\file UIGdbBreakpoint.h 
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Ui for show breakpoints in QTabWiget. It used by GdbBreakpoint class. 

*/
#ifndef UIGDBBREAKPOINT_H
#define UIGDBBREAKPOINT_H
//
#include "ui_UIGdbBreakpoint.h"
//
#include <QPointer>
//
#include "../gdbBreakpointStruct.h"

#include "UIBreakpointDelegate.h"

/*!
	\brief Ui for show breakpoints in QTabWiget. It used by GdbBreakpoint class. 
	\details Frontend for remote breakpoints
*/
class UIGdbBreakpoint : public QWidget, public Ui::UIGdbBreakpoint
{
	Q_OBJECT
	//

public slots:

	void onEnableChanged(int );
	void onConditionChanged( );
	void onItemChanged(const QModelIndex & , const QModelIndex & );

public:
	static UIGdbBreakpoint* self( QWidget* = 0 );
	//
	void upDateData(const QList<Breakpoint *> & );
	UIGdbBreakpoint( QWidget* = 0 );

protected:
	void closeEvent( QCloseEvent* );
	//
private:
	static QPointer<UIGdbBreakpoint> _self;
	
	QStandardItemModel *model;
	UIBreakpointDelegate *delegate;

signals:

	/*!
		\details Emited when the user want enable or disable one breakpoint.
	*/
	void enabledBreakpoint(const QString  &, const int &, const bool &);

	/*!
		\details Emited when the user want continned or not one breakpoint.
	*/
	void conditionnedBreakpoint(const QString  &, const int &, const QString &);

};
//
#endif // UIGDBBREAKPOINT_H
