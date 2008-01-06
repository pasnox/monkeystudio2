#ifndef PSEARCH_H
#define PSEARCH_H

#include "MonkeyExport.h"
#include "ui_pSearch.h"

#include <QPointer>

class QsciScintilla;

class Q_MONKEY_EXPORT pSearch : public pDockWidget, public Ui::pSearch
{
	Q_OBJECT
	friend class MonkeyCore;

public:
	QsciScintilla* editor() const;
	void setEditor( QsciScintilla* );

public slots:
	bool on_tbPrevious_clicked();
	bool on_tbNext_clicked();
	bool on_tbReplace_clicked();
	void on_tbReplaceAll_clicked();

protected:
	pSearch( QsciScintilla* = 0 );
	bool checkEditor();
	void keyPressEvent( QKeyEvent* );

	QPointer<QsciScintilla> mEditor;

};

#endif // PSEARCH_H
