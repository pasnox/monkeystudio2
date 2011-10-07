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
#ifndef PQUEUEDMESSAGETOOLBAR_H
#define PQUEUEDMESSAGETOOLBAR_H

#include "../objects/MonkeyExport.h"
#include "pQueuedMessageWidget.h"

#include <QToolBar>

class Q_MONKEY_EXPORT pQueuedMessageToolBar : public QToolBar
{
    Q_OBJECT
    
public:
    pQueuedMessageToolBar( QWidget* parent = 0 );
    virtual ~pQueuedMessageToolBar();
    
    static void setDefaultTimeout( int timeout );
    static const int& defaultTimeout();
    
    static void setDefaultPixmap( const QPixmap& pixmap );
    static const QPixmap& defaultPixmap();
    
    static void setDefaultBackground( const QBrush& brush );
    static const QBrush& defaultBackground();
    
    static void setDefaultForeground( const QBrush& brush );
    static const QBrush& defaultForeground();

protected:
    static int mDefaultTimeout;
    static QPixmap* mDefaultPixmap;
    static QBrush mDefaultBackground;
    static QBrush mDefaultForeground;
    QPalette mDefaultPalette;
    pQueuedMessageWidget* mQueuedWidget;

public slots:
    int appendMessage( const pQueuedMessage& message );
    int appendMessage( const QString& message, int milliseconds = pQueuedMessageToolBar::defaultTimeout(), const QPixmap& pixmap = pQueuedMessageToolBar::defaultPixmap(), const QBrush& background = pQueuedMessageToolBar::defaultBackground(), const QBrush& foreground = pQueuedMessageToolBar::defaultForeground() );
    void removeMessage( const pQueuedMessage& message );
    void removeMessage( int id );

protected slots:
    void messageShown( const pQueuedMessage& message );
    void messageCleared();
};

#endif // PQUEUEDMESSAGETOOLBAR_H
