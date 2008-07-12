/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbBridgeEditor
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbInterpreter class

	Store all interpreter 


	Xiantia@gmail.com

	for Debugger v1.3.0
*/


#ifndef GDBBRIDGEEDITOR_H
#define GDBBRIDGEEDITOR_H

#include <QObject>

#include <fresh.h>
#include <workspacemanager.h>

#include <coremanager.h>
#include <maininterface.h>
#include <qscintillamanager.h>

#include "../addon/gdbBreakpoint/gdbBreakpointStruct.h"

//! Implements all functions for remote Qsci.

/**
\author xiantia
\version 1.3.2

This class configure Qsci. For example, set sensitivity margin, set read only editor and add or remove marker in the margin.
For this a struct Editor is used. All signals and slots are connected to the other class by DockGNUDebugger class.  
*/

class GdbBridgeEditor : public QObject , public QSingleton<GdbBridgeEditor>
{

	Q_OBJECT
	friend class QSingleton<GdbBridgeEditor>;

	//! struct for store fileName and Qsci pointer
	typedef struct Editor
	{
		//! fileName 
		QString fileName;
		//! Qsci pointer 
		pEditor * pointeur;
	};


public :
	GdbBridgeEditor(QObject * parent = 0);
	~GdbBridgeEditor();

public slots:

	//! New file is opened
	/**
	When a new editor (source code) is opened this function is call, it store fileName and pointer in Editor struct. 
	Set sensitivity the margin and request if this file has breakpoint or backtrace before the last closed.
	*/
	void addEditor(const QString &);

	//! Remove Qsci pointer to the list
	void removeEditor(const int &);
	//! Remove all icons (breakpoint) in QSci margin  for all editor opened
	void removeAllBreakpoints();
	//! Remove all icons (backtrace) in Qsci margin for all editor opened
	void removeBacktrace();

	//! User has clicked in the margin
	/**
	When user click in the margin, this function emit userToggleBreakpoint signal.
	GdbBreakpoint class answer and call onToggleBreakpoint function via DockGNUDebugger class.
	*/
	void onMarginClicked(int, int, Qt::KeyboardModifiers);

	//! Toggle breakpoint icon in margin
	void onToggleBreakpoint(const Breakpoint &, const BaseBreakpoint & , const bool & b);
	//! Toggle backtrace icon in margin
	/**
	Move backtrace in editor where Gdb has breaked
	*/
	void onToggleBacktrace(const QString & , const int & );
	

	//! User has selected string in editor
	/**
	If user select string in editor, this string is possible a var, Gdb can print the value of this.
	GdbToolTip show this value in the tooltip.
	*/
	void onCopyAvailable(bool);

private:

	//! find file by name
	/**
	* \retval Pointer to the Qsci editor
	*/
	pEditor * findFile(const QString &);

	//! List of all editor opened
	QList<Editor> editorList;

	//! Remove all icons (breakpoints) at line for only one editor
	void removeAllBreakpointsAt( pEditor * , const int & );
	//! Same as add function, but call when editor is opened before GNU plugin is started
	void fileOpenedBeforeDebugger();

signals :

	//! Emit when user click in the margin 
	void userToggleBreakpoint(const QString & , const int &);
	//! Request breakpoint for the file
	void requestBreakpoint(const QString &);
	//! Request backtrace for this file
	void requestBacktrace(const QString &);
	//! Request show var, user select string in editor
	void requestShowVar(const QString &);
};
#endif
