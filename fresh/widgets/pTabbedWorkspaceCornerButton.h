/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pTabbedWorkspaceCornerButton.h
** Date      : 2008-01-14T00:27:54
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



#ifndef PTABBEDWORKSPACECORNERBUTTON_H
#define PTABBEDWORKSPACECORNERBUTTON_H

#include "MonkeyExport.h"

#include <QToolButton>
#include <QBoxLayout>

class QStyleOptionToolButton;

class Q_MONKEY_EXPORT pTabbedWorkspaceCornerButton : public QToolButton
{
	Q_OBJECT

public:
	enum CursorArea { caNone = 0, caArrow, caButton, caArrowClicked, caButtonClicked };

	pTabbedWorkspaceCornerButton( QWidget* p, QBoxLayout::Direction = QBoxLayout::LeftToRight );

	virtual QSize sizeHint() const;

	QBoxLayout::Direction direction() const;

public slots:
	void setDirection( QBoxLayout::Direction );

protected:
	virtual void paintEvent( QPaintEvent* );
	virtual void mousePressEvent( QMouseEvent* );
	virtual void mouseReleaseEvent( QMouseEvent* );

	pTabbedWorkspaceCornerButton::CursorArea cursorArea( const QPoint& = QPoint() ) const;

	QMenu* hasMenu() const;
	bool menuButtonDown() const;
	void setStyleOption( QStyleOptionToolButton* ) const;

	QBoxLayout::Direction mDirection;
	bool mMenuDown;

};

#endif // PTABBEDWORKSPACECORNERBUTTON_H
