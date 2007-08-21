#ifndef PEDITOR_H
#define PEDITOR_H

#include "qsciscintilla.h"

#include "MonkeyExport.h"

class Q_MONKEY_EXPORT pEditor : public QsciScintilla
{
	Q_OBJECT

protected:
	bool mCopyAvailable;
	static bool mPasteAvailableInit;
	static bool mPasteAvailable;
	QPoint mCursorPosition;

public:
	pEditor( QWidget* = 0 );
	virtual ~pEditor();

	bool lineNumbersMarginEnabled() const;
	int lineNumbersMarginWidth() const;
	bool lineNumbersMarginAutoWidth() const;
	bool copyAvailable();
	bool canPaste();
	QPoint cursorPosition() const;

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
