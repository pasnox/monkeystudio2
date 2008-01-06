// This module implements the "official" low-level API.
//
// Copyright (c) 2008
// 	Phil Thompson <phil@river-bank.demon.co.uk>
// 
// This file is part of QScintilla.
// 
// This copy of QScintilla is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option) any
// later version.
// 
// QScintilla is supplied in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along with
// QScintilla; see the file LICENSE.  If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#include "Qsci/qsciscintillabase.h"

#include <qapplication.h>
#include <qclipboard.h>
#include <qcolor.h>
#include <qscrollbar.h>

#include <qdragobject.h>
#include <qevent.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qptrlist.h>

#include "ScintillaQt.h"


// The #defines in Scintilla.h and the enums in qsciscintillabase.h conflict
// (because we want to use the same names) so we have to undefine those we use
// in this file.
#undef  SCI_SETCARETPERIOD
#undef  SCK_DOWN
#undef  SCK_UP
#undef  SCK_LEFT
#undef  SCK_RIGHT
#undef  SCK_HOME
#undef  SCK_END
#undef  SCK_PRIOR
#undef  SCK_NEXT
#undef  SCK_DELETE
#undef  SCK_INSERT
#undef  SCK_ESCAPE
#undef  SCK_BACK
#undef  SCK_TAB
#undef  SCK_RETURN
#undef  SCK_ADD
#undef  SCK_SUBTRACT
#undef  SCK_DIVIDE
#undef  SCK_WIN
#undef  SCK_RWIN
#undef  SCK_MENU


// Remember if we have linked the lexers.
static bool lexersLinked = false;

// The list of instances.
static QPtrList<QsciScintillaBase> poolList;


// The ctor.
QsciScintillaBase::QsciScintillaBase(QWidget *parent, const char *name,
        WFlags f)
    : QWidget(parent, name, f)
{
    QGridLayout *layout = new QGridLayout(this, 2, 2);

    txtarea = new QWidget(this, 0, WRepaintNoErase|WResizeNoErase);
    txtarea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,
                QSizePolicy::Expanding));
    txtarea->setMouseTracking(true);
    txtarea->setAcceptDrops(true);
    txtarea->setFocusPolicy(WheelFocus);
    txtarea->setFocusProxy(this);
    layout->addWidget(txtarea, 0, 0);

    vsb = new QScrollBar(Vertical, this);
    layout->addWidget(vsb, 0, 1);
    connect(vsb, SIGNAL(valueChanged(int)), SLOT(handleVSb(int)));

    hsb = new QScrollBar(Horizontal, this);
    layout->addWidget(hsb, 1, 0);
    connect(hsb, SIGNAL(valueChanged(int)), SLOT(handleHSb(int)));

    txtarea->installEventFilter(this);

    setFocusPolicy(WheelFocus);

    sci = new ScintillaQt(this);

    SendScintilla(SCI_SETCARETPERIOD, QApplication::cursorFlashTime() / 2);

    // Make sure the lexers are linked in.
    if (!lexersLinked)
    {
        Scintilla_LinkLexers();
        lexersLinked = true;
    }

    QClipboard *cb = QApplication::clipboard();

    if (cb->supportsSelection())
        connect(cb, SIGNAL(selectionChanged()), SLOT(handleSelection()));

    // Add it to the pool.
    poolList.append(this);
}


// The dtor.
QsciScintillaBase::~QsciScintillaBase()
{
    // Remove it from the pool.
    poolList.remove(this);

    delete sci;
}


// Return an instance from the pool.
QsciScintillaBase *QsciScintillaBase::pool()
{
    return poolList.first();
}


// Return the horizontal scrollbar widget.
QScrollBar *QsciScintillaBase::horizontalScrollBar() const
{
    return hsb;
}


// Return the vertical scrollbar widget.
QScrollBar *QsciScintillaBase::verticalScrollBar() const
{
    return vsb;
}


// Return the viewport widget.
QWidget *QsciScintillaBase::viewport() const
{
    return txtarea;
}


