/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QtAssistantChild.h
** Date      : 2007-11-04T22:53:00
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
	virtual void openFile( const QString&, const QPoint& = QPoint(), QTextCodec* = 0 ) {}
	virtual void closeFile( const QString& ) {}
	virtual void closeFiles() {}
	
private:
	QtAssistantChild();
	
protected:
	MainWindow* mMain;
	QDockWidget* mDock;

	virtual void closeEvent( QCloseEvent* );
	
protected slots:
	void helpWindow_destroyed( QObject* );
	void currentChanged( int );
	void showLink( const QString& = QString() );
	void showLink( const QUrl& );
	
};

#endif // QTASSISTANTCHILD_H
