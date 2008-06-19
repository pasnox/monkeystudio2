/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbBridgeEditor
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

#include "gdbBridgeEditor.1.3.h"

#include <QMessageBox>


GdbBridgeEditor::GdbBridgeEditor(QObject *parent) : QObject(parent)
{
	editorList.clear();
	// get all files opened before debugger plugin is enable
	fileOpenedBeforeDebugger();
}

GdbBridgeEditor::~GdbBridgeEditor()
{
	removeAllBreakpoints();
	removeBacktrace();
}

// new file is opened, add this on list
void GdbBridgeEditor::addEditor(const QString & s)
{
	if(MonkeyCore::fileManager() && MonkeyCore::fileManager()->currentChild())
	{
		// get new file opened
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e)
		{
			// get name of file
			QString name = MonkeyCore::fileManager()->currentChildFile();
			// save opened editor
			Editor pe={name, e};
			editorList << pe;
			
			// set margin Qsci sensitive
			pe.pointeur->setMarginSensitivity(0,true);
			// connect margin clicked
			connect (pe.pointeur, SIGNAL(marginClicked (int, int , Qt::KeyboardModifiers )), this, SLOT(onMarginClicked(int, int,  Qt::KeyboardModifiers)));

			emit requestBreakpoint(pe.fileName);
			emit requestBacktrace(pe.fileName);
		}
	}
}

// file closed , remove this of list
void GdbBridgeEditor::removeEditor(const int & i)
{
	// remove editor of list
	if(i < editorList.count())
		editorList.removeAt(i);
}

void GdbBridgeEditor::removeAllBreakpoints()
{
	foreach(Editor e , editorList)
	{
		// fix v1.3.2
		e.pointeur->markerDeleteAll( pEditor::mdEnabledBreak);	
		e.pointeur->markerDeleteAll( pEditor::mdDisabledBreak);	
		e.pointeur->markerDeleteAll( pEditor::mdEnabledConditionalBreak);	
		e.pointeur->markerDeleteAll( pEditor::mdDisabledConditionalBreak);	
	}
}

//

void GdbBridgeEditor::removeAllBreakpointsAt( pEditor *e , const int & line)
{
		e->markerDelete(line, pEditor::mdEnabledBreak);	
		e->markerDelete(line, pEditor::mdDisabledBreak);	
		e->markerDelete(line, pEditor::mdEnabledConditionalBreak);	
		e->markerDelete(line, pEditor::mdDisabledConditionalBreak);	
}

//

void GdbBridgeEditor::removeBacktrace()
{
	foreach(Editor e , editorList)
	{
		e.pointeur->markerDeleteAll( pEditor::mdPlay);	
	}
}

//

void GdbBridgeEditor::fileOpenedBeforeDebugger()
{
	//find if editor is open before load plugin
	if(MonkeyCore::workspace())
	{
		QList<pAbstractChild * >  e = MonkeyCore::workspace()->children();
		foreach(pAbstractChild * pf, e)//for(int i =0; i< e.count(); i++)
		{
//			pAbstractChild  * pf = e.at(i);
			// fix v 1.3.2 if last file opened is a Designer Ui
			if( qobject_cast<pEditor*>( pf->currentEditor() ))
			{
				Editor pe = {pf->currentFile() , pf->currentEditor()}; 
				editorList << pe;

				// set margin Qsci sensitive
				pe.pointeur->setMarginSensitivity(0,true);
				// connect margin clicked
				connect (pe.pointeur , SIGNAL(marginClicked (int, int , Qt::KeyboardModifiers )), this, SLOT(onMarginClicked(int, int,  Qt::KeyboardModifiers)));
			}
		}
	}
}

// User click under margin

void GdbBridgeEditor::onMarginClicked( int margeIndex, int line, Qt::KeyboardModifiers d)
{
	// get the name of the current file
	if(MonkeyCore::fileManager())
	{
		QString fileName = MonkeyCore::fileManager()->currentChildFile();
		emit userToggleBreakpoint(fileName, line );
	}
}

// Toggle breakpoint

void GdbBridgeEditor::onToggleBreakpoint(const Breakpoint & bp, const BaseBreakpoint & p, const bool & b)
{
	pEditor * e = findFile(bp.fileName);
	if(e)
	{
		removeAllBreakpointsAt(e, p.line - 1);

		if(b)
		{
			switch(p.type)
			{
			case 1 : // normal break
				p.enable ? e->markerAdd (p.line - 1, pEditor::mdEnabledBreak) : e->markerAdd (p.line - 1, pEditor::mdDisabledBreak);	
				break;
			case 2 : // conditionned
				p.enable ? e->markerAdd (p.line - 1, pEditor::mdEnabledConditionalBreak) : e->markerAdd (p.line - 1, pEditor::mdDisabledConditionalBreak);	
			}
		}
	}
}

//

void GdbBridgeEditor::onToggleBacktrace(const QString & fileName, const int & line)
{
	// remove back trace in all editor
	removeBacktrace();

	// open file (if is not same)
	if(MonkeyCore::workspace())
		MonkeyCore::workspace()->goToLine(fileName, QPoint(1,line), true);

	// now the current file is , bug fix if no file is open
	if(MonkeyCore::fileManager() && MonkeyCore::fileManager()->currentChild())
	{
		pEditor * e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e) e->markerAdd (line - 1, pEditor::mdPlay);
	}

}

// find file in editorList
pEditor * GdbBridgeEditor::findFile(const QString & file)
{
	/*
		QFileInfo requier :
		editor->fileName =  C:/.../../src/main.cpp
		file = scr/main.cpp
	*/

	for(int i=0; i< editorList.count(); i++)
	{
		QString fileName = editorList.at(i).fileName;
		if(QFileInfo(fileName).fileName() == QFileInfo(file).fileName()) return editorList.at(i).pointeur;
	}
	return NULL;
}

//

