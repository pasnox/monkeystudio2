/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : DesignerClient.cpp
** Date : lun aoÃ» 28 18:50:38 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qdesignerclient.h"

#include "qmdiserver.h"

#include <QFile>
#include <QAction>
#include <QString>
#include <QScrollArea>
#include <QHBoxLayout>

#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowManagerInterface>

#if QT_VERSION < 0x040200
class QtUndoStack : public QObject {};
#else
#include <QUndoStack>
#include <QtDesigner>
//#include <QDesignerPropertySheetExtension>
//#include <QDesignerPropertyEditorInterface>
#endif

QDesignerClient::QDesignerClient(QDesignerFormWindowInterface *i,
								const QString& f, QWidget *w)
 : qmdiWidget(w), pInterface(i)
{
	if ( !pInterface )
		qFatal("Can't instantiate a designer client with NULL interface");
	
	QHBoxLayout *l = new QHBoxLayout(this);
	l->setMargin(2);
	l->setSpacing(2);
	l->addWidget(pInterface);
	
	if ( !f.isEmpty() )
	{
		QFile file(f);
		setFileName(f);
		
		pInterface->setFileName(f);
		pInterface->setContents(&file);
		pInterface->setDirty(false);
	}
	
	#if QT_VERSION < 0x040200
	connect(pInterface->commandHistory(), SIGNAL( commandExecuted() ),
			this						, SLOT  ( updateChanged() ) );
	#else
	connect(pInterface->commandHistory(), SIGNAL( indexChanged(int) ),
			this						, SLOT  ( updateChanged() ) );
	
	connect(pInterface	, SIGNAL( geometryChanged() ),
			this		, SLOT  ( geometryChanged() ) );
	#endif
	
	QDesignerFormWindowManagerInterface *m;
	m =	pInterface->core()->formWindowManager();
	
	aUndo = m->actionUndo();
	aUndo->setIcon( QIcon(":/undo.png") );
	aRedo = m->actionRedo();
	aRedo->setIcon( QIcon(":/redo.png") );
	
	aCut = m->actionCut();
	aCopy = m->actionCopy();
	aPaste = m->actionPaste();
	aDelete = m->actionDelete();
	
	aVertical = m->actionVerticalLayout();
	aHorizontal = m->actionHorizontalLayout();
	aGrid = m->actionGridLayout();
	aSplitH = m->actionSplitHorizontal();
	aSplitV = m->actionSplitVertical();
	aBreak = m->actionBreakLayout();
	aAdjust = m->actionAdjustSize();
	
	aLower = m->actionLower();
	aRaise = m->actionRaise();
	
	menus["&Edit"]->addAction(aUndo);
	menus["&Edit"]->addAction(aRedo);
	menus["&Edit"]->addSeparator();
	menus["&Edit"]->addAction(aCut);
	menus["&Edit"]->addAction(aCopy);
	menus["&Edit"]->addAction(aPaste);
	menus["&Edit"]->addAction(aDelete);
	menus["&Edit"]->addSeparator();
	menus["&Edit"]->addAction(aVertical);
	menus["&Edit"]->addAction(aHorizontal);
	menus["&Edit"]->addAction(aGrid);
	menus["&Edit"]->addAction(aSplitV);
	menus["&Edit"]->addAction(aSplitH);
	menus["&Edit"]->addAction(aBreak);
	menus["&Edit"]->addAction(aAdjust);
	
	toolbars["Edit"]->addAction(aUndo);
	toolbars["Edit"]->addAction(aRedo);
	toolbars["Edit"]->addSeparator();
	toolbars["Edit"]->addAction(aCut);
	toolbars["Edit"]->addAction(aCopy);
	toolbars["Edit"]->addAction(aPaste);
	toolbars["Edit"]->addAction(aDelete);
	toolbars["Edit"]->addSeparator();
	toolbars["Edit"]->addAction(aVertical);
	toolbars["Edit"]->addAction(aHorizontal);
	toolbars["Edit"]->addAction(aGrid);
	toolbars["Edit"]->addAction(aSplitV);
	toolbars["Edit"]->addAction(aSplitH);
	toolbars["Edit"]->addAction(aBreak);
	toolbars["Edit"]->addAction(aAdjust);
	
	t.start(1000, this);
}

QDesignerClient::~QDesignerClient()
{
	
}

void QDesignerClient::timerEvent(QTimerEvent *e)
{
	if ( e->timerId() != t.timerId() )
		return;
	
	if ( pInterface->isDirty() != isContentModified() )
		updateChanged();
	
	t.start(1000, this);
}

void QDesignerClient::save()
{
	if ( fileName().isEmpty() )
	{
		if ( server() )
			return server()->saveClientAs(this);
		
		return;
	}
	
	QFile f( fileName() );
	QTextStream out(&f);
	
	if ( !f.open(QFile::WriteOnly | QFile::Text) )
		return;
	
	out << pInterface->contents();
	
	t.stop();
	setContentModified(false);
	pInterface->setDirty(false);
	t.start(1000, this);
}

QDesignerFormWindowInterface* QDesignerClient::interface() const
{
	return pInterface;
}

void QDesignerClient::updateChanged()
{
	setContentModified(pInterface->isDirty());
}

#if QT_VERSION >= 0x040200
void QDesignerClient::geometryChanged()
{
	QDesignerFormEditorInterface *c = pInterface->core();
	
	if( QObject *o = c->propertyEditor()->object() )
	{
		QDesignerPropertySheetExtension *sheet = 
			qt_extension<QDesignerPropertySheetExtension*>(c->extensionManager(), o);
		c->propertyEditor()->setPropertyValue("geometry", sheet->property(sheet->indexOf("geometry")));
	}
}
#endif
