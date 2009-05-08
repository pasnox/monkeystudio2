#include "qCtagsSenseSearchPopup.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QScrollBar>

#include <QDebug>

qCtagsSenseSearchPopup::qCtagsSenseSearchPopup( QWidget* widget )
	: QTreeView( widget )
{
	eatFocusOut = true;
	mWidget = 0;
	
	setHeaderHidden( true );
	setUniformRowHeights( true );
	setFocusPolicy( Qt::NoFocus );
	setEditTriggers( QAbstractItemView::NoEditTriggers );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setSelectionBehavior( QAbstractItemView::SelectRows );
	setSelectionMode( QAbstractItemView::SingleSelection );
	
	installEventFilter( this );
	setWidget( widget );
}

qCtagsSenseSearchPopup::~qCtagsSenseSearchPopup()
{
	setWidget( 0 );
}

bool qCtagsSenseSearchPopup::eventFilter( QObject* object, QEvent* event )
{
    if ( eatFocusOut && object == mWidget && event->type() == QEvent::FocusOut )
	{
        if ( isVisible() )
		{
            return true;
		}
    }
	
	if ( object == mWidget && event->type() == QEvent::KeyPress )
	{
		QKeyEvent* ke = static_cast<QKeyEvent*>( event );
		
		if ( ke->key() == Qt::Key_Space && ke->modifiers() == Qt::ControlModifier )
		{
			showPopup();
			return true;
		}
	}
	
	if ( object == this )
	{
		return searchPopupEventFilter( event );
	}
	
	return QFrame::eventFilter( object, event );
}

bool qCtagsSenseSearchPopup::searchPopupEventFilter( QEvent* e )
{	
    switch ( e->type() )
	{
		case QEvent::KeyPress:
		{
			QKeyEvent* ke = static_cast<QKeyEvent*>( e );
			
			QModelIndex curIndex = currentIndex();
			QModelIndexList selList = selectionModel()->selectedIndexes();
			
			const int key = ke->key();
			
			// Handle popup navigation keys. These are hardcoded because up/down might make the
			// widget do something else (lineedit cursor moves to home/end on mac, for instance)
			switch ( key )
			{
				case Qt::Key_End:
				case Qt::Key_Home:
					if ( ke->modifiers() & Qt::ControlModifier )
					{
						return false;
					}
					break;
				case Qt::Key_Up:
					if ( !curIndex.isValid() )
					{
						return true;
					}
					else if ( curIndex.row() == 0 )
					{
						return true;
					}
					return false;
				case Qt::Key_Down:
					if ( !curIndex.isValid() )
					{
						return true;
					}
					else if ( curIndex.row() == model()->rowCount() -1 )
					{
						return true;
					}
					return false;
				case Qt::Key_PageUp:
				case Qt::Key_PageDown:
					return false;
			}
			
			// Send the event to the widget. If the widget accepted the event, do nothing
			// If the widget did not accept the event, provide a default implementation
			eatFocusOut = false;
			static_cast<QObject*>( mWidget )->event( ke );
			eatFocusOut = true;
			if ( !mWidget || e->isAccepted() || !isVisible() )
			{
				// widget lost focus, hide the popup
				if ( mWidget && ( !mWidget->hasFocus()
#ifdef QT_KEYPAD_NAVIGATION
					|| ( QApplication::keypadNavigationEnabled() && !mWidget->hasEditFocus() )
#endif
					) )
					hide();
				if ( e->isAccepted() )
				{
					return true;
				}
			}
			
			// default implementation for keys not handled by the widget when popup is open
			switch ( key )
			{
#ifdef QT_KEYPAD_NAVIGATION
				case Qt::Key_Select:
					if ( !QApplication::keypadNavigationEnabled() )
					{
						break;
					}
#endif
				case Qt::Key_Return:
				case Qt::Key_Enter:
				case Qt::Key_Tab:
					hide();
					break;
				case Qt::Key_F4:
					if ( ke->modifiers() & Qt::AltModifier )
					{
						hide();
					}
					break;
				case Qt::Key_Backtab:
				case Qt::Key_Escape:
					hide();
					break;
				default:
					break;
			}
			
			return true;
		}
#ifdef QT_KEYPAD_NAVIGATION
		case QEvent::KeyRelease:
		{
			QKeyEvent* ke = static_cast<QKeyEvent *>( e );
			if ( QApplication::keypadNavigationEnabled() && ke->key() == Qt::Key_Back )
			{
				// Send the event to the 'widget'. This is what we did for KeyPress, so we need
				// to do the same for KeyRelease, in case the widget's KeyPress event set
				// up something (such as a timer) that is relying on also receiving the
				// key release. I see this as a bug in Qt, and should really set it up for all
				// the affected keys. However, it is difficult to tell how this will affect
				// existing code, and I can't test for every combination!
				eatFocusOut = false;
				static_cast<QObject*>( mWidget )->event( ke );
				eatFocusOut = true;
			}
			break;
		}
#endif
		case QEvent::MouseButtonPress:
		{
#ifdef QT_KEYPAD_NAVIGATION
			if ( QApplication::keypadNavigationEnabled() )
			{
				// if we've clicked in the widget (or its descendant), let it handle the click
				QWidget* source = qobject_cast<QWidget*>( o );
				if ( source )
				{
					QPoint pos = source->mapToGlobal( (static_cast<QMouseEvent *>( e ) )->pos() );
					QWidget* target = QApplication::widgetAt( pos );
					if ( target && ( mWidget->isAncestorOf( target ) || target == mWidget ) )
					{
						eatFocusOut = false;
						static_cast<QObject*>( target )->event( e );
						eatFocusOut = true;
						return true;
					}
				}
			}
#endif
			if ( !underMouse() )
			{
				hide();
				return true;
			}
		}
		
		return false;

		case QEvent::InputMethod:
		case QEvent::ShortcutOverride:
			QApplication::sendEvent( mWidget, e );
			break;
		default:
			return false;
    }
	
    return false;
}

