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
#include "QtAssistantViewer.h"
#include "QtAssistantChild.h"

#include <QHelpEngine>

QtAssistantViewer::QtAssistantViewer( QHelpEngine* engine, QtAssistantChild* child, const QUrl& homeUrl )
    : QTextBrowser( child )
{
    Q_ASSERT( engine );
    Q_ASSERT( child );
    
    mEngine = engine;
    mChild = child;
    mHomeUrl = homeUrl;

    connect( this, SIGNAL( copyAvailable( bool ) ), this, SIGNAL( cutAvailable( bool ) ) );
    
    connect( this, SIGNAL( copyAvailable( bool ) ), this, SIGNAL( actionsChanged() ) );
    //connect( this, SIGNAL( cutAvailable( bool ) ), this, SIGNAL( actionsChanged() ) ); // emited by copyAvailable
    connect( this, SIGNAL( pasteAvailable( bool ) ), this, SIGNAL( actionsChanged() ) ); // never emited but it's not so important
    connect( this, SIGNAL( undoAvailable( bool ) ), this, SIGNAL( actionsChanged() ) );
    connect( this, SIGNAL( redoAvailable( bool ) ), this, SIGNAL( actionsChanged() ) );
    connect( this, SIGNAL( backwardAvailable( bool ) ), this, SIGNAL( actionsChanged() ) );
    connect( this, SIGNAL( forwardAvailable( bool ) ), this, SIGNAL( actionsChanged() ) );
    connect( this, SIGNAL( historyChanged() ), this, SIGNAL( actionsChanged() ) );
}

void QtAssistantViewer::setSource( const QUrl& url )
{
    mHomeUrl = mHomeUrl.isValid() ? mHomeUrl : url;
    QTextBrowser::setSource( url );
}

int QtAssistantViewer::zoom() const
{
    return 0;
}

void QtAssistantViewer::setZoom( int zoom )
{
    //
}

void QtAssistantViewer::resetZoom()
{
    setZoom( 1 );
}

QVariant QtAssistantViewer::loadResource( int type, const QUrl& name )
{
    if ( name.scheme().toLower() == "qthelp" ) {
        const QByteArray data = mEngine->fileData( name );
        return data;
    }
    
    return QTextBrowser::loadResource( type, name );
}

void QtAssistantViewer::wheelEvent( QWheelEvent* e )
{
    /*if ( e->modifiers() & Qt::ControlModifier )
    {
        const int delta = e->delta();
        if ( delta > 0 )
            zoomOut();
        else if ( delta < 0 )
            zoomIn();
        e->accept();
        return;
    }*/
    
    QTextBrowser::wheelEvent( e );
}

void QtAssistantViewer::mouseReleaseEvent( QMouseEvent* e )
{
    /*if ( e->button() == Qt::XButton1 )
    {
        triggerPageAction( QWebPage::Back );
        return;
    }

    if ( e->button() == Qt::XButton2 )
    {
        triggerPageAction( QWebPage::Forward );
        return;
    }*/

    QTextBrowser::mouseReleaseEvent( e );
}
