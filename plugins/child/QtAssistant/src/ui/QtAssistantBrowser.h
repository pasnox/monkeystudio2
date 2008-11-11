/****************************************************************************
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
****************************************************************************/
#ifndef QTASSISTANTBROWSER_H
#define QTASSISTANTBROWSER_H

#include "ui_QtAssistantBrowser.h"

class QHelpEngine;
class HelpViewer;

class QtAssistantBrowser : public pAbstractChild, public Ui::QtAssistantBrowser
{
	Q_OBJECT

public:
	QtAssistantBrowser( QHelpEngine* engine, QWidget* parent = 0 );
	virtual ~QtAssistantBrowser();

	virtual QPoint cursorPosition() const;
	virtual QString currentFile() const;
	virtual QString currentFileName() const;
	virtual pEditor* currentEditor() const;
	virtual bool isModified() const;
	virtual bool isUndoAvailable() const;
	virtual bool isRedoAvailable() const;
	virtual bool isCopyAvailable() const;
	virtual bool isPasteAvailable() const;
	virtual bool isGoToAvailable() const;
	virtual bool isModified( const QString& filename ) const;
	virtual bool isPrintAvailable() const;

public slots:
	virtual void showFile( const QString& filename );
	virtual void undo();
	virtual void redo();
	virtual void cut();
	virtual void copy();
	virtual void paste();
	virtual void goTo();
	virtual void goTo( const QString& filename, const QPoint& pos, bool highlight = false );
	virtual void invokeSearch();
	virtual void saveFile( const QString& filename );
	virtual void backupCurrentFile( const QString& filename );
	virtual void saveFiles();
	virtual bool openFile( const QString& filename, const QString& codec );
	virtual void closeFile( const QString& filename );
	virtual void closeFiles();
	virtual void printFile( const QString& filename );
	virtual void quickPrintFile( const QString& filename );

	HelpViewer* newEmptyTab( qreal zoom = 1.0 );
	HelpViewer* currentTab( bool create );
	void cloneTab();
	void closeTab();
	
	void openUrl( const QUrl& url );
	void openInNewTabUrl( const QUrl& url );
	
	void searchZoomIn();
	void searchZoomOut();
	void searchResetZoom();
	
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
	
	void restoreLastShownPages();

protected:
	QHelpEngine* mHelpEngine;
	int mLastHelpView;
	int mSearchZoomCount;
	QToolButton* tbCloneTab;
	QToolButton* tbCloseTab;
	
	void hideEvent( QHideEvent* e );
	bool eventFilter( QObject* obj, QEvent* e );
	void find( QString ttf, bool forward, bool backward );

protected slots:
	void updateActions();
	void on_twPages_currentChanged( int index );
	void helpViewer_sourceChanged( const QUrl& url );
	void helpViewer_actionsChanged();
	void searchingStarted();
	void searchingFinished( int hits );
	void search() const;
	void search_customContextMenuRequested( const QPoint& pos );

signals:
	void showBrowserRequested();
};

#endif // QTASSISTANTBROWSER_H
