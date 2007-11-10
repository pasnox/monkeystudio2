/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : pTabBar
** FileName  : pTabBar.h
** Date      : 2007-11-09T23:58:01
** License   : GPL
** Comment   : Your comment here
** Home Page : 
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PTABBAR_H
#define PTABBAR_H

#include <QTabBar>

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
	virtual QSize tabSizeHint( int ) const;
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

};

#endif // PTABBAR_H
