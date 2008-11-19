/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
#include "pAction.h"

#include "pDockWidget.h"

#include <QStyle>

/*!
	\brief Create a new pDockWidget instance
	\param title The dock title
	\param parent The dock parent
	\param flags The dock window flags
*/
pDockWidget::pDockWidget( const QString& title, QWidget* parent, Qt::WindowFlags flags )
        : QDockWidget( title, parent, flags ), 
		mToggleViewPAction( 0 ),
		mAutoFocusWidget (0)
{}

/*!
	\brief Create a new pDockWidget instance
	\param parent The dock parent
	\param flags The dock window flags
*/
pDockWidget::pDockWidget( QWidget* parent, Qt::WindowFlags flags )
        : QDockWidget( parent, flags ), 
		mToggleViewPAction( 0 ),
		mAutoFocusWidget (0)
{}

pDockWidget::~pDockWidget()
{
        if ( mToggleViewPAction )
                delete mToggleViewPAction;
}

QSize pDockWidget::contentsSize() const
{
	QSize contents = size();
	int fw = style()->pixelMetric( QStyle::PM_DockWidgetFrameWidth );
	QSize frame( 2 *fw, fw );
#ifdef Q_OS_WIN
	frame -= QSize( 0, 3 );
#endif
	contents -= frame;
	return contents;
}

/*!
	\details Return the dock sizeHint
*/
QSize pDockWidget::sizeHint() const
{ return mSize.isValid() && !isFloating() ? mSize : QDockWidget::sizeHint(); }

/*!
	\details Create pAction, which will togle visible state of dock and installToggleViewAction
    it to "Docks" menu
    \param defaultShortcut Default shortcut for dock
*/
pAction* pDockWidget::toggleViewPAction (QString defaultShortcut)
{
    if (mToggleViewPAction)
        return mToggleViewPAction;
    
    if (! windowTitle().isEmpty())
    {
        mToggleViewPAction = new pAction (  "a" + windowTitle(), 
                                            windowIcon(), 
                                            tr("Show ") + windowTitle(), 
                                            defaultShortcut,
                                            "Docks");
        mToggleViewPAction->setCheckable (true);
        mToggleViewPAction->setChecked (isVisible());
        connect (mToggleViewPAction, SIGNAL (toggled (bool)), this, SLOT (onToggleViewActionTriggered (bool)));
        connect (this, SIGNAL (visibilityChanged (bool)), mToggleViewPAction, SLOT (setChecked (bool)));
        return mToggleViewPAction;
    }
    else
    {
        return NULL;
    }
}


/*!
	\details Set widget, which will recieve focus automaticaly, when toggled action for show dock
	\param widget Widget
*/
void pDockWidget::setAutoFocusWidget (QWidget* widget)
{
	mAutoFocusWidget = widget;
}

/*!
	\details Set dock visibility
	\param visible if tru, dock will be visible, else dock is hidden
*/
void pDockWidget::setVisible( bool visible )
{
	if ( !visible && !isFloating() )
		mSize = contentsSize();
	QDockWidget::setVisible( visible );
}

/*!
	\details Set dock visibility, if nessesery - set focus to widget
	\param checked - if true - show, else - hide
*/
#include <QDebug>
void pDockWidget::onToggleViewActionTriggered (bool checked)
{
	if (checked && mAutoFocusWidget)
	{
		mAutoFocusWidget->setFocus();
		activateWindow();
		qDebug () << isActiveWindow() << mAutoFocusWidget->hasFocus();
		qDebug () << mAutoFocusWidget << mAutoFocusWidget->focusPolicy();
		qDebug () << QApplication::focusWidget();
	}
	setVisible (checked);
}
