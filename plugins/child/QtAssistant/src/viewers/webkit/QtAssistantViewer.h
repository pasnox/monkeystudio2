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

#include <QWebView>
#include <QWebHistory>
#include <QWebFrame>
#include <QAction>

class QHelpEngine;
class QtAssistantChild;

class QtAssistantViewer : public QWebView
{
    Q_OBJECT
    
public:
    QtAssistantViewer( QHelpEngine* engine, QtAssistantChild* child, const QUrl& homeUrl = QUrl() );
    
    void setSource( const QUrl& url );
    
    inline QUrl source() const
    { return url(); }
    
    inline QString documentTitle() const
    { return title(); }

    inline bool hasSelection() const
    { return !selectedText().isEmpty(); }
    
    inline bool isUndoAvailable() const
    { return pageAction( QWebPage::Undo )->isEnabled(); }
    
    inline bool isRedoAvailable() const
    { return pageAction( QWebPage::Redo )->isEnabled(); }
    
    inline bool isCopyAvailable() const
    { return pageAction( QWebPage::Copy )->isEnabled(); }
    
    inline bool isBackwardAvailable() const
    { return pageAction( QWebPage::Back )->isEnabled(); }
    
    inline bool isForwardAvailable() const
    { return pageAction( QWebPage::Forward )->isEnabled(); }
    
    inline bool isPasteAvailable() const
    { return pageAction( QWebPage::Paste )->isEnabled(); }
    
    inline QMap<QUrl, QString> historyUrls() const {
        QMap<QUrl, QString> urls;
        
        foreach ( const QWebHistoryItem& item, history()->items() ) {
            urls[ item.url() ] = item.title();
        }
        
        return urls;
    }
    
    int zoom() const;
    void setZoom( int zoom );
    void resetZoom();
    void zoomIn( int range = 1 );
    void zoomOut( int range = 1 );
    
    inline QString toHtml() const
    { return page()->mainFrame()->toHtml(); }
    
    /*

    

    inline bool hasSelection() const
    { return !selectedText().isEmpty(); }

    

    inline void copy()
    { triggerPageAction( QWebPage::Copy ); }

    inline bool isForwardAvailable() const
    { return pageAction( QWebPage::Forward )->isEnabled(); }

    inline bool isBackwardAvailable() const
    { return pageAction( QWebPage::Back )->isEnabled(); }*/

public slots:
    void home();
    inline void backward()
    { back(); }
    
    inline void undo()
    { pageAction( QWebPage::Undo )->trigger(); }
    
    inline void redo()
    { pageAction( QWebPage::Redo )->trigger(); }
    
    inline void cut()
    { pageAction( QWebPage::Cut )->trigger(); }
    
    inline void copy()
    { pageAction( QWebPage::Copy )->trigger(); }
    
    inline void paste()
    { pageAction( QWebPage::Paste )->trigger(); }

protected:
    virtual void wheelEvent( QWheelEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );

private slots:
    void actionChanged();
    void loadFinished( bool ok );

private:
    QHelpEngine* mEngine;
    QtAssistantChild* mChild;
    QUrl mHomeUrl;

signals:
    void copyAvailable( bool enabled );
    void cutAvailable( bool enabled );
    void pasteAvailable( bool enabled );
    void undoAvailable( bool enabled );
    void redoAvailable( bool enabled );
    void forwardAvailable( bool enabled );
    void backwardAvailable( bool enabled );
    void actionsChanged();
    void highlighted( const QString& );
    void sourceChanged( const QUrl& );
};

#endif // QTASSISTANTVIEWER_H
