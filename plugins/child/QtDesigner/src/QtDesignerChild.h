/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QtDesignerChild.h
** Date      : 2007-11-04T22:53:23
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QTDESIGNERCHILD_H
#define QTDESIGNERCHILD_H

#include "pAbstractChild.h"
#include "QSingleton.h"

class QToolBar;
class QMdiArea;
class QMdiSubWindow;
class pEditor;

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;
class QDesignerWidgetBox;
class QDesignerActionEditor;
class QDesignerPropertyEditor;
class QDesignerObjectInspector;
class QDesignerSignalSlotEditor;

class QtDesignerChild : public pAbstractChild, public QSingleton<QtDesignerChild>
{
	Q_OBJECT
	friend class QSingleton<QtDesignerChild>;
	
protected:
	QAction* aPreview;
	QDesignerFormEditorInterface* mCore;
	QDesignerWidgetBox* pWidgetBox;
	QDesignerActionEditor* pActionEditor;
	QDesignerPropertyEditor* pPropertyEditor;
	QDesignerObjectInspector* pObjectInspector;
	QDesignerSignalSlotEditor* pSignalSlotEditor;
	
	QDesignerFormWindowInterface* createForm();
	bool eventFilter( QObject*, QEvent* );

public:
	QDesignerFormEditorInterface* core();

protected slots:
	void subWindowActivated( QMdiSubWindow* );
	void activeFormWindowChanged( QDesignerFormWindowInterface* w );
	void geometryChanged();
	void formChanged();
	void editWidgets();
	void previewCurrentForm();
	void setModified( QDesignerFormWindowInterface* );
	
public:
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

private:
	QtDesignerChild();
	~QtDesignerChild();

	QToolBar* mToolBar;
	QMdiArea* mArea;
	
public slots:
	virtual void searchPrevious() {}
	virtual void searchNext() {}
	virtual void goTo( const QString&, const QPoint&, bool = false );
	virtual void backupCurrentFile( const QString& );
	virtual void openFile( const QString&, const QPoint& = QPoint(), QTextCodec* = 0 );
	virtual void closeFile( const QString& );
	virtual void closeFiles();
	
};

#endif // QTDESIGNERCHILD_H
