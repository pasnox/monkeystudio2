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
	\file gdbKernelDispatcher.1.3.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class dispatch all messages to all AddOn
*/

#ifndef GDBKERNELDISPATCHER_H
#define GDBKERNELDISPATCHER_H

#include <QObject>
#include "gdbCore.h"
#include "gdbParser.h"

/*!
	\brief This class dispatch all messages to all AddOn
	\details DockGNUDebugger connect some signals from GdbParser class (done, error, info ,interpreter, targetStopped, targetLoaded,....) to some slots (
	onDone, onError, onInfo, onInterpreter, ... ).

	This slots call GdbKernelDispatcher::error() for example. Now all error() function of all AddOn is called.

	If you want write a new AddOn for GNU debugger, just derive GdbCore and re-implements all functions and 
	add your AddOn by call GdbKernelDispatcher::add() in DockGNUDebugger class
*/
class GdbKernelDispatcher : public QObject, public QSingleton<GdbKernelDispatcher>
{
	Q_OBJECT
	friend class QSingleton<GdbKernelDispatcher>;

public :

	GdbKernelDispatcher(QObject * parent = 0);
	~GdbKernelDispatcher();

	void add( const QPointer< class GdbCore> &);

	void gdbStarted();
	void gdbFinished();
	void gdbError();

	void targetLoaded(const int &, const QString &);
	void targetNoLoaded(const int &, const QString &);
	void targetRunning(const int &, const QString &);
	void targetStopped(const int &, const QString &);
	void targetExited(const int &, const QString &);

	void error(const int &, const QString &);
	void done(const int &, const QString &);
	void info(const int &, const QString &);
	void prompt(const int &, const QString &);

	/*!
		\details Return all AddOn list
	*/
	QList<QPointer< class GdbCore> > list() { return addonList;}

private :

	/*! 
		\details List of all AddOn pointer
	*/
	QList<QPointer< class GdbCore> > addonList;

	/*!
		\details Pointer to the GdbParser class
	*/
	QPointer<GdbParser> mParser;
};

#endif
