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
