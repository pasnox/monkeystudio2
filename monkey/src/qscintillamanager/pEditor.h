/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pEditor.h
** Date      : 2007-11-04T22:46:05
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef PEDITOR_H
#define PEDITOR_H

#include "qsciscintilla.h"

#include "MonkeyExport.h"

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
	void invokeSearchReplace();
	void invokeGoToLine();
	void convertTabs( int = -1 );
	void makeBackup();

signals:
	void cursorPositionChanged( const QPoint& );
	void undoAvailable( bool );
	void redoAvailable( bool );
	void pasteAvailable( bool );

};

#endif // PEDITOR_H
