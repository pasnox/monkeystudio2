// The implementation of various Qt version independent classes used by the
// rest of the port.
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


#include "SciClasses.h"

#include <qevent.h>
#include <qpainter.h>

#include "ScintillaQt.h"
#include "ListBoxQt.h"


// Create a call tip.
SciCallTip::SciCallTip(QWidget *parent, ScintillaQt *sci_)
    : QWidget(parent, 0, Qt::WType_Popup|Qt::WStyle_Customize|Qt::WStyle_NoBorder),
      sci(sci_)
{
    // Ensure that the main window keeps the focus (and the caret flashing)
    // when this is displayed.
    setFocusProxy(parent);
}


// Destroy a call tip.
SciCallTip::~SciCallTip()
{
    // Ensure that the main window doesn't receive a focus out event when
    // this is destroyed.
    setFocusProxy(0);
}


// Paint a call tip.
void SciCallTip::paintEvent(QPaintEvent *)
{
    Surface *surfaceWindow = Surface::Allocate();

    if (!surfaceWindow)
        return;

    QPainter p(this);

    surfaceWindow->Init(&p);
    sci->ct.PaintCT(surfaceWindow);

    delete surfaceWindow;
}


// Handle a mouse press in a call tip.
void SciCallTip::mousePressEvent(QMouseEvent *e)
{
    Point pt;

    pt.x = e->x();
    pt.y = e->y();

    sci->ct.MouseClick(pt);
    sci->CallTipClick();
}



// Add an item and associated command to the popup and enable it if required.
void SciPopup::addItem(const QString &label, int cmd, bool enabled,
        ScintillaQt *sci_)
{
    insertItem(label, this, SLOT(on_triggered(int)), 0, cmd);
    setItemEnabled(cmd, enabled);
    sci = sci_;
}


// Add a separator to the popup.
void SciPopup::addSeparator()
{
    insertSeparator();
}


// A slot to handle a menu action being triggered.
void SciPopup::on_triggered(int cmd)
{
    sci->Command(cmd);
}



SciListBox::SciListBox(QWidget *parent, ListBoxQt *lbx_)
    : QListBox(parent,0,Qt::WType_Popup|Qt::WStyle_Customize|Qt::WStyle_NoBorder|Qt::WStaticContents), lbx(lbx_)
{
    setFocusProxy(parent);

    setFrameShape(StyledPanel);
    setFrameShadow(Plain);

    connect(this,SIGNAL(doubleClicked(QListBoxItem *)),
        SLOT(handleSelection()));

    connect(this,SIGNAL(highlighted(QListBoxItem *)),
        SLOT(ensureCurrentVisible()));
}


int SciListBox::find(const QString &prefix)
{
    return index(findItem(prefix, Qt::CaseSensitive|Qt::BeginsWith));
}



SciListBox::~SciListBox()
{
    // Ensure that the main widget doesn't get a focus out event when this is
    // destroyed.
    setFocusProxy(0);
}


void SciListBox::handleSelection()
{
    if (lbx && lbx->cb_action)
        lbx->cb_action(lbx->cb_data);
}
