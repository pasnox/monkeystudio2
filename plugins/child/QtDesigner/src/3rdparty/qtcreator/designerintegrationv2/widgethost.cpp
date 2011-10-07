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
#include "widgethost.h"
#include "formresizer.h"
#include "widgethostconstants.h"

#include <QtDesigner/QDesignerFormWindowInterface>
#include <QtDesigner/QDesignerFormWindowCursorInterface>

#include <QtGui/QPalette>
#include <QtGui/QLayout>
#include <QtGui/QFrame>
#include <QtGui/QResizeEvent>
#include <QtCore/QDebug>

using namespace SharedTools;

// ---------- WidgetHost
WidgetHost::WidgetHost(QWidget *parent, QDesignerFormWindowInterface *formWindow) :
    QScrollArea(parent),
    m_formWindow(0),
    m_formResizer(new Internal::FormResizer)
{
    setWidget(m_formResizer);
    // Re-set flag (gets cleared by QScrollArea): Make the resize grip of a mainwindow form find the resizer as resizable window.
    m_formResizer->setWindowFlags(m_formResizer->windowFlags() | Qt::SubWindow);
    setFormWindow(formWindow);
}

WidgetHost::~WidgetHost()
{
    if (m_formWindow)
        delete m_formWindow;
}

void WidgetHost::setFormWindow(QDesignerFormWindowInterface *fw)
{
    m_formWindow = fw;
    if (!fw)
        return;

    m_formResizer->setFormWindow(fw);

    setBackgroundRole(QPalette::Base);
    m_formWindow->setAutoFillBackground(true);
    m_formWindow->setBackgroundRole(QPalette::Background);

    connect(m_formResizer, SIGNAL(formWindowSizeChanged(QRect, QRect)),
            this, SLOT(fwSizeWasChanged(QRect, QRect)));
}

QSize WidgetHost::formWindowSize() const
{
    if (!m_formWindow || !m_formWindow->mainContainer())
        return QSize();
    return m_formWindow->mainContainer()->size();
}

void WidgetHost::fwSizeWasChanged(const QRect &, const QRect &)
{
    // newGeo is the mouse coordinates, thus moving the Right will actually emit wrong height
    emit formWindowSizeChanged(formWindowSize().width(), formWindowSize().height());
}

void WidgetHost::updateFormWindowSelectionHandles(bool active)
{
    Internal::SelectionHandleState state = Internal::SelectionHandleOff;
    const QDesignerFormWindowCursorInterface *cursor = m_formWindow->cursor();
    if (cursor->isWidgetSelected(m_formWindow->mainContainer()))
        state = active ? Internal::SelectionHandleActive :  Internal::SelectionHandleInactive;

    m_formResizer->setState(state);
}

QWidget *WidgetHost::integrationContainer() const
{
    return m_formResizer;
}