// Handle events on behalf of the text area.
bool QsciScintillaBase::eventFilter(QObject *o, QEvent *e)
{
    if (o != txtarea)
        return QWidget::eventFilter(o, e);

    bool used = true;

    switch (e->type())
    {
    case QEvent::ContextMenu:
        contextMenuEvent(static_cast<QContextMenuEvent *>(e));
        break;

    case QEvent::DragEnter:
        dragEnterEvent(static_cast<QDragEnterEvent *>(e));
        break;

    case QEvent::DragLeave:
        dragLeaveEvent(static_cast<QDragLeaveEvent *>(e));
        break;

    case QEvent::DragMove:
        dragMoveEvent(static_cast<QDragMoveEvent *>(e));
        break;

    case QEvent::Drop:
        dropEvent(static_cast<QDropEvent *>(e));
        break;

    case QEvent::MouseButtonDblClick:
        mouseDoubleClickEvent(static_cast<QMouseEvent *>(e));
        break;

    case QEvent::MouseButtonPress:
        mousePressEvent(static_cast<QMouseEvent *>(e));
        break;

    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent *>(e));
        break;

    case QEvent::MouseMove:
        mouseMoveEvent(static_cast<QMouseEvent *>(e));
        break;

    case QEvent::Paint:
        paintEvent(static_cast<QPaintEvent *>(e));
        break;

    case QEvent::Resize:
        resizeEvent(static_cast<QResizeEvent *>(e));
        break;

    case QEvent::Wheel:
        {
            QWheelEvent *we = static_cast<QWheelEvent *>(e);

            setFocus();

            if (we->orientation() == Horizontal || we->state() & ShiftButton)
                QApplication::sendEvent(hsb, we);
            else if (we->orientation() == Vertical)
                QApplication::sendEvent(vsb, we);

            break;
        }

    default:
        used = false;
    }

    return used;
}


// Send a message to the real Scintilla widget using the low level Scintilla
// API.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        long lParam) const
{
    return sci->WndProc(msg, wParam, lParam);
}


// Send a message to the real Scintilla widget that needs a TextRange
// structure.
long QsciScintillaBase::SendScintilla(unsigned int msg, long cpMin, long cpMax,
        char *lpstrText) const
{
    TextRange tr;

    tr.chrg.cpMin = cpMin;
    tr.chrg.cpMax = cpMax;
    tr.lpstrText = lpstrText;

    return sci->WndProc(msg, 0, reinterpret_cast<long>(&tr));
}


// Send a message to the real Scintilla widget that needs a RangeToFormat
// structure.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        QPainter *hdc, const QRect &rc, long cpMin, long cpMax) const
{
    RangeToFormat rf;

    rf.hdc = rf.hdcTarget = reinterpret_cast<SurfaceID>(hdc);

    rf.rc.left = rc.left();
    rf.rc.top = rc.top();
    rf.rc.right = rc.right() + 1;
    rf.rc.bottom = rc.bottom() + 1;

    rf.chrg.cpMin = cpMin;
    rf.chrg.cpMax = cpMax;

    return sci->WndProc(msg, wParam, reinterpret_cast<long>(&rf));
}


// Send a message to the real Scintilla widget that needs a colour.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        const QColor &col) const
{
    long lParam = (col.blue() << 16) | (col.green() << 8) | col.red();

    return sci->WndProc(msg, wParam, lParam);
}


// Send a message to the real Scintilla widget that needs a colour.
long QsciScintillaBase::SendScintilla(unsigned int msg, const QColor &col) const
{
    unsigned long wParam = (col.blue() << 16) | (col.green() << 8) | col.red();

    return sci->WndProc(msg, wParam, 0);
}


// Handle the timer on behalf of the ScintillaQt instance.
void QsciScintillaBase::handleTimer()
{
    sci->Tick();
}


// Re-implemented to handle the context menu.
void QsciScintillaBase::contextMenuEvent(QContextMenuEvent *e)
{
    sci->ContextMenu(Point(e->globalX(), e->globalY()));
}


// Re-implemented to tell the widget it has the focus.
void QsciScintillaBase::focusInEvent(QFocusEvent *)
{
    sci->SetFocusState(true);
}


