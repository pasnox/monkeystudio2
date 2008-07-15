/****************************************************************************
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
****************************************************************************/
/*!
	\file gdbBridgeEditor.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Implements all functions for remote Qsci.
*/

#include "gdbBridgeEditor.h"

#include <QMessageBox>

/*!
	\details Create new object
	Get if editor are opened before that Debugger pluggin is started.
	\param parent of this object
*/
GdbBridgeEditor::GdbBridgeEditor(QObject *parent) : QObject(parent)
{
	editorList.clear();
	// get all files opened before debugger plugin is enable
	fileOpenedBeforeDebugger();
}

//
/*!
	\details Remove all breakpoints and backtrace icon in editor
*/
GdbBridgeEditor::~GdbBridgeEditor()
{
	removeAllBreakpoints();
	removeBacktrace();
}

/*!
	\details New file is opened.
	When a new editor (source code) is opened this function is call, 
	it store fileName and pointer in Editor struct. 
	Set sensitivity the margin and request if this file has breakpoint or backtrace 
	before the last closed.
	\param s is de name of editor
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

	
/*!
	\details User has selected string in editor
	If user select string in editor, this string is possible a var, Gdb can print the value of this.
	GdbToolTip class can show this value in the tooltip.
	\param b indicate the new text is selected in editor, false if the text is not unselected.
*/
void GdbBridgeEditor::onCopyAvailable(bool b)
{
	pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
	if(e && b)
		emit requestShowVar(e->selectedText());
}

/*!
	\details Remove Qsci pointer to the list, when this editor is closed by a user
	\param i is the index in this list
*/
void GdbBridgeEditor::removeEditor(const int & i)
{
	// remove editor of list
	if(i < editorList.count())
		editorList.removeAt(i);
}

/*!
	\details Remove all icons (breakpoint) in QSci margin and for all editors opened
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

/*!
	\details  Remove all icons (breakpoints) at line for only one editor
	\param e is the pointer to the editor
	\param line is the line number
*/
void GdbBridgeEditor::removeAllBreakpointsAt( pEditor *e , const int & line)
{
	e->markerDelete(line, pEditor::mdEnabledBreak);	
	e->markerDelete(line, pEditor::mdDisabledBreak);	
	e->markerDelete(line, pEditor::mdEnabledConditionalBreak);	
	e->markerDelete(line, pEditor::mdDisabledConditionalBreak);	
}

/*!
	\details Remove all icons (backtrace) in Qsci margin for all editor opened
*/
void GdbBridgeEditor::removeBacktrace()
{
	foreach(Editor e , editorList)
	{
		e.pointeur->markerDeleteAll( pEditor::mdPlay);	
	}
}

/*!
	\details Same as add() function, but call when an editor is opened before GNU plugin is started
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

/*!
	\details User has clicked in the margin.
	When user click in the margin, this function emit userToggleBreakpoint signal.
	GdbBreakpoint class answer and call onToggleBreakpoint function via DockGNUDebugger class.
	\param margeIndex is the margin index
	\param line is the line number for toggle breakpoint
	\param d is Qt::KeyboardModifiers
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

/*!
	\details Toggle breakpoint icon in margin
	\param bp is Breakpoint struct 
	\param p is BaseBreakpoint struct
	\param b indicate if breakpoint is add or deleted
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

/*!
	\details Toggle backtrace icon in margin.
	Move backtrace in editor where Gdb has breaked
	\param fileName is the name of this file 
	\param line is the line number
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

/*!
	\details find file by name
	\param file is the name of file
	\retval A pointer to the editor or NULL if not found.
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

