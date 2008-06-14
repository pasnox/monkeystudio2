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

class GdbKernelDispatcher : public QObject, public QSingleton<GdbKernelDispatcher>
{

	Q_OBJECT
	friend class QSingleton<GdbKernelDispatcher>;

public :

	GdbKernelDispatcher(QObject * parent = 0);
	~GdbKernelDispatcher();

	void add(/*const QPointer<class GdbParser> &, */ const QPointer< class GdbCore> &);
	void remove( const QPointer< class GdbCore> &);
	void removeAll();

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

	QList<QPointer< class GdbCore> > list() { return addonList;}

private :

	QList<QPointer< class GdbCore> > addonList;

	QPointer<GdbParser> mParser;
};

#endif