// Re-implemented to tell the widget it has lost the focus.
void QsciScintillaBase::focusOutEvent(QFocusEvent *)
{
    // Qt sends focus out events even though the focus may now be with a widget
    // that has set us as the focus proxy (eg. an autocompletion list popup).
    // If this is the case we don't tell Scintilla as it would immediately
    // destroy the popup.
    if (qApp->focusWidget() != this)
        sci->SetFocusState(false);
}


// Re-implemented to make sure tabs are passed to the editor.
bool QsciScintillaBase::focusNextPrevChild(bool next)
{
    if (!sci->pdoc->IsReadOnly())
        return false;

    return QWidget::focusNextPrevChild(next);
}


// Handle the selection changing.
void QsciScintillaBase::handleSelection()
{
    if (!QApplication::clipboard()->ownsSelection())
        sci->UnclaimSelection();
}


// Handle key presses.
void QsciScintillaBase::keyPressEvent(QKeyEvent *e)
{
    unsigned key;
    QCString utf8;

    bool shift = e->state() & Qt::ShiftButton;
    bool ctrl = e->state() & Qt::ControlButton;
    bool alt = e->state() & Qt::AltButton;

    switch (e->key())
    {
    case Qt::Key_Down:
        key = SCK_DOWN;
        break;

    case Qt::Key_Up:
        key = SCK_UP;
        break;

    case Qt::Key_Left:
        key = SCK_LEFT;
        break;

    case Qt::Key_Right:
        key = SCK_RIGHT;
        break;

    case Qt::Key_Home:
        key = SCK_HOME;
        break;

    case Qt::Key_End:
        key = SCK_END;
        break;

    case Qt::Key_Prior:
        key = SCK_PRIOR;
        break;

    case Qt::Key_Next:
        key = SCK_NEXT;
        break;

    case Qt::Key_Delete:
        key = SCK_DELETE;
        break;

    case Qt::Key_Insert:
        key = SCK_INSERT;
        break;

    case Qt::Key_Escape:
        key = SCK_ESCAPE;
        break;

    case Qt::Key_Backspace:
        key = SCK_BACK;
        break;

    case Qt::Key_Tab:
        key = SCK_TAB;
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        key = SCK_RETURN;
        break;

    case Qt::Key_Super_L:
        key = SCK_WIN;
        break;

    case Qt::Key_Super_R:
        key = SCK_RWIN;
        break;

    case Qt::Key_Menu:
        key = SCK_MENU;
        break;

    default:
        // See if the input was a single ASCII key.  If so it will be passed to
        // KeyDown to allow it to be filtered.  Correct the modifiers and key
        // for ASCII letters as Qt uses the ASCII code of uppercase letters for
        // Key_A etc.
        utf8 = e->text().utf8();

        if (utf8.length() != 1)
            key = 0;
        else if ((key = utf8[0]) >= 0x80)
            key = 0;
        else if (key >= 0x01 && key <= 0x1a)
            key += 0x40;
        else if (key >= 'A' && key <= 'Z')
            shift = true;
        else if (key >= 'a' && key <= 'z')
        {
            key -= 0x20;
            shift = false;
        }
    }

    if (key)
    {
        bool consumed = false;

        sci->KeyDown(key, shift, ctrl, alt, &consumed);

        if (consumed)
        {
            e->accept();
            return;
        }
    }

    // Add the text if it has a compatible size depending on what Unicode mode
    // we are in.
    if (utf8.length() > 0 && (sci->IsUnicodeMode() || utf8.length() == 1))
    {
        sci->AddCharUTF(utf8.data(), utf8.length());
        e->accept();
    }
    else
        QWidget::keyPressEvent(e);
}




// Handle a mouse button double click.
void QsciScintillaBase::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
    {
        e->ignore();
        return;
    }

    setFocus();

    // Make sure Scintilla will interpret this as a double-click.
    unsigned clickTime = sci->lastClickTime + Platform::DoubleClickTime() - 1;

    bool shift = e->state() & Qt::ShiftButton;
    bool ctrl = e->state() & Qt::ControlButton;
    bool alt = e->state() & Qt::AltButton;

    sci->ButtonDown(Point(e->x(), e->y()), clickTime, shift, ctrl, alt);

    // Remember the current position and time in case it turns into a triple
    // click.
    triple_click_at = e->globalPos();
    triple_click.start(QApplication::doubleClickInterval());
}


