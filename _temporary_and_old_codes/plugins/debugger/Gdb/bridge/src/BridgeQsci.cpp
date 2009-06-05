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
	\file BridgeQsci.cpp
	\date 15/05/09
	\author Xiantia
	\version 1.4.0
	\brief Implements all functions for remote Qsci.
*/

#include "BridgeQsci.h"

#include <pFileManager.h>
#include <pAbstractChild.h>

#include <QMessageBox>
#include <QtDebug>


/*!
	\details Create new object.
	Get if editor are opened before that GNU Debugger plugin is started and connect some slots
	\param parent of this object
*/
BridgeQsci::BridgeQsci(QObject *parent) : QObject(parent)
{
	editorList.clear();

	connect( MonkeyCore::workspace(), SIGNAL( fileOpened( const QString & ) ), this, SLOT( addEditor( const QString & ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( documentAboutToClose( int ) ), this, SLOT( deleteEditorFromList( int ) ));

	/* breakpoint */
	// when user click on margin
	connect(this, SIGNAL(userToggleBreakpoint(const QString &, const int &)), GdbCore::KernelDispatcher()->findAddon("GdbBreakpoint"), 
		SLOT(onToggleBreakpoint(const QString &, const int &)));
	// when the breakpoint is add or delete
//	connect(GdbCore::KernelDispatcher()->findAddon("GdbBreakpoint"), SIGNAL(toggleBreakpoint(const Breakpoint &, const SingleBreakpoint &, const bool &)), this,
//		SLOT(onToggleBreakpoint(const Breakpoint &, const SingleBreakpoint &, const bool& )));

	connect(GdbCore::KernelDispatcher()->findAddon("GdbBreakpoint"), SIGNAL(toggleBreakpoint(const QHash<QString,QString> &, const bool &)), this,
		SLOT(onToggleBreakpoint(const QHash<QString,QString> &, const bool &)));


	// when editor is re-open (breakpoint)
	connect(this, SIGNAL(requestBreakpoint(QString)), GdbCore::KernelDispatcher()->findAddon("GdbBreakpoint"),
		SLOT(onRequestBreakpoint(QString)));
	// delete breakpoint -> quit debugger mode
	connect(GdbCore::KernelDispatcher()->findAddon("GdbBreakpoint"), SIGNAL(removeAllBreakpoints()), this,
		SLOT(breakpointDeleteAll()));
	// when user double click on breakpoint Ui
	connect(GdbCore::KernelDispatcher()->findAddon("GdbBreakpoint"), SIGNAL(gotoBreakpoint(QString , int)), this,
		SLOT(onGotoBreakpoint(QString, int)));

	/* backtrace */
	// when editor is re-open (backtrace)
	connect(this, SIGNAL(requestBacktrace(QString)), GdbCore::KernelDispatcher()->findAddon("GdbBacktrace"),
		SLOT(onRequestBacktrace(QString)));
	// move bactrace
	connect(GdbCore::KernelDispatcher()->findAddon("GdbBacktrace"), SIGNAL(toggleBacktrace(const QString &, const int &)), this,
		SLOT(onToggleBacktrace(const QString &, const int &) ));
	// delete backtrace -> quit debugger mode
	connect(GdbCore::KernelDispatcher()->findAddon("GdbBacktrace"), SIGNAL(removeBacktrace()), this,
		SLOT(backtraceDeleteAll() ));
	// when user double click on backtrace Ui 
	connect(GdbCore::KernelDispatcher()->findAddon("GdbBacktrace"), SIGNAL(gotoBacktrace(QString, int)), this,
		SLOT(onGotoBacktrace(QString, int) ));

	/* tooltip */
	// when user select variable
	connect(this, SIGNAL(requestShowVar(const QString &)), GdbCore::KernelDispatcher()->findAddon("GdbToolTip") , 
		SLOT(onRequestShowVar(const QString &)));

	// get all files opened before debugger plugin is enable
	// and set context
	fileOpenedBeforeDebugger();
}

//
/*!
	\details Remove all breakpoints and backtrace icon in editor
*/
BridgeQsci::~BridgeQsci()
{
	breakpointDeleteAll(); // delete all break icon group on any opened editor
	backtraceDeleteAll(); // delete all back icon group on any opend editor
	editorList.clear(); // clear current pointer
}

/*!
	\details Create the same icon but with the alpha channel.
	Same as QPixmap().SetEnabled(false)
	\param p is the pixmap to disable.
*/
QPixmap BridgeQsci::setDisable(QPixmap p)
{
  QBitmap b(QSize(16,16));
  b.fill(QColor(0,0,0,120));
  p.setMask(b);
  return p;
}

/*!
	\details Registers icons under Qsci
	When a new editor (source code) is opened this function is call, 
	\param e is the pointer of editor
*/
void BridgeQsci::setIconContext(pEditor *e)
{
	// icons group Breakpoint
	mIconList["igBreakpoint"]["miNormalEnable"] = e->markerDefine( QPixmap( ":/icons/normalbreakpoint.png" ).scaled(16,16));
	mIconList["igBreakpoint"]["miNormalDisable"] = e->markerDefine( setDisable(QPixmap( ":/icons/normalbreakpoint.png" ).scaled(16,16)));
	mIconList["igBreakpoint"]["miConditionEnable"] = e->markerDefine( QPixmap( ":/icons/conditionbreakpoint.png" ).scaled(16,16));
	mIconList["igBreakpoint"]["miConditionDisable"] = e->markerDefine( setDisable(QPixmap( ":/icons/conditionbreakpoint.png" ).scaled(16,16)));

	mIconList["igBreakpoint"]["miSablier"] = e->markerDefine( QPixmap( ":/icons/sablier.png" ).scaled(16,16));

	// icons group Backtrace
	mIconList["igBacktrace"]["miBacktrace"] = e->markerDefine( QPixmap( ":/icons/play.png").scaled(16,16));
}


/*!
	\details Set Margin of Qsci sensitivity
	When a new editor (source code) is opened this function is call, 
	\param e is the pointer of editor
*/
void BridgeQsci::setMarginContext(pEditor *e)
{
	// set margin Qsci sensitive (3 colonnes ??)
	e->setMarginSensitivity(0,true);
	e->setMarginSensitivity(1,true);
	e->setMarginSensitivity(2,true);
}


/*!
	\details New file is opened.
	When a new editor (source code) is opened this function is call, 
	it store fileName and pointer in Editor struct. 
	Set sensitivity the margin and request if this file has breakpoint or backtrace 
	before the last closed.
	\param fileName is de name of editor
*/
void BridgeQsci::addEditor(const QString & fileName)
{
	Q_UNUSED(fileName);
	
	if(MonkeyCore::fileManager() && MonkeyCore::fileManager()->currentChild())
	{
		// get new file opened
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e)
		{
			setIconContext(e);
			setMarginContext(e);

			// get name of file
			QString name = MonkeyCore::fileManager()->currentChildFile();
			// save opened editor
			Editor pe={name, e};
			editorList << pe;
			
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
	\param b indicate the new text is selected in editor, false if the text is now unselected.
*/
void BridgeQsci::onCopyAvailable(bool b)
{
	pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
	if(e && b)
		emit requestShowVar(e->selectedText());
}

/*!
	\details Remove Qsci pointer to the list, when this editor is closed by a user
	\param index is the index in this list
*/
void BridgeQsci::deleteEditorFromList(const int & index)
{
	// remove editor of list
	if(index < editorList.count())
		editorList.removeAt(index);
}

/*!
	\details Remove all icons (breakpoint and pending) in QSci margin and for all editors opened
*/
void BridgeQsci::breakpointDeleteAll()
{
	foreach(Editor e , editorList)
	{
		foreach(int i, mIconList["igBreakpoint"]) e.pointeur->markerDeleteAll(i);
	}
}

/*!
	\details  Remove all icons (breakpoints and pending) at line for only one editor
	\param e is the pointer to the editor
	\param line is the line number
*/
void BridgeQsci::breakpointDeleteAtLine( pEditor *e , const int & line)
{
	foreach(int i, mIconList["igBreakpoint"]) e->markerDelete(line, i);
}

/*!
	\details Remove all icons (backtrace) in Qsci margin for all editor opened
*/
void BridgeQsci::backtraceDeleteAll()
{
	foreach(Editor e , editorList)	e.pointeur->markerDeleteAll( mIconList["igBacktrace"]["miBacktrace"]);	
}

/*!
	\details Same as add() function, but call when an editor is opened before GNU Debugger plugin is started.
*/
void BridgeQsci::fileOpenedBeforeDebugger()
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
			
				setIconContext(pe.pointeur);
				setMarginContext(pe.pointeur);

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
	\param margeIndex is the margin index, (no use)
	\param line is the line number for toggle breakpoint
	\param d is Qt::KeyboardModifiers, (no use)
*/
void BridgeQsci::onMarginClicked( int margeIndex, int line, Qt::KeyboardModifiers d)
{
	Q_UNUSED(margeIndex);
	Q_UNUSED(d);

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
/*void BridgeQsci::onToggleBreakpoint(const Breakpoint & bp, const SingleBreakpoint & p, const bool & b)
{

	pEditor * e = findFile(bp.fileName);
	if(e)
	{
		breakpointDeleteAtLine(e, p.line - 1);

		if(b)
		{
			switch(p.type)
			{
			case 1 : // normal break 1.4.0
//				p.enable ? e->markerAdd (p.line - 1, pEditor::mdEnabledBreak) : e->markerAdd (p.line - 1, pEditor::mdDisabledBreak);	
				p.enable ? e->markerAdd (p.line - 1, mIconList["igBreakpoint"]["miNormalEnable"]) : e->markerAdd (p.line - 1, mIconList["igBreakpoint"]["miNormalDisable"]);	
				break;
			case 2 : // conditionned 1.4.0
//				p.enable ? e->markerAdd (p.line - 1, pEditor::mdEnabledConditionalBreak) : e->markerAdd (p.line - 1, pEditor::mdDisabledConditionalBreak);
				p.enable ? e->markerAdd (p.line - 1, mIconList["igBreakpoint"]["miConditionEnable"]) : e->markerAdd (p.line - 1, mIconList["igBreakpoint"]["miConditionDisable"]);
				break;
			}
			if(p.pending) e->markerAdd (p.line -1 , mIconList["igBreakpoint"]["miSablier"]);
		}
	}
}*/

void BridgeQsci::onToggleBreakpoint(const QHash<QString, QString> & hash, const bool & b)
{

	pEditor * e = findFile(hash["fileName"]);
	if(e)
	{
		breakpointDeleteAtLine(e, hash["line"].toInt() - 1);

		if(b)
		{
			if(hash["type"] == "normalBreak")
				hash["enable"].toInt() ? e->markerAdd (hash["line"].toInt() - 1, mIconList["igBreakpoint"]["miNormalEnable"]) : e->markerAdd (hash["line"].toInt() - 1, mIconList["igBreakpoint"]["miNormalDisable"]);	

			if(hash["type"] == "conditionnedBreak")
				hash["enable"].toInt() ? e->markerAdd (hash["line"].toInt() - 1, mIconList["igBreakpoint"]["miConditionEnable"]) : e->markerAdd (hash["line"].toInt() - 1, mIconList["igBreakpoint"]["miConditionDisable"]);

			if(hash["pending"].toInt())  e->markerAdd (hash["line"].toInt() -1 , mIconList["igBreakpoint"]["miSablier"]);
		}
	}
}
/*!
	\details Toggle backtrace icon in margin.
	Move backtrace in editor where Gdb has breaked.
	\param fileName is the name of this file 
	\param line is the line number
*/
void BridgeQsci::onToggleBacktrace(const QString & fileName, const int & line)
{
	// remove back trace in all editor
	backtraceDeleteAll();

	// open file (if is not same)
	if(MonkeyCore::workspace())
		MonkeyCore::workspace()->goToLine(fileName, QPoint(1,line), true, pMonkeyStudio::defaultCodec());

	// now the current file is , bug fix if no file is open
	if(MonkeyCore::fileManager() && MonkeyCore::fileManager()->currentChild())
	{
		pEditor * e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e) e->markerAdd (line - 1, mIconList["igBacktrace"]["miBacktrace"]);
	}

}

/*!
	\details find file by name
	\param file is the name of file
	\retval A pointer to the editor or NULL if not found.
*/
pEditor * BridgeQsci::findFile(const QString & file)
{
	for(int i=0; i< editorList.count(); i++)
	{
		QString fileName = editorList.at(i).fileName;
//		if(QFileInfo(fileName).fileName() == QFileInfo(file).fileName()) 
		if(fileName == file) // ok fixed 1.4.0
			return editorList.at(i).pointeur;
	}
	return NULL;
}

//

void BridgeQsci::onGotoBreakpoint(QString fileName, int line)
{
	if(MonkeyCore::workspace())
		MonkeyCore::workspace()->goToLine(fileName, QPoint(1,line), true, pMonkeyStudio::defaultCodec());
}

void BridgeQsci::onGotoBacktrace(QString fileName, int line)
{
	if(MonkeyCore::workspace())
		MonkeyCore::workspace()->goToLine(fileName, QPoint(1,line), true, pMonkeyStudio::defaultCodec());
}
