/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbToolTip
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbToolTip class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#include "gdbToolTip.h"
#include <QToolTip>


GdbToolTip::GdbToolTip(QObject * parent) : GdbCore(parent)
{
	// new connexion 

	setEnabled(true);
	setWaitEndProcess(false);
	enteredShowVar = false;
	isGdbStarted = false;
	isTargetStopped = false;

	GdbCore::Parser()->addRestoreLine(name(),
		"^\\$\\d+\\s+=\\s+.*" , 
		".*\\(gdb\\)\\s*$");

	interpreterValue = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^print .*"), 
		QRegExp("^\\$\\d+\\s+=\\s+.*"),
		"^info,interpreter=\"" + name() + "\",event=\"WatchValue\",answerGdb=\"");

	// connect interpreters to functions
	Connect.add(this, interpreterValue, &GdbToolTip::onValue);

	// create sequencer 
	Sequencer = new GdbSequencer(this);
}

//

GdbToolTip::~GdbToolTip()
{
}

//

QString GdbToolTip::name()
{
	return "GdbToolTip";
}

//

QPointer<QWidget> GdbToolTip::widget()
{
	return (QPointer<QWidget>)( NULL );
}

//

QIcon GdbToolTip::icon()
{
	return QIcon();
}

//

void GdbToolTip::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect.call( i, id, s);
}

// Gdb status

void GdbToolTip::gdbStarted()
{
	setWaitEndProcess(false);
	isGdbStarted = true;
}

//

void GdbToolTip::gdbFinished()
{
	setWaitEndProcess(false);
	isGdbStarted = false;
}

//

void GdbToolTip::gdbError()
{
	isGdbStarted= false;
}

// Taget status

void GdbToolTip::targetLoaded(const int &, const QString &){}
void GdbToolTip::targetNoLoaded(const int &, const QString &){}
void GdbToolTip::targetRunning(const int & id, const QString & s)
{
	isTargetStopped = false;
}

void GdbToolTip::targetStopped(const int & id, const QString & s)
{
	isTargetStopped = true;
}

void GdbToolTip::targetExited(const int & id, const QString & s)
{
	isTargetStopped = true;
}

// Parser status

void GdbToolTip::error(const int &, const QString & s)
{
	enteredShowVar=false;
}

//

void GdbToolTip::done(const int &, const QString & s){}
void GdbToolTip::info(const int &, const QString &){}

void GdbToolTip::prompt(const int &, const QString & )
{
	setWaitEndProcess(false);
}

// Interpreters

void GdbToolTip::onValue(int, QString s)
{
	setWaitEndProcess(false);
	enteredShowVar=false;
	Sequencer->remove();
	QString v = findValue(s,"answerGdb");
	v = v.right(v.length() - v.indexOf(" = ") - 3);
	QToolTip::showText(QCursor::pos (), v, NULL);
}

// slot from Qsci

void GdbToolTip::onRequestShowVar(const QString & n)
{
	if(!enteredShowVar && isGdbStarted && isTargetStopped)
	{
		enteredShowVar = true;
		QToolTip::hideText();

		QList<SequencerCmd> s;
		s <<  SequencerCmd("printValue", "print " + n) ; 
		Sequencer->add(name() , s);
		Sequencer->start();
		setWaitEndProcess(true);
	}
}
