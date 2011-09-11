/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pChild.h
** Date      : 2008-01-14T00:37:20
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
#ifndef PCHILD_H
#define PCHILD_H

#include <MonkeyExport.h>

#include "pAbstractChild.h"

class pEditor;

class Q_MONKEY_EXPORT pChild : public pAbstractChild
{
	Q_OBJECT

public:
	pChild();
	virtual ~pChild();

	virtual QString language() const;
	virtual QString fileBuffer() const;
	virtual QString context() const;
	virtual void initializeContext( QToolBar* tb );
	virtual QPoint cursorPosition() const;
	virtual pEditor* editor() const;
	virtual bool isModified() const;
	virtual bool isUndoAvailable() const;
	virtual void undo();
	virtual bool isRedoAvailable() const;
	virtual void redo();
	virtual void cut();
	virtual void copy();
	virtual void paste();
	virtual void goTo();
	virtual void goTo( const QPoint& position, int selectionLength = -1 );
	virtual bool isCopyAvailable() const;
	virtual bool isPasteAvailable() const;
	virtual bool isGoToAvailable() const;
	virtual bool isPrintAvailable() const;
	virtual void saveFile();
	virtual void backupFileAs( const QString& fileName );
	virtual bool openFile( const QString& fileName, const QString& codec );
	virtual void closeFile();
	virtual void reload();
	virtual void setFileBuffer( const QString& content );
	virtual void printFile();
	virtual void quickPrintFile();

protected:
	pEditor* mEditor;

protected slots:
	void cursorPositionChanged();

public slots:
	void invokeSearch();
};

#endif // PCHILD_H
