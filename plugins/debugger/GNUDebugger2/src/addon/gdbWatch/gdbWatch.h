/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbWatch
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbWatch class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBWATCH_H
#define GDBWATCH_H

#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"
#include "./ui/UIGdbWatch.h"
#include "../gdbScript/gdbScript.h"

#include "gdbWatchStruct.h"

//! This class implements all function see user values. This class is an AddOn for GNU debugger 
/**
\author xiantia
\version 1.3.2
*/
class GdbWatch : public GdbCore
{
	Q_OBJECT


public:

	GdbWatch(QObject * parent = 0);
	~GdbWatch();

public slots:

	void onType( int , QString );
	void onAddress( int , QString );
	void onValue( int , QString );
	void onUserAddVar(QString);
	void onScriptFinishedTranslate(const QString &);

	QString name();
	QPointer<QWidget> widget();
	QIcon icon();

	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	// gdb
	void gdbFinished();
	void gdbStarted();
	void gdbError();

	// target
	void targetLoaded(const int &, const QString &);
	void targetNoLoaded(const int &, const QString &);
	void targetRunning(const int &, const QString &);
	void targetStopped(const int &, const QString &);
	void targetExited(const int &, const QString &);

	// Parser
	void error(const int &, const QString &);
	void done(const int &, const QString &);
	void info(const int &, const QString &);
	void prompt(const int &, const QString &);

private:

	GdbConnectTemplate<GdbWatch> Connect;
	QPointer<BaseInterpreter> interpreterAddress;
	QPointer<BaseInterpreter> interpreterWhatis;
	QPointer<BaseInterpreter> interpreterValue;

	QPointer<GdbSequencer> Sequencer;
	QPointer<UIGdbWatch> mWidget;


	void showColor(QTreeWidgetItem *, int , QString );
	int currentVar;
	void createSequencer();
	bool isPointer(const QString & );

	QString mOldValue;

signals:

	void requestScriptTranslate(const QString &, const QString &);

};

#endif
