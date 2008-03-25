/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pActionManager.h
** Date      : 2008-01-14T00:27:39
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
#ifndef PACTIONMANAGER_H
#define PACTIONMANAGER_H

#include "../objects/MonkeyExport.h"
#include "../objects/QSingleton.h"

#include <QHash>
#include <QKeySequence>

class QAction;
class pKeySequenceInput;
class QTreeWidget;
class QPushButton;
class pSettings;

typedef QList<QAction*> pActionList;
typedef QHash<QString, pActionList> pHashActionList;

class Q_MONKEY_EXPORT pActionManager : public QObject, public QSingleton<pActionManager>
{
	Q_OBJECT
	friend class QSingleton<pActionManager>;
	friend class pShortcutsEditor;

public:
	static void setSettings( pSettings* );
	static pSettings* settings();
	static void reloadSettings();

	static QAction* addAction( const QString&, QAction* );
	static void removeAction( const QString&, QAction* );
	static pHashActionList actions();

	static QString globalGroup();

	static QKeySequence getShortcut( const QString&, QAction*, const QKeySequence& );
	static bool setShortcut( QAction*, const QKeySequence& );

	static QString lastError();

private:
	pActionManager( QObject* = QApplication::instance() );

	pHashActionList mActions;
	QString mError;
	pSettings* mSettings;

public slots:
	void showSettings();

};

#endif // PACTIONMANAGER_H
