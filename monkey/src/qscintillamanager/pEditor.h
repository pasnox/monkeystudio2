/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pEditor.h
** Date      : 2008-01-14T00:37:06
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
#ifndef PEDITOR_H
#define PEDITOR_H

#include <qscintilla.h>
#include <fresh.h>

class Q_MONKEY_EXPORT pEditor : public QsciScintilla
{
	Q_OBJECT

public:
	pEditor( QWidget* = 0 );
	virtual ~pEditor();

	enum RegisterImageType { riClass = 0, riEnum, riFunction, riMember, riNamespace, riStruct, riTypedef, riVariable };

	bool lineNumbersMarginEnabled() const;
	int lineNumbersMarginWidth() const;
	bool lineNumbersMarginAutoWidth() const;
	bool copyAvailable();
	bool canPaste();
	QPoint cursorPosition() const;

protected:
	void keyPressEvent( QKeyEvent* );

	bool mCopyAvailable;
	static bool mPasteAvailableInit;
	static bool mPasteAvailable;
	QPoint mCursorPosition;

protected slots:
	void linesChanged();
	void setCopyAvailable( bool );
	void cursorPositionChanged( int, int );
	void textChanged();
	void clipboardDataChanged();

public slots:
	void setLineNumbersMarginEnabled( bool );
	void setLineNumbersMarginWidth( int );
	void setLineNumbersMarginAutoWidth( bool );
	bool openFile( const QString& );
	bool saveFile( const QString& = QString() );
	bool saveBackup( const QString& );
	void closeFile();
	void print( bool = false );
	void quickPrint();
	void selectNone();
	void invokeGoToLine();
	void convertTabs();
	void makeBackup();

signals:
	void cursorPositionChanged( const QPoint& );
	void undoAvailable( bool );
	void redoAvailable( bool );
	void pasteAvailable( bool );

};

#endif // PEDITOR_H