// Handle a mouse move.
void QsciScintillaBase::mouseMoveEvent(QMouseEvent *e)
{
    sci->ButtonMove(Point(e->x(), e->y()));
}


// Handle a mouse button press.
void QsciScintillaBase::mousePressEvent(QMouseEvent *e)
{
    setFocus();

    Point pt(e->x(), e->y());

    if (e->button() == Qt::LeftButton)
    {
        unsigned clickTime;

        // It is a triple click if the timer is running and the mouse hasn't
        // moved too much.
        if (triple_click.isActive() && (e->globalPos() - triple_click_at).manhattanLength() < QApplication::startDragDistance())
            clickTime = sci->lastClickTime + Platform::DoubleClickTime() - 1;
        else
            clickTime = sci->lastClickTime + Platform::DoubleClickTime() + 1;

        triple_click.stop();

        bool shift = e->state() & Qt::ShiftButton;
        bool ctrl = e->state() & Qt::ControlButton;
        bool alt = e->state() & Qt::AltButton;

        sci->ButtonDown(pt, clickTime, shift, ctrl, alt);
    }
    else if (e->button() == Qt::MidButton)
    {
        QClipboard *cb = QApplication::clipboard();

        if (cb->supportsSelection())
        {
            int pos = sci->PositionFromLocation(pt);

            sci->SetSelection(pos, pos);
            sci->pasteFromClipboard(QClipboard::Selection);
        }
    }
}


// Handle a mouse button releases.
void QsciScintillaBase::mouseReleaseEvent(QMouseEvent *e)
{
    if (sci->HaveMouseCapture() && e->button() == Qt::LeftButton)
    {
        bool ctrl = e->state() & Qt::ControlButton;

        sci->ButtonUp(Point(e->x(), e->y()), 0, ctrl);
    }
}


// Handle paint events.
void QsciScintillaBase::paintEvent(QPaintEvent *e)
{
    sci->paintEvent(e);
}


// Handle resize events.
void QsciScintillaBase::resizeEvent(QResizeEvent *)
{
    sci->ChangeSize();
}


// Handle the vertical scrollbar.
void QsciScintillaBase::handleVSb(int value)
{
    sci->ScrollTo(value);
}


// Handle the horizontal scrollbar.
void QsciScintillaBase::handleHSb(int value)
{
    sci->HorizontalScrollTo(value);
}


// Handle drag enters.
void QsciScintillaBase::dragEnterEvent(QDragEnterEvent *e)
{
    QsciScintillaBase::dragMoveEvent(e);
}


// Handle drag leaves.
void QsciScintillaBase::dragLeaveEvent(QDragLeaveEvent *)
{
    sci->SetDragPosition(-1);
}


// Handle drag moves.
void QsciScintillaBase::dragMoveEvent(QDragMoveEvent *e)
{
    sci->SetDragPosition(sci->PositionFromLocation(Point(e->pos().x(),
                    e->pos().y())));

    if (sci->pdoc->IsReadOnly() || !QTextDrag::canDecode(e))
    {
        e->ignore();
        return;
    }

    e->acceptAction();
}


// Handle drops.
void QsciScintillaBase::dropEvent(QDropEvent *e)
{
    bool moving;
    const char *s;

    QString text;

    if (sci->pdoc->IsReadOnly() || !QTextDrag::decode(e, text))
    {
        e->ignore();
        return;
    }

    moving = (e->source() == txtarea && e->action() == QDropEvent::Move);

    e->acceptAction();

    QCString cs;

    if (sci->IsUnicodeMode())
    {
        cs = text.utf8();
        s = cs.data();
    }
    else
        s = text.latin1();

    sci->DropAt(sci->posDrop, s, moving, false);
    sci->Redraw();
}
