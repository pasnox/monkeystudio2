/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbBacktrace
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbBreakpoint class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#include "gdbBacktrace.h"
#include <QMessageBox>
#include <QFileInfo>


GdbBacktrace::GdbBacktrace(QObject * parent, QPointer<GdbParser> pa , QPointer<GdbProcess> pr) : GdbCore(parent, pa, pr)
{
	// new connexion for auto-call onBreakpointAdd()
	Connect = new GdbConnectTemplate<GdbBacktrace>;

	setEnabled(true);
	setWaitEndProcess(false);

	mCurrentLine = -1;
	mCurrentFile.clear();
	
	/*
		Backtrace line splitted in two lines
			"#0  UIForm (this=0x80dc268, parent=0x0) "
			"  at src/ui/UIForm.cpp:43"
	*/
	/*
		#0 main (arc, arg) at src/main.cpp:23
	  
		#0  QObject::connect (sender=<incomplete type>, 
		signal=0x48e278 "2toggleBreakpoint(QString, int, bool)", 
		receiver=<incomplete type>, 
		method=0x48e250 "1onToggleBreakpoint(QString, int, bool)", 
		type=AutoConnection) at kernel/qobject.cpp:2348
	*/

	GdbCore::Parser()->addRestoreLine("^#0\\s.*" , 
		".*\\sat\\s.*:\\d+$");


	/*
		create new parser :

		cRegExpCmd = "bt"
		aRegExp = 
	*/

	
	interpreterBacktrace = GdbCore::Parser()->addInterpreter("Backtrace",
		"bt",
		QRegExp("^bt"),
		QRegExp("^#0\\s.*\\sat\\s.*:\\d+"),
		"^info,interpreter=\"" + name() + "\",event=\"Backtrace\",answerGdb=\"");

	// connect interpreter to function
	Connect->add(this, interpreterBacktrace, &GdbBacktrace::onBacktrace);


	/*
		create new parser :

		cRegExpCmd = "info"
		aRegExp = "(gdb) "
	*/
	interpreterInfoSource = GdbCore::Parser()->addInterpreter("Info-source",
		"info source",
		QRegExp("^info source"),
		QRegExp("^Located\\sin\\s.*"),
		"^info,interpreter=\"" + name() + "\",event=\"Info-source\",answerGdb=\"");

	Connect->add(this, interpreterInfoSource, &GdbBacktrace::onInfoSource);

	mWidget = new QTextEdit();
}


GdbBacktrace::~GdbBacktrace()
{
	delete Connect;
	delete mWidget;
}

QString GdbBacktrace::name()
{
	return "GdbBacktrace";
}


QPointer<QWidget> GdbBacktrace::widget()
{
	return (QPointer<QWidget>)( mWidget );
}

void GdbBacktrace::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect->call( i, id, s);
}



void GdbBacktrace::gdbFinished()
{
	setWaitEndProcess(false);
	mCurrentLine = -1;
	mCurrentFile.clear();
}

void GdbBacktrace::gdbStarted()
{
	setWaitEndProcess(false);
	mCurrentLine = -1;
	mCurrentFile.clear();
}

void GdbBacktrace::targetRunning(const int & id, const QString & s)
{
	// just delete bactrace when target running
	emit onToggleBacktrace("", -1);
}

void GdbBacktrace::targetStopped(const int & id, const QString & s)
{
	setWaitEndProcess(true);

	mWidget->append("*** target stopped ***");
	GdbCore::Parser()->setLastCommand("bt");
	GdbCore::Process()->sendRawData("bt");
}

void GdbBacktrace::targetExited(const int & id, const QString & s)
{
	// just delete bactrace when target exited
	emit onToggleBacktrace("", -1);
}

void GdbBacktrace::onBacktrace(int id, QString s)
{
	mWidget->append("** bactrace receiver **");
	
	/*
		#0  qMain (argc=1, argv=0x3d4c20) at src/main.cpp:65
	*/
	QRegExp r("^#0.*\\sat\\s([^:]+):(\\d+)$");

	if(r.exactMatch(findValue(s,"answerGdb")))
	{
		QStringList l = r.capturedTexts();
		mCurrentLine = l.at(2).toInt();
		mCurrentFile = l.at(1);

		mWidget->append("Current Line : " + QString::number(mCurrentLine));

		GdbCore::Parser()->setLastCommand("info source");
		GdbCore::Process()->sendRawData("info source");
	}
}

void GdbBacktrace::onInfoSource(int id, QString s)
{
	mWidget->append("** infosource receiver **");
	
	/*
	*/


	QRegExp r("Located\\sin\\s(.*)" );

	if(r.exactMatch( findValue(s,"answerGdb") ))
	{
		//get the full path of file
		QStringList l = r.capturedTexts();
		mCurrentFile = l.at(1);

		mWidget->append("Current File : " + mCurrentFile);

		emit onToggleBacktrace(mCurrentFile, mCurrentLine);

		setWaitEndProcess(false);
	}

}

void GdbBacktrace::onRequestBacktrace(const QString & s)
{
	mWidget->append("*** Request backtrace *** " + s);
	if(s == mCurrentFile)
		emit onToggleBacktrace(mCurrentFile, mCurrentLine);
}
