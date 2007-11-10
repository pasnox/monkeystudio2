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

class pTabBar : public QTabBar
{
	Q_OBJECT

public:
	pTabBar( QWidget* = 0 );

protected:
	void mousePressEvent( QMouseEvent* e );
	void mouseMoveEvent(QMouseEvent * e );
	void dragEnterEvent( QDragEnterEvent* e );
	void dropEvent( QDropEvent* e );

	QPoint dragStartPosition;

protected slots:
	void clearTabsColor();

signals:
	void leftButtonPressed( int, const QPoint& );
	void midButtonPressed( int, const QPoint& );
	void rightButtonPressed( int, const QPoint& );
	void tabDropped( int, int );

};

#endif // PTABBAR_H
