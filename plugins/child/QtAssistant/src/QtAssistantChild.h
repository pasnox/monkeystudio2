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
#ifndef QTASSISTANTCHILD_H
#define QTASSISTANTCHILD_H

#include <workspacemanager.h>

class QtAssistantChild : public pAbstractChild
{
	Q_OBJECT
	
public:
	QtAssistantChild( QWidget* parent );
	~QtAssistantChild();

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
	virtual bool openFile( const QString& filename, QTextCodec* codec );
	virtual void closeFile( const QString& filename );
	virtual void closeFiles();
	virtual void printFile( const QString& filename );
	virtual void quickPrintFile( const QString& filename );

/*
signals:
	// emit when a file is opened
	void fileOpened( const QString& ); // ok
	// emit when a file is closed
	void fileClosed( const QString& ); // ok
	// emit when the child layout mode has changed
	void layoutModeChanged( pAbstractChild::LayoutMode );
	// emit when the child document mode has changed
	void documentModeChanged( pAbstractChild::DocumentMode );
	// emit when cursor position changed
	void cursorPositionChanged( const QPoint& ); // ok
	// emit when current file changed
	void currentFileChanged( const QString& );
	// emit when a file is modified
	void modifiedChanged( bool ); // ok
	// emit when undo has changed
	void undoAvailableChanged( bool ); // ok
	// emit when undo has changed
	void redoAvailableChanged( bool ); // ok
	// emit when a file paste available change
	void pasteAvailableChanged( bool ); // ok
	// emit when a file copy available change
	void copyAvailableChanged( bool ); // ok
*/
};

#endif // QTASSISTANTCHILD_H
