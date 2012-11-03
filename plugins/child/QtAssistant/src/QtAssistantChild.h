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
#ifndef QTASSISTANTCHILD_H
#define QTASSISTANTCHILD_H

#include <workspace/pAbstractChild.h>

class QHelpEngine;
class QtAssistantInlineSearch;
class QToolButton;
class QComboBox;

class QtAssistantChild : public pAbstractChild
{
    Q_OBJECT

    friend class QtAssistant;
    friend class QtAssistantDock;

public:
    static QtAssistantChild* instance( QHelpEngine* engine, QWidget* parent = 0, bool create = true );

    QtAssistantChild( QHelpEngine* engine, QWidget* parent = 0 );
    virtual ~QtAssistantChild();

    virtual QString fileBuffer() const;
    virtual QString context() const;
    virtual void initializeContext( QToolBar* tb );
    virtual QPoint cursorPosition() const;
    virtual pEditor* editor() const;
    virtual bool isModified() const;
    virtual bool isUndoAvailable() const;
    virtual bool isRedoAvailable() const;
    virtual bool isCopyAvailable() const;
    virtual bool isPasteAvailable() const;
    virtual bool isGoToAvailable() const;
    virtual bool isPrintAvailable() const;

    class QtAssistantViewer* viewer( int index = -1 ) const;
    QtAssistantViewer* newEmptyViewer( int zoom = 1 );

protected:
    QHelpEngine* mEngine;
    QTabWidget* twPages;
    QtAssistantInlineSearch* isSearch;
    QToolButton* tbCloneTab;
    QAction* aPrevious;
    QAction* aNext;
    QAction* aHome;
    QAction* aSearchText;
    QAction* aZoomIn;
    QAction* aZoomOut;
    QAction* aZoomReset;
    QAction* aAddNewPage;
    QAction* aNextTab;
    QAction* aPreviousTab;
    QPointer<QComboBox> cbUrl;
    bool mFirstOpenUrl;

    void find( QString ttf, bool forward, bool backward );

public slots:
    virtual void undo();
    virtual void redo();
    virtual void cut();
    virtual void copy();
    virtual void paste();
    virtual void goTo();
    virtual void goTo( const QPoint& position, int selectionLength = -1 );
    virtual void invokeSearch();
    virtual void saveFile();
    virtual void backupFileAs( const QString& fileName );
    virtual bool openFile( const QString& fileName, const QString& codec );
    virtual void closeFile();
    virtual void reload();
    virtual void setFileBuffer( const QString& content );
    virtual void printFile();
    virtual void quickPrintFile();

    void openUrl( const QUrl& url );
    void openUrlInNewTab( const QUrl& url );
    void cloneTab();
    void closeTab( int index );

    void focusCurrentTab();
    void saveSession();
    void restoreSession();

    void previousTab();
    void nextTab();
    void previousPage();
    void nextPage();
    void homePage();
    void zoomIn();
    void zoomOut();
    void zoomReset();

    void findNext();
    void findPrevious();
    void findCurrentText( const QString& text );

protected slots:
    void updateContextActions();
    void viewer_sourceChanged( const QUrl& url );
    void viewer_actionsChanged();
    void cbUrl_currentIndexChanged( int index );
};

#endif // QTASSISTANTCHILD_H
