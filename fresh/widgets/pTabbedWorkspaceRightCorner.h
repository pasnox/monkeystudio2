/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pTabbedWorkspaceRightCorner.h
** Date      : 2008-01-14T00:27:55
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
#ifndef PTABBEDWORKSPACERIGHTCORNER_H
#define PTABBEDWORKSPACERIGHTCORNER_H

#include "MonkeyExport.h"
#include "pTabbedWorkspaceCorner.h"
#include "QSingleton.h"
#include "pTabbedWorkspace.h"

#include <QTabBar>

class QAction;
class QActionGroup;

class Q_MONKEY_EXPORT pTabbedWorkspaceRightCorner : public pTabbedWorkspaceCorner, public QSingleton<pTabbedWorkspaceRightCorner>
{
	Q_OBJECT
	friend class QSingleton<pTabbedWorkspaceRightCorner>;

protected:
	pTabbedWorkspaceRightCorner( pTabbedWorkspace* );

	QActionGroup* agTabShape;
	QActionGroup* agTabMode;
	QActionGroup* agDocumentMode;

protected slots:
	void agTabShape_triggered( QAction* );
	void agTabMode_triggered( QAction* );
	void agDocumentMode_triggered( QAction* );

	void tabShapeChanged( QTabBar::Shape );
	void tabModeChanged( pTabbedWorkspace::TabMode );
	void documentModeChanged( pTabbedWorkspace::DocumentMode );

signals:
	void requestChangeTabMode( pTabbedWorkspace::TabMode );
	void requestChangeDocumentMode( pTabbedWorkspace::DocumentMode );
	void requestChangeTabShape( QTabBar::Shape );

};

#endif // PTABBEDWORKSPACERIGHTCORNER_H
