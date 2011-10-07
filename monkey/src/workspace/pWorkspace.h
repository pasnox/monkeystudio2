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
#ifndef PWORKSPACE_H
#define PWORKSPACE_H

#include <MonkeyExport.h>

#include "ExMdiArea.h"
#include "consolemanager/pConsoleManager.h"

#include <QFrame>

class pAbstractChild;
class pChild;
class QMainWindow;
class XUPProjectItem;
class QFileSystemWatcher;
class QTimer;
class QVBoxLayout;
class pOpenedFileExplorer;
class QActionGroup;
class QToolBar;

class Q_MONKEY_EXPORT pWorkspace : public QFrame
{
    Q_OBJECT

public:
    pWorkspace( QMainWindow* parent = 0 );
    
    static QString defaultContext();
    
    void loadSettings();
    void initMultiToolBar( QToolBar* tb );
    pOpenedFileExplorer* dockWidget() const;
    QFileSystemWatcher* fileWatcher() const;
    
    pAbstractChild* document( int index ) const;
    int indexOfDocument( pAbstractChild* document ) const;
    QList<pAbstractChild*> documents() const;
    
    void setCurrentDocument( pAbstractChild* document );
    pAbstractChild* currentDocument() const;
    
    void goToLine( const QString& fileName, const QPoint& pos, const QString& codec, int selectionLength = -1 );
    void closeDocument( pAbstractChild* document, bool showDialog = true );
    ExMdiArea::DocumentViewMode documentMode() const;
    void handleDocument( pAbstractChild* document );
    void unhandleDocument( pAbstractChild* document );

public slots:
    void closeFile( const QString& );
    
    void closeCurrentDocument();
    bool closeAllDocuments();
    
    void activateNextDocument();
    void activatePreviousDocument();
    void focusEditor();
    
    void tile();
    void cascade();
    void minimize();
    void restore();
    
    void setDocumentMode( ExMdiArea::DocumentViewMode mode );
    pAbstractChild* createNewTextEditor();
    
protected:
    QVBoxLayout* mLayout;
    ExMdiArea* mMdiArea;
    QActionGroup* mViewModesGroup;
    pOpenedFileExplorer* mOpenedFileExplorer;
    QFileSystemWatcher* mFileWatcher;
    QTimer* mContentChangedTimer;
    static int CONTENT_CHANGED_TIME_OUT;
    static QString DEFAULT_CONTEXT;
    
    bool eventFilter( QObject* object, QEvent* event );
    void updateGuiState( pAbstractChild* document );

protected slots:
    void document_fileOpened();
    void document_contentChanged();
    void document_modifiedChanged( bool modified );
    void document_fileClosed();
    void document_fileReloaded();

    void contentChangedTimer_timeout();
    void multitoolbar_notifyChanges();
    void viewModes_triggered( QAction* action );
    void mdiArea_subWindowActivated( QMdiSubWindow* document );
    
    void internal_urlsDropped( const QList<QUrl>& urls );
    void internal_currentProjectChanged( XUPProjectItem* currentProject, XUPProjectItem* previousProject );

public slots:
    // file menu
    void fileNew_triggered();
    void fileSessionSave_triggered();
    void fileSessionRestore_triggered();
    void fileSaveCurrent_triggered();
    void fileSaveAll_triggered();
    void fileCloseCurrent_triggered();
    void fileCloseAll_triggered();
    void fileReload_triggered();
    void fileSaveAsBackup_triggered();
    void fileQuickPrint_triggered();
    void filePrint_triggered();
    void fileExit_triggered();

    // edit menu
    void editSettings_triggered();
    void editTranslations_triggered();
    void editUndo_triggered();
    void editRedo_triggered();
    void editCut_triggered();
    void editCopy_triggered();
    void editPaste_triggered();
    void editSearch_triggered();
    void editGoTo_triggered();
    void editExpandAbbreviation_triggered();
    void editPrepareAPIs_triggered();

    // help menu
    void helpAboutApplication_triggered();
    void helpAboutQt_triggered();
#ifdef __COVERAGESCANNER__
    void helpTestReport_triggered();
#endif

signals:
    // a file has been opened
    void documentOpened( pAbstractChild* document );
    // a file have changed
    void documentChanged( pAbstractChild* document );
    // a file modified state changed
    void documentModifiedChanged( pAbstractChild* document, bool modified );
    // document about to close
    void documentAboutToClose( pAbstractChild* document );
    // a file has been closed
    void documentClosed( pAbstractChild* document );
    // a file has been reloaded
    void documentReloaded( pAbstractChild* document );
    // current file changed
    void currentDocumentChanged( pAbstractChild* document );
};

#endif // PWORKSPACE_H
