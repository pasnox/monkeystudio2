/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Child Plugins
** FileName  : QtAssistantChild.h
** Date      : 2008-01-14T00:57:10
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
#ifndef QTASSISTANTCHILD_H
#define QTASSISTANTCHILD_H

#include "pAbstractChild.h"
#include "QSingleton.h"

#include <QUrl>

class MainWindow;
class HelpWindow;
class QDockWidget;
class pEditor;

class QtAssistantChild : public pAbstractChild, public QSingleton<QtAssistantChild>
{
	Q_OBJECT
	friend class QSingleton<QtAssistantChild>;
	
public:
	~QtAssistantChild();
	QList<HelpWindow*> browsersList() const;
 
	virtual QStringList files() const;
	virtual QPoint cursorPosition() const;
	virtual void showFile( const QString& );
	virtual QString currentFile() const;
	virtual QString currentFileName() const;
	inline virtual pEditor* currentEditor() const { return 0; }
	virtual bool isModified() const;
	virtual bool isUndoAvailable() const;
	virtual bool isRedoAvailable() const;
	virtual bool isPasteAvailable() const;
	virtual bool isCopyAvailable() const;
	virtual bool isModified( const QString& ) const;
	virtual void saveFile( const QString& );
	virtual void saveFiles();
	virtual void printFile( const QString& );
	virtual void quickPrintFile( const QString& );
	virtual void undo();
	virtual void redo();
	virtual void cut();
	virtual void copy();
	virtual void paste();
	virtual void searchReplace();
	virtual void goTo();
	virtual bool isSearchReplaceAvailable() const;
	virtual bool isGoToAvailable() const;
	virtual bool isPrintAvailable() const;
	MainWindow* main();
	QDockWidget* dock();
	
public slots:
	virtual void searchPrevious() {}
	virtual void searchNext() {}
	virtual void goTo( const QString&, const QPoint&, bool = false ) {}
	virtual void backupCurrentFile( const QString& ) {}
	virtual bool openFile( const QString&, QTextCodec* = 0 ) {return false;}
	virtual void closeFile( const QString& ) {}
	virtual void closeFiles() {}
	
private:
	QtAssistantChild( QObject* = 0 );
	
protected:
	MainWindow* mMain;
	QDockWidget* mDock;
	
protected slots:
	void helpWindow_destroyed( QObject* );
	void currentChanged( int );
	void showLink( const QString& = QString() );
	void showLink( const QUrl& );
	
};

#endif // QTASSISTANTCHILD_H
