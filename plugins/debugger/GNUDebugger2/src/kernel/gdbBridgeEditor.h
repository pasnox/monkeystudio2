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
	\file gdbBridgeEditor.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Implements all functions for remote Qsci.
*/
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

/*!
	\brief Implements all functions for remote Qsci.
	\details This class configure Qsci. For example, set sensitivity margin, set read only editor and add or remove marker in the margin.
	For this a struct Editor is used. All signals and slots are connected to the other class by DockGNUDebugger class.  
*/
class GdbBridgeEditor : public QObject , public QSingleton<GdbBridgeEditor>
{
	Q_OBJECT
	friend class QSingleton<GdbBridgeEditor>;

	/*!
		\brief Struct for store fileName and Qsci pointer
	*/
	typedef struct Editor
	{
		QString fileName;
		pEditor * pointeur;
	};


public :
	GdbBridgeEditor(QObject * parent = 0);
	~GdbBridgeEditor();

public slots:

	void addEditor(const QString &);
	void removeEditor(const int &);
	void removeAllBreakpoints();
	void removeBacktrace();

	void onMarginClicked(int, int, Qt::KeyboardModifiers);
	void onToggleBreakpoint(const Breakpoint &, const BaseBreakpoint & , const bool & b);
	void onToggleBacktrace(const QString & , const int & );
	void onCopyAvailable(bool);

private:

	pEditor * findFile(const QString &);

	/*! 
		\details List of all editor opened
		\sa addEditor(), removeEditor()
	*/
	QList<Editor> editorList;

	void removeAllBreakpointsAt( pEditor * , const int & );
	void fileOpenedBeforeDebugger();

signals :

	/*!
		\details Emit when user click in the margin
		\param fileName is the name of file.
		\param line is the number of line where user has toggle breakpoint.
		\sa GdbBreakpoint::toggleBreakpoint()
	*/
	void userToggleBreakpoint(const QString &fileName , const int &line);

	/*!
		\details Emit when an editor is opened.
		This signal indicate that this editor request breakpoint for this file.
		\param fileName is the name of file.
		\sa GdbBreakpoint::onRequestBreakpoint()
	*/
	void requestBreakpoint(const QString &fileName);

	/*!
		\details Emit when an editor is opened.
		This signal indicate that this editor request backtrace for this file
		\param fileName is the name of file.
		\sa GdbBacktrace::onRequestBacktrace()
	*/
	void requestBacktrace(const QString &fileName);

	/*!
		\details Request show var, user select string in editor.
		This signal indicate that this editor has selected string and want show value.
		\param textSelected is the text selected by the user in Qsci.
		\sa GdbToolTip::onRequestShowVar()
	*/
	void requestShowVar(const QString &textSelected);
};
#endif
