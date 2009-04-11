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
	\file UIManageParsingFile.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Manage parsing file
*/

#ifndef UIMANAGEPARSINGFILE_H
#define UIMANAGEPARSINGFILE_H
//
#include "ui_UIManageParsingFile.h"
//
#include <QPointer>
//
#include "../kernel/gdbPatternFile.h"

#include "UIManageDelegate.h"

/*!
	\brief Manage parsing file
	\details This Ui can modifit the parsing file. It use QTreeWidget and Delegate for this.
	You can find pattern too and change values.
*/
class UIManageParsingFile : public QDialog, public Ui::UIManageParsingFile
{
	Q_OBJECT
	//
public:
	static UIManageParsingFile* self( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	UIManageParsingFile( QWidget* = 0 );
	static QPointer<UIManageParsingFile> _self;
	//
	QList<GdbPattern> * l;

	QStandardItemModel *model;
	UIManageDelegate *delegate;

public slots:
	/*! 
		\details Save
	*/
	void onSave();
	/*! 
		\details Find
	*/
	void onTextChanged(QString s);
};
//
#endif // UIMANAGEPARSINGFILE_H
