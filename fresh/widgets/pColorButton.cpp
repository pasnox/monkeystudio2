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
#include "pColorButton.h"

#include <QColorDialog>

pColorButton::pColorButton( QWidget* parent )
    : QPushButton( parent )
{
    init( QColor( Qt::black ) );
}

pColorButton::pColorButton( const QColor& color, QWidget* parent )
    : QPushButton( parent )
{
    init( color );
}

pColorButton::~pColorButton()
{
}

void pColorButton::init( const QColor& color )
{
    connect( this, SIGNAL( clicked() ), this, SLOT( onClicked() ) );
    
    mColorNameHidden = false;
    
    setColor( color );
}

void pColorButton::updateColorName()
{
    setToolTip( mColor.name().replace( "#", QString::number( mColor.alpha(), 16 ).prepend( "#" ) ) );
    
    if ( mColorNameHidden )
    {
        setText( QString::null );
    }
    else
    {
        setText( toolTip() );
    }
}

bool pColorButton::isColorNameHidden() const
{
    return mColorNameHidden;
}

void pColorButton::setColorNameHidden( bool hidden )
{
    mColorNameHidden = hidden;
    
    updateColorName();
}

const QColor& pColorButton::color() const
{
    return mColor;
}

void pColorButton::setColor( const QColor& color )
{
    mColor = color;
    
    QPixmap pixmap( iconSize() );
    pixmap.fill( mColor );
    
    setIcon( QIcon( pixmap ) );
    updateColorName();
}

void pColorButton::onClicked()
{
    bool ok;
    const QRgb rgb = QColorDialog::getRgba( mColor.rgba(), &ok, window() );
    
    if ( ok )
    {
        QColor color = QColor::fromRgba( rgb );
        setColor( color );
    }
}
