/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pTabBar.h
** Date      : 2008-01-14T00:27:50
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



#ifndef PTABBAR_H
#define PTABBAR_H

#include <QTabBar>
#include <QList>
#include <QUrl>

class QAction;
class pAction;

class pTabBar : public QTabBar
{
	Q_OBJECT

public:
	pTabBar( QWidget* = 0 );

	QColor tabsColor() const;
	QColor currentTabColor() const;
	bool tabsHaveCloseButton() const;
	bool tabsHaveShortcut() const;
	bool tabsElided() const;
	QAction* toggleTabsHaveCloseButtonAction() const;
	QAction* toggleTabsHaveShortcutAction() const;
	QAction* toggleTabsElidedAction() const;

public slots:
	void resetTabsColor();
	void setTabsColor( const QColor& );
	void setCurrentTabColor( const QColor& );
	void setTabsHaveCloseButton( bool );
	void setTabsHaveShortcut( bool );
	void setTabsElided( bool );

protected:
	virtual void paintEvent( QPaintEvent* );
	virtual void mousePressEvent( QMouseEvent* );
	virtual void mouseReleaseEvent( QMouseEvent* );
	virtual void mouseMoveEvent(QMouseEvent* );
	virtual void dragEnterEvent( QDragEnterEvent* );
	virtual void dropEvent( QDropEvent* );
	virtual void tabInserted( int );
	virtual void tabRemoved( int );
	virtual QRect iconRectForTab( int );
	virtual bool inCloseButtonRect( int, const QPoint& );
	void updateTabsNumber( int = -1 );

	QPoint dragStartPosition;
	QColor mTabsColor;
	QColor mCurrentTabColor;

	pAction* aToggleTabsHaveCloseButton;
	pAction* aToggleTabsHaveShortcut;
	pAction* aToggleTabsElided;

signals:
	void leftButtonPressed( int, const QPoint& );
	void midButtonPressed( int, const QPoint& );
	void rightButtonPressed( int, const QPoint& );
	void tabDropped( int, int );
	void tabsColorChanged( const QColor& );
	void currentTabColorChanged( const QColor& );
	void closeButtonClicked( int );
	void tabsHaveCloseButtonChanged( bool );
	void tabsHaveShortcutChanged( bool );
	void tabsElidedChanged( bool );
	void urlsDropped( const QList<QUrl>& );

};

#endif // PTABBAR_H
