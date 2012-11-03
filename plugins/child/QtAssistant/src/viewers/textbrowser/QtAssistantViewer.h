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
#ifndef QTASSISTANTVIEWER_H
#define QTASSISTANTVIEWER_H

#include <QTextBrowser>
#include <QAction>

class QHelpEngine;
class QtAssistantChild;

class QtAssistantViewer : public QTextBrowser
{
    Q_OBJECT
    
public:
    QtAssistantViewer( QHelpEngine* engine, QtAssistantChild* child, const QUrl& homeUrl = QUrl() );
    
    virtual void setSource( const QUrl& url );

    inline bool hasSelection() const
    { return textCursor().hasSelection(); }
    
    inline bool isUndoAvailable() const
    { return document()->isUndoAvailable(); }
    
    inline bool isRedoAvailable() const
    { return document()->isRedoAvailable(); }
    
    inline bool isCopyAvailable() const
    { return hasSelection(); }
    
    inline bool isPasteAvailable() const
    { return canPaste(); }
    
    inline QMap<QUrl, QString> historyUrls() const {
        QMap<QUrl, QString> urls;
        
        for ( int i = backwardHistoryCount(); i < forwardHistoryCount(); i++ ) {
            if ( i == 0 ) {
                urls[ source() ] = documentTitle();
            }
            else {
                urls[ historyUrl( i ) ] = historyTitle( i );
            }
        }
        
        for ( int i = 1; i < forwardHistoryCount(); i++ ) {
            urls[ historyUrl( i ) ] = historyTitle( i );
        }
        
        return urls;
    }

    int zoom() const;
    void setZoom( int zoom );
    void resetZoom();
    
    virtual QVariant loadResource( int type, const QUrl& name );

protected:
    virtual void wheelEvent( QWheelEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );

private:
    QHelpEngine* mEngine;
    QtAssistantChild* mChild;
    QUrl mHomeUrl;

signals:
    void cutAvailable( bool enabled );
    void pasteAvailable( bool enabled );
    void actionsChanged();
};

#endif // QTASSISTANTVIEWER_H
