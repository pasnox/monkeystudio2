/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
#ifndef SIZEHANDLERECT_H
#define SIZEHANDLERECT_H

#include "namespace_global.h"

#include "widgethostconstants.h"

#include <QtGui/QWidget>
#include <QtCore/QPoint>

namespace SharedTools {
namespace Internal {

class SizeHandleRect : public QWidget
{
    Q_OBJECT
public:
    enum Direction { LeftTop, Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left };

    SizeHandleRect(QWidget *parent, Direction d, QWidget *resizable);

    Direction dir() const  { return m_dir; }
    void updateCursor();
    void setState(SelectionHandleState st);

signals:

    void mouseButtonReleased(const QRect &, const QRect &);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    void tryResize(const QSize &delta);

private:
    const Direction m_dir;
    QPoint m_startPos;
    QPoint m_curPos;
    QSize m_startSize;
    QSize m_curSize;
    QWidget *m_resizable;
    SelectionHandleState m_state;
};

}
} // namespace SharedTools


#endif // SIZEHANDLERECT_H
