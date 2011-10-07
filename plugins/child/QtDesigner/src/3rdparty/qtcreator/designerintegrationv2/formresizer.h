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
#ifndef FORMRESIZER_H
#define FORMRESIZER_H

#include "namespace_global.h"

#include "widgethostconstants.h"

#include <QtGui/QWidget>
#include <QtCore/QVector>

QT_FORWARD_DECLARE_CLASS(QDesignerFormWindowInterface)
QT_FORWARD_DECLARE_CLASS(QFrame)

namespace SharedTools {
namespace Internal {

class SizeHandleRect;

/* A window to embed a form window interface as follows:
 *
 *            Widget
 *              |
 *          +---+----+
 *          |        |
 *          |        |
 *       Handles   QVBoxLayout [margin: SELECTION_MARGIN]
 *                   | 
 *                 Frame [margin: lineWidth]
 *                   |
 *                 QVBoxLayout
 *                   |
 *                 QDesignerFormWindowInterface
 *
 * Can be embedded into a QScrollArea. */

class FormResizer : public QWidget
{
    Q_OBJECT
public:

    FormResizer(QWidget *parent = 0);

    void updateGeometry();
    void setState(SelectionHandleState st);
    void update();

    void setFormWindow(QDesignerFormWindowInterface *fw);

signals:
    void formWindowSizeChanged(const QRect &oldGeo, const QRect &newGeo);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void mainContainerChanged();

private:
    QSize decorationSize() const;
    QWidget *mainContainer();

    QFrame *m_frame;
    typedef QVector<SizeHandleRect*> Handles;
    Handles m_handles;
    QDesignerFormWindowInterface * m_formWindow;
};

}
} // namespace SharedTools

#endif // FORMRESIZER_H
