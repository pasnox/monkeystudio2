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
#include "ExMdiArea.h"

#include <QMdiSubWindow>
#include <QChildEvent>
#include <QDebug>

static const Qt::WindowFlags CustomizeWindowFlags =
      Qt::FramelessWindowHint
    | Qt::CustomizeWindowHint
    | Qt::WindowTitleHint
    | Qt::WindowSystemMenuHint
    | Qt::WindowMinimizeButtonHint
    | Qt::WindowMaximizeButtonHint
    | Qt::WindowMinMaxButtonsHint;

ExMdiArea::ExMdiArea( QWidget* parent )
    : QMdiArea( parent ), mMode( ExMdiArea::Undefined )
{
    setActivationOrder( QMdiArea::ActivationHistoryOrder );
    setDocumentMode( true );
    
    connect( this, SIGNAL( subWindowActivated( QMdiSubWindow* ) ), this, SLOT( updateWindowFlags( QMdiSubWindow* ) ) );
    
    setDocumentViewMode( ExMdiArea::MDI );
}

ExMdiArea::DocumentViewMode ExMdiArea::documentViewMode() const
{
    return mMode;
}

void ExMdiArea::setDocumentViewMode( ExMdiArea::DocumentViewMode mode )
{
    if ( mMode == mode ) {
        return;
    }
    
    mMode = mode;
    updateWindowsFlags();
    
    switch ( mode ) {
        case ExMdiArea::Undefined:
            Q_ASSERT( 0 );
            break;
        case ExMdiArea::MDI:
            setViewMode( QMdiArea::SubWindowView );
            break;
        case ExMdiArea::NoTabs:
            setViewMode( QMdiArea::SubWindowView );
            break;
        case ExMdiArea::TopTabs:
            setViewMode( QMdiArea::TabbedView );
            setTabPosition( QTabWidget::North );
            break;
        case ExMdiArea::BottomTabs:
            setViewMode( QMdiArea::TabbedView );
            setTabPosition( QTabWidget::South );
            break;
        case ExMdiArea::LeftTabs:
            setViewMode( QMdiArea::TabbedView );
            setTabPosition( QTabWidget::West );
            break;
        case ExMdiArea::RightTabs:
            setViewMode( QMdiArea::TabbedView );
            setTabPosition( QTabWidget::East );
            break;
    }
}

Qt::WindowFlags ExMdiArea::documentViewModeWindowFlags( ExMdiArea::DocumentViewMode mode ) const
{
    if ( mode == ExMdiArea::NoTabs ) {
        return Qt::FramelessWindowHint | Qt::CustomizeWindowHint;
    }
    
    Qt::WindowFlags windowFlags = 0;
    Qt::WindowFlags windowType = windowFlags & Qt::WindowType_Mask;
    
    if ( windowType == Qt::Dialog || windowFlags & Qt::MSWindowsFixedSizeDialogHint ) {
        windowFlags |= Qt::WindowTitleHint | Qt::WindowSystemMenuHint;
    }

    // Set standard flags if none of the customize flags are set
    if ( !( windowFlags & CustomizeWindowFlags ) ) {
        windowFlags |= Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint;
    }
    else if ( windowFlags & Qt::FramelessWindowHint && windowFlags & Qt::WindowStaysOnTopHint ) {
        windowFlags = Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
    }
    else if ( windowFlags & Qt::FramelessWindowHint ) {
        windowFlags = Qt::FramelessWindowHint;
    }

    windowFlags &= ~windowType;
    windowFlags |= Qt::SubWindow;
    return windowFlags;
}

void ExMdiArea::updateWindowFlags( QMdiSubWindow* window )
{
    if ( window ) {
        window->setWindowFlags( documentViewModeWindowFlags( mMode ) );
    }
}

void ExMdiArea::updateWindowsFlags()
{
    foreach ( QMdiSubWindow* window, subWindowList() ) {
        updateWindowFlags( window );
    }
    
    QMdiSubWindow* window = currentSubWindow();
    
    if ( window ) {
        if ( window->isMaximized() ) {
            window->showNormal();
            window->showMaximized();
        }
        else {
            window->showMaximized();
            window->showNormal();
        }
    }
}
