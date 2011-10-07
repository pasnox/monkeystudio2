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
#include "pStylesToolButton.h"
#include "pStylesActionGroup.h"

#include <QMenu>

pStylesToolButton::pStylesToolButton( const QString& textFormat, QWidget* parent )
    : QToolButton( parent )
{
    setPopupMode( QToolButton::MenuButtonPopup );
    
    mActions = new pStylesActionGroup( textFormat, this );
    mMenu = new QMenu( this );
    
    refreshActions();
    
    connect( mActions, SIGNAL( styleSelected(const QString& ) ), this, SIGNAL( styleSelected(const QString& ) ) );
}

QStringList pStylesToolButton::availableStyles() const
{
    return mActions->availableStyles();
}

QString pStylesToolButton::systemStyle() const
{
    return mActions->systemStyle();
}

QAction* pStylesToolButton::systemAction() const
{
    return mActions->systemAction();
}

QString pStylesToolButton::applicationStyle() const
{
    return mActions->applicationStyle();
}

QAction* pStylesToolButton::applicationAction() const
{
    return mActions->applicationAction();
}

bool pStylesToolButton::isCheckableActions() const
{
    return mActions->isCheckable();
}

QString pStylesToolButton::currentStyle() const
{
    return mActions->currentStyle();
}

void pStylesToolButton::refreshActions()
{
    mActions->refreshActions();
    mMenu->addActions( mActions->actions() );
    
    setMenu( mMenu );
    setDefaultAction( mActions->systemAction() );
}

void pStylesToolButton::setCheckableActions( bool checkable )
{
    mActions->setCheckable( checkable );
}

void pStylesToolButton::setCurrentStyle( const QString& style )
{
    mActions->setCurrentStyle( style );
}
