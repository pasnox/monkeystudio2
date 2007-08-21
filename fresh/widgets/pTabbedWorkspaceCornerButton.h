/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pTabbedWorkspaceCornerButton.h
 * LICENSE      : GPL
 * COMMENTARY   : This class provide a QToolButton that can be rotated
 ********************************************************************************************************/
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
