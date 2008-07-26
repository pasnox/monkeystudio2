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
	\file pAction.h
	\date 2008-01-14T00:27:39
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief An action that can be customized by user
*/
#ifndef PACTION_H
#define PACTION_H

#include "../objects/MonkeyExport.h"

#include <QAction>

#include "pActionManager.h"

/*!
	\brief An action that can be customized by user
	\details This kind of action take a default key sequence and a group
	\details that allow the action be edited using an action editor dialog.
	\details The action is removed from its group when it's deleted.
*/
class Q_MONKEY_EXPORT pAction : public QAction
{
	Q_OBJECT

public:
	pAction( const QString& name, const QString& text, const QKeySequence& keySequence, const QString& group = pActionManager::globalGroup() );
	pAction( const QString& name, const QIcon& icon, const QString& text, const QKeySequence& keySequence, const QString& group = pActionManager::globalGroup() );
	~pAction();

	static QString fixedString( const QString& string );

	static const char* _DEFAULT_SHORTCUT_PROPERTY_;
	static const char* _GROUP_PROPERTY_;
	static const char* _SETTINGS_SCOPE_;
};

#endif // PACTION_H
