/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pSearch.h
** Date      : 2007-11-04T22:46:02
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef PSEARCH_H
#define PSEARCH_H

#include "MonkeyExport.h"
#include "ui_pSearch.h"
#include "QSingleton.h"

#include <QPointer>

class QsciScintilla;

class Q_MONKEY_EXPORT pSearch : public QDockWidget, public Ui::pSearch, public QSingleton<pSearch>
{
	Q_OBJECT
	friend class QSingleton<pSearch>;

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
