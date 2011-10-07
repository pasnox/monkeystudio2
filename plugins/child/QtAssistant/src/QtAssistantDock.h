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
#ifndef QTASSISTANTDOCK_H
#define QTASSISTANTDOCK_H

#include <pDockWidget.h>

#include <QUrl>

class QAction;
class QMenu;
class QActionGroup;
class QStackedWidget;
class QLineEdit;
class QProgressBar;

class QtAssistantDock : public pDockWidget
{
    Q_OBJECT
    
public:
    QtAssistantDock( QWidget* parent = 0 );
    virtual ~QtAssistantDock();
    
    class QtAssistantChild* child() const;

protected:
    QAction* aShow;
    QAction* aContents;
    QAction* aIndex;
    QAction* aBookmarks;
    QAction* aSearch;
    QAction* aFilter;
    QMenu* mFilters;
    QActionGroup* aFilterGroup;
    QStackedWidget* mStacked;
    QWidget* wContents;
    QWidget* wIndex;
    QLineEdit* mLookFor;
    QWidget* wBookmarks;
    QWidget* wSearch;
    QProgressBar* mProgress;
    QAction* aKeywordHelp;
    QAction* aSearchHelp;
    
    class QHelpEngine* mHelpEngine;
    class MkSQtDocInstaller* mDocInstaller;
    class BookmarkManager* mBookmarkManager;
    class BookmarkWidget* bwBookmarks;
    
    bool eventFilter( QObject* obj, QEvent* e );
    
    bool isWordCharacter( const QChar& character ) const;
    QString currentWord( const QString& text, int cursorPos ) const;
    QString currentWord() const;

public slots:
    void openUrl( const QUrl& url, bool newTab = false );
    void openInNewTabUrl( const QUrl& url );
    void openUrls( const QMap<QString, QUrl>& links, const QString& keyword, bool newTab = false );

protected slots:
    void aPagesGroup_triggered( QAction* action );
    void updateFilters( const QString& filter );
    void aFilterGroup_triggered( QAction* action );
    void open_customContextMenuRequested( const QPoint& pos );
    void disableSearchLineEdit();
    void enableSearchLineEdit();
    void filterIndices( const QString& filter );
    void searchingStarted();
    void searchingFinished( int hits );
    void search();
    void addBookmark();
    void keywordHelp();
    void searchHelp();

signals:
    void helpShown();
};

#endif // QTASSISTANTDOCK_H
