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
#include "formresizer.h"
#include "sizehandlerect.h"
#include "widgethostconstants.h"

#include <QtCore/QDebug>

#include <QtDesigner/QDesignerFormWindowInterface>

#include <QtGui/QResizeEvent>
#include <QtGui/QPalette>
#include <QtGui/QLayout>
#include <QtGui/QFrame>
#include <QtGui/QResizeEvent>

enum { debugFormResizer = 0 };

using namespace SharedTools::Internal;

FormResizer::FormResizer(QWidget *parent) :
    QWidget(parent),
    m_frame(new QFrame),
    m_formWindow(0)
{
    // Make the resize grip of a mainwindow form find us as resizable window.
    setWindowFlags(windowFlags() | Qt::SubWindow);
    setBackgroundRole(QPalette::Base);

    QVBoxLayout *handleLayout = new QVBoxLayout(this);
    handleLayout->setMargin(SELECTION_MARGIN);
    handleLayout->addWidget(m_frame);

    m_frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    QVBoxLayout *layout = new QVBoxLayout(m_frame);
    layout->setMargin(0);
    // handles
    m_handles.reserve(SizeHandleRect::Left);
    for (int i = SizeHandleRect::LeftTop; i <= SizeHandleRect::Left; ++i) {
        SizeHandleRect *shr = new SizeHandleRect(this, static_cast<SizeHandleRect::Direction>(i), this);
        connect(shr, SIGNAL(mouseButtonReleased(QRect,QRect)), this, SIGNAL(formWindowSizeChanged(QRect,QRect)));
        m_handles.push_back(shr);
    }
    setState(SelectionHandleActive);
    updateGeometry();
}

void FormResizer::updateGeometry()
{
    const QRect &geom = m_frame->geometry();

    if (debugFormResizer)
        qDebug() << "FormResizer::updateGeometry() " << size() << " frame " << geom;

    const int w = SELECTION_HANDLE_SIZE;
    const int h = SELECTION_HANDLE_SIZE;

    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it) {
        SizeHandleRect *hndl = *it;;
        switch (hndl->dir()) {
        case SizeHandleRect::LeftTop:
            hndl->move(geom.x() - w / 2, geom.y() - h / 2);
            break;
        case SizeHandleRect::Top:
            hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() - h / 2);
            break;
        case SizeHandleRect::RightTop:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() - h / 2);
            break;
        case SizeHandleRect::Right:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() + geom.height() / 2 - h / 2);
            break;
        case SizeHandleRect::RightBottom:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case SizeHandleRect::Bottom:
            hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case SizeHandleRect::LeftBottom:
            hndl->move(geom.x() - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case SizeHandleRect::Left:
            hndl->move(geom.x() - w / 2, geom.y() + geom.height() / 2 - h / 2);
            break;
        default:
            break;
        }
    }
}

void FormResizer::update()
{
    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it) {
        (*it)->update();
    }
}

void FormResizer::setState(SelectionHandleState st)
{
    if (debugFormResizer)
        qDebug() << "FormResizer::setState " << st;

    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
        (*it)->setState(st);
}

void FormResizer::setFormWindow(QDesignerFormWindowInterface *fw)
{
    if (debugFormResizer)
        qDebug() << "FormResizer::setFormWindow " << fw;
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(m_frame->layout());
    Q_ASSERT(layout);
    if (layout->count())
        delete layout->takeAt(0);
    m_formWindow = fw;

    if (m_formWindow)
        layout->addWidget(m_formWindow);
    mainContainerChanged();
     connect(fw, SIGNAL(mainContainerChanged(QWidget*)), this, SLOT(mainContainerChanged()));
}

void FormResizer::resizeEvent(QResizeEvent *event)
{
    if (debugFormResizer)
        qDebug() << ">FormResizer::resizeEvent" <<  event->size();
    updateGeometry();
    QWidget::resizeEvent(event);
    if (debugFormResizer)
        qDebug() << "<FormResizer::resizeEvent";
}

QSize FormResizer::decorationSize() const
{
    const int lineWidth = m_frame->lineWidth();
    const QMargins frameMargins = m_frame->contentsMargins();
    const int margin = 2* SELECTION_MARGIN;
    QSize size = QSize( margin, margin );
    size += QSize( qMax( frameMargins.left(), lineWidth ), qMax( frameMargins.top(), lineWidth ) );
    size += QSize( qMax( frameMargins.right(), lineWidth ), qMax( frameMargins.bottom(), lineWidth ) );
    return size;
}

QWidget *FormResizer::mainContainer()
{
    if (m_formWindow)
        return m_formWindow->mainContainer();
    return 0;
}

void FormResizer::mainContainerChanged()
{
    const QSize maxWidgetSize = QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    if (const QWidget *mc = mainContainer()) {
        // Set Maximum size which is not handled via a hint (as opposed to minimum size)
        const QSize maxWidgetSize = QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        const QSize formMaxSize = mc->maximumSize();
        QSize newMaxSize = maxWidgetSize;
        if (formMaxSize != maxWidgetSize)
            newMaxSize = formMaxSize + decorationSize();
        if (debugFormResizer)
            qDebug() << "FormResizer::mainContainerChanged" <<  mc << " Size " << mc->size()<< newMaxSize;
        setMaximumSize(newMaxSize);
        resize(decorationSize() + mc->size());
    } else {
        setMaximumSize(maxWidgetSize);
    }
}