void qCtagsSenseSearchPopup::setWidget( QWidget* widget )
{
	if ( mWidget )
	{
		mWidget->removeEventFilter( this );
	}
	
	mWidget = widget;
	
	if ( mWidget )
	{
		setVisible( false );
		setParent( mWidget, Qt::Popup );
		setFocusProxy( mWidget );
		mWidget->installEventFilter( this );
	}
}

void qCtagsSenseSearchPopup::showPopup( const QRect& rect )
{
	QWidget* widget = mWidget;
	QTreeView* popup = this;
	
	// original QCompleter code
    const QRect screen = QApplication::desktop()->availableGeometry(widget);
    Qt::LayoutDirection dir = widget->layoutDirection();
    QPoint pos;
    int rw, rh, w;
    int h = (popup->sizeHintForRow(0) * 10/*qMin(7, popup->model()->rowCount()) + 3*/) + 3; // pasnox fix
    QScrollBar *hsb = popup->horizontalScrollBar();
    if (hsb && hsb->isVisible())
        h += popup->horizontalScrollBar()->sizeHint().height();

    if (rect.isValid()) {
        rh = rect.height();
        w = rw = rect.width();
        pos = widget->mapToGlobal(dir == Qt::RightToLeft ? rect.bottomRight() : rect.bottomLeft());
    } else {
        rh = widget->height();
        rw = widget->width();
        pos = widget->mapToGlobal(QPoint(0, widget->height() - 2));
        w = widget->width();
    }

    if ((pos.x() + rw) > (screen.x() + screen.width()))
        pos.setX(screen.x() + screen.width() - w);
    if (pos.x() < screen.x())
        pos.setX(screen.x());
    if (((pos.y() + rh) > (screen.y() + screen.height())) && ((pos.y() - h - rh) >= 0))
        pos.setY(pos.y() - qMax(h, popup->minimumHeight()) - rh + 2);

    popup->setGeometry(pos.x(), pos.y(), w, h);

    if (popup->model()->hasChildren()) // pasnox fixed show for nothing
        popup->show();
	else
		popup->hide();
}
