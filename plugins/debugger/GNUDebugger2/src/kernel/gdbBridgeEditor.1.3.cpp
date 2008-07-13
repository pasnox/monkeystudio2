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

/*
Contructor of GdbBrigdeEditor

GdbBridgeEditor connect signal from Qsci Editor and GNU Debugger plugin
 
*/
GdbBridgeEditor::GdbBridgeEditor(QObject *parent) : QObject(parent)
{
	editorList.clear();
	// get all files opened before debugger plugin is enable
	fileOpenedBeforeDebugger();
}

//

GdbBridgeEditor::~GdbBridgeEditor()
{
	removeAllBreakpoints();
	removeBacktrace();
}

/*
Save the new Qsci editor in <b>editorList</b> variable

When user open a new Qsci editor (code source) this function is call and save the current pointer 
*/
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
			connect (pe.pointeur, SIGNAL(copyAvailable(bool )), this, SLOT(onCopyAvailable(bool )));

			emit requestBreakpoint(pe.fileName);
			emit requestBacktrace(pe.fileName);
		}
	}
}

/*
Signal form Qsci indicate the user select text

When user select text in editor, GNU debugger plugin can show a value of variable 
*/

void GdbBridgeEditor::onCopyAvailable(bool b)
{
	pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
	if(e && b)
		emit requestShowVar(e->selectedText());
}

/*
Remove pointer saved in <b>editorList</b> variable

When user close Qsci editor (code source) this function is call and remove the current pointer 
*/
void GdbBridgeEditor::removeEditor(const int & i)
{
	// remove editor of list
	if(i < editorList.count())
		editorList.removeAt(i);
}

/*
Remove all breakpoints icon in Qsci editor
*/

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

/*
Remove all icons in Qsci editor <b>e</b> present in <b>line</b>
*/
void GdbBridgeEditor::removeAllBreakpointsAt( pEditor *e , const int & line)
{
		e->markerDelete(line, pEditor::mdEnabledBreak);	
		e->markerDelete(line, pEditor::mdDisabledBreak);	
		e->markerDelete(line, pEditor::mdEnabledConditionalBreak);	
		e->markerDelete(line, pEditor::mdDisabledConditionalBreak);	
}

/*
Remove BackTrace icon in current editor 
*/

void GdbBridgeEditor::removeBacktrace()
{
	foreach(Editor e , editorList)
	{
		e.pointeur->markerDeleteAll( pEditor::mdPlay);	
	}
}

/*
Same as Add() function but save Qsci editor pointer if is opened before GNU debugger is started
*/

void GdbBridgeEditor::fileOpenedBeforeDebugger()
{
	//find if editor is open before load plugin
	if(MonkeyCore::workspace())
	{
		QList<pAbstractChild * >  e = MonkeyCore::workspace()->children();
		foreach(pAbstractChild * pf, e)//for(int i =0; i< e.count(); i++)
		{
			// fix v 1.3.2 if last file opened is a Designer Ui
			if( qobject_cast<pEditor*>( pf->currentEditor() ))
			{
				Editor pe = {pf->currentFile() , pf->currentEditor()}; 
				editorList << pe;

				// set margin Qsci sensitive
				pe.pointeur->setMarginSensitivity(0,true);
				// connect margin clicked
				connect (pe.pointeur , SIGNAL(marginClicked (int, int , Qt::KeyboardModifiers )), this, SLOT(onMarginClicked(int, int,  Qt::KeyboardModifiers)));
				connect (pe.pointeur, SIGNAL(copyAvailable(bool )), this, SLOT(onCopyAvailable(bool )));
			}
		}
	}
}

/*
User click in Qsci margin
*/

void GdbBridgeEditor::onMarginClicked( int margeIndex, int line, Qt::KeyboardModifiers d)
{
	// get the name of the current file
	if(MonkeyCore::fileManager())
	{
		QString fileName = MonkeyCore::fileManager()->currentChildFile();
		emit userToggleBreakpoint(fileName, line );
	}
}

/*
Toggle breakpoint icon 
*/

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

/*
Toggle backtrace icon
*/

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

/**
Find file in <b>editorList</b> variable
*/
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

