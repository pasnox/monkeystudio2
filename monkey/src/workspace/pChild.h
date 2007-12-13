/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : pChild.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef PCHILD_H
#define PCHILD_H

#include "MonkeyExport.h"
#include "pAbstractChild.h"

class pEditor;

class pChild : public pAbstractChild
{
	Q_OBJECT

protected:
	pEditor* mEditor;

public:
	pChild();
	virtual ~pChild();

	// return child language
	virtual QString language() const;

	// return cursor position if available
	virtual QPoint cursorPosition() const;

	// show/focus the file in child
	virtual void showFile( const QString& );

	// the current visible / focused file
	virtual QString currentFile() const;

	// the current visible / focused file name ( without path )
	virtual QString currentFileName() const;

	// the current visible editor
	virtual pEditor* currentEditor() const;

	// return the current file modified flag
	virtual bool isModified() const;

	// return the current file undo flag
	virtual bool isUndoAvailable() const;

	// undo
	virtual void undo();

	// return the current file redo flag
	virtual bool isRedoAvailable() const;

	// redo
	virtual void redo();

	// cut
	virtual void cut();

	// copy
	virtual void copy();

	// paste
	virtual void paste();

	// search/replace in the current child
	virtual void searchReplace();

	// search previous
	virtual void searchPrevious();

	// search next
	virtual void searchNext();

	// go to in the current child
	virtual void goTo();

	// go to position for file and highlight line according to bool
	virtual void goTo( const QString&, const QPoint&, bool = false );

	// return the current file copy available
	virtual bool isCopyAvailable() const;

	// return the current file paste available
	virtual bool isPasteAvailable() const;

	// return is search/replace is available
	virtual bool isSearchReplaceAvailable() const;

	// return is goto is available
	virtual bool isGoToAvailable() const;

	// return the modified state of file
	virtual bool isModified( const QString& ) const;

	// return if print is available
	virtual bool isPrintAvailable() const;

	// ask to save file
	virtual void saveFile( const QString& );

	// ask to save all files
	virtual void saveFiles();
	
	// ask to backup the current file
	void backupCurrentFile( const QString& );

	// ask to load file
	virtual void openFile( const QString&, QTextCodec* = 0 );

	// ask to close file
	virtual void closeFile( const QString& );

	// ask to close all files
	virtual void closeFiles();

	// ask to print this file
	virtual void printFile( const QString& );

	// ask to quick print this file
	virtual void quickPrintFile( const QString& );

protected slots:
	void cursorPositionChanged();

};

#endif // PCHILD_H
