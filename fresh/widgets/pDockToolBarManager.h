/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pDockToolBarManager.h
** Date      : 2008-01-14T00:27:41
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
#ifndef PDOCKTOOLBARMANAGER_H
#define PDOCKTOOLBARMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QHash>
#include <QBoxLayout>

class QMainWindow;
class pDockToolBar;
class QDockWidget;
class pSettings;

class Q_MONKEY_EXPORT pDockToolBarManager : public QObject, public QSingleton<pDockToolBarManager>
{
	Q_OBJECT
	friend class QSingleton<pDockToolBarManager>;

public:
	QMainWindow* mainWindow() const;
	pDockToolBar* bar( Qt::ToolBarArea );

	void setSettings( pSettings* );
	pSettings* settings();

	static Qt::ToolBarArea dockWidgetAreaToToolBarArea( Qt::DockWidgetArea );
	static Qt::DockWidgetArea toolBarAreaToDockWidgetArea( Qt::ToolBarArea );
	static QBoxLayout::Direction toolBarAreaToBoxLayoutDirection( Qt::ToolBarArea );

protected:
	QMainWindow* mMain;
	QHash<Qt::ToolBarArea, pDockToolBar*> mBars;
	pSettings* mSettings;

	pDockToolBarManager( QMainWindow* );

public slots:
	void dockWidgetAreaChanged( QDockWidget*, pDockToolBar* );
	virtual void restoreState( pDockToolBar* = 0 );
	virtual void saveState( pDockToolBar* = 0 );

};

#endif // PDOCKTOOLBARMANAGER_H
