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
	\file UIGdbWatch.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Ui for show variables values in QTabWiget. It used by GdbWatch class. 
*/

#ifndef UIGDBWATCH_H
#define UIGDBWATCH_H
//
#include "ui_UIGdbWatch.h"
//
#include <QPointer>
//
/*!
	\brief Ui for show variables values in QTabWiget. It used by GdbWatch class. 
*/
class UIGdbWatch : public QWidget, public Ui::UIGdbWatch
{
	Q_OBJECT
	//
public:
	static UIGdbWatch* self( QWidget* = 0 );
	//
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	QStringList getAllvar();

public slots:

	void userDeleteVar ( const QModelIndex &  ) ;

protected:

	void closeEvent( QCloseEvent* );
	//
private:
	UIGdbWatch( QWidget* = 0 );
	static QPointer<UIGdbWatch> _self;
	//
signals:

	void userAddVar(const QString &);

};
//
#endif // UIGDBBACKTRACE_H
