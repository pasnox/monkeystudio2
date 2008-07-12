/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbKernelDispatcher
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/


#ifndef GDBKERNELDISPATCHER_H
#define GDBKERNELDISPATCHER_H

#include <QObject>
#include "gdbCore.1.3.h"
#include "gdbParser.1.3.h"

//! This class dispatch all messages to all AddOn
/**
\author xiantia
\version 1.3.2

DockGNUDebugger connect some signals from GdbParser class (done, error, info ,interpreter, targetStopped, targetLoaded,....) to some slots (
onDone, onError, onInfo, onInterpreter, ... ).

This slots call GdbKernelDispatcher::error() for example. Now all error() function of all AddOn is called.

If you want write a new AddOn for GNU debugger, just derive GdbCore and re-implements all functions and add your AddOn by call GdbKernelDispatcher::add().
in DockGNUDebugger class
*/

class GdbKernelDispatcher : public QObject, public QSingleton<GdbKernelDispatcher>
{

	Q_OBJECT
	friend class QSingleton<GdbKernelDispatcher>;

public :

	GdbKernelDispatcher(QObject * parent = 0);
	~GdbKernelDispatcher();

	//! Add AddOn to the list
	void add( const QPointer< class GdbCore> &);
	//! Remove AddOn to the list
	void remove( const QPointer< class GdbCore> &);
	//! Remove all AddOn
	void removeAll();

	//! Call all AddOn::gdbStarted() function
	void gdbStarted();
	//! Call all AddOn::gdbFinished() function
	void gdbFinished();
	//! Call all AddOn::gdbError() function
	void gdbError();

	//! Call all AddOn::targetLoaded() function
	void targetLoaded(const int &, const QString &);
	//! Call all AddOn::targetNoLoaded() function
	void targetNoLoaded(const int &, const QString &);
	//! Call all AddOn::targetRunning() function
	void targetRunning(const int &, const QString &);
	//! Call all AddOn::targetStopped() function
	void targetStopped(const int &, const QString &);
	//! Call all AddOn::targetExited() function
	void targetExited(const int &, const QString &);

	//! Call all AddOn::error() function
	void error(const int &, const QString &);
	//! Call all AddOn::done() function
	void done(const int &, const QString &);
	//! Call all AddOn::info() function
	void info(const int &, const QString &);
	//! Call all AddOn::prompt() function
	void prompt(const int &, const QString &);

	//! Return all AddOn in this list
	QList<QPointer< class GdbCore> > list() { return addonList;}

private :

	//! List of all AddOn pointer
	QList<QPointer< class GdbCore> > addonList;

	//! Pointer to the GdbParser class
	QPointer<GdbParser> mParser;
};

#endif
