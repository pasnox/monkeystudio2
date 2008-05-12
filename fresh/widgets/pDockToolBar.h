/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pDockToolBar.h
** Date      : 2008-01-14T00:27:40
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
#ifndef PDOCKTOOLBAR_H
#define PDOCKTOOLBAR_H

#include "../objects/MonkeyExport.h"

#include <QToolBar>
#include <QIcon>
#include <QHash>

class QDockWidget;
class QAbstractButton;
class QFrame;
class QBoxLayout;
class pDockToolBarManager;

class Q_MONKEY_EXPORT pDockToolBar : public QToolBar
{
	Q_OBJECT

public:
	pDockToolBar( pDockToolBarManager*, Qt::Orientation = Qt::Horizontal, QMainWindow* = 0 );

	virtual bool eventFilter( QObject*, QEvent* );

	void addAction( QAction* = 0, bool = true );
	void addActions( QList<QAction*>, bool = true );

	int addDock( QDockWidget*, const QString& = QString(), const QIcon& = QIcon() );

	bool isDockVisible( int ) const;
	bool isDockVisible( QDockWidget* ) const;

	bool exclusive() const;
	bool textAlwaysVisible() const;

	int id( QDockWidget* ) const;
	int id( QAbstractButton* ) const;

	QDockWidget* dock( int ) const;
	QDockWidget* dock( QAbstractButton* ) const;

	QAbstractButton* button( int ) const;
	QAbstractButton* button( QDockWidget* ) const;
	
	QList<QDockWidget*> docks() const;
	QList<QAbstractButton*> buttons() const;

	int count() const;
	
	QAction* toggleExclusiveAction() const;

private:
	pDockToolBarManager* mManager;
	int mUniqueId;
	bool mTextAlwaysVisible;
	QFrame* mFrame;
	QBoxLayout* mLayout;
	QAction* aDockFrame;
	QAction* aToggleExclusive;
	QHash<int, QAbstractButton*> mButtons;
	QHash<int, QDockWidget*> mDocks;

public slots:
	void removeDock( int );
	void removeDock( QDockWidget* );
	void setDockVisible( QDockWidget*, bool );
	void setExclusive( bool );
	void setTextAlwaysVisible( bool visible );

private slots:
	void internal_checkVisibility();
	void internal_checkButtonText( QAbstractButton* );
	void internal_orientationChanged( Qt::Orientation );
	void internal_dockChanged();
	void internal_dockDestroyed( QObject* );
	void internal_buttonClicked( bool );

signals:
	void buttonClicked( int );
	void dockWidgetAreaChanged( QDockWidget*, pDockToolBar* );

};

#endif // PDOCKTOOLBAR_H
