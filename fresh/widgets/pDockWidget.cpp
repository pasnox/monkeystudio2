/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pDockWidget.cpp
** Date      : 2008-01-14T00:27:42
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
#include "pDockWidget.h"

#include <QStyle>

pDockWidget::pDockWidget( const QString& t, QWidget* w, Qt::WindowFlags f )
	: QDockWidget( t, w, f )
{}

pDockWidget::pDockWidget( QWidget* w, Qt::WindowFlags f )
	: QDockWidget( w, f )
{}

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

QSize pDockWidget::sizeHint() const
{ return mSize.isValid() && !isFloating() ? mSize : QDockWidget::sizeHint(); }

void pDockWidget::setVisible( bool b )
{
	if ( !b && !isFloating() )		mSize = contentsSize();	QDockWidget::setVisible( b );
}
