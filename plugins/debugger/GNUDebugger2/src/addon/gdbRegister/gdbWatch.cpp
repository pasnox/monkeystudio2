/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbRegister
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbBacktrace class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#include "gdbWatch.h"
#include <QMessageBox>
#include <QFileInfo>


GdbWatch::GdbWatch(QObject * parent) : GdbCore(parent)
{
	// new connexion 
	Connect = new GdbConnectTemplate<GdbWatch>;

	setEnabled(true);
	setWaitEndProcess(false);

	GdbCore::Parser()->addRestoreLine(name(),
		"^\\$\\d+\\s+=\\s+.*" , 
		".*\\(gdb\\)\\s*$");


	interpreterWhatis = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^whatis .*"),
		QRegExp("^type\\s+=\\s+.*"),
		"^info,interpreter=\"" + name() + "\",event=\"WatchType\",answerGdb=\"");

	interpreterAddress = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^p .*"), // use p and not print command because interpreterValue use this
		QRegExp("^\\$\\d+\\s+=\\s+.*"),
		"^info,interpreter=\"" + name() + "\",event=\"WatchAddress\",answerGdb=\"");

	interpreterValue = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^print .*"), // use print and not p because interpreterAddress use this
		QRegExp("^\\$\\d+\\s+=\\s+.*"),
		"^info,interpreter=\"" + name() + "\",event=\"WatchValue\",answerGdb=\"");

	// connect interpreters to functions
	Connect->add(this, interpreterWhatis, &GdbWatch::onType);
	Connect->add(this, interpreterValue, &GdbWatch::onValue);
	Connect->add(this, interpreterAddress, &GdbWatch::onAddress);

	// create sequencer for test
	Sequencer = new GdbSequencer(this);
	QList<SequencerCmd> s = QList<SequencerCmd>() 	<< SequencerCmd("printType", "whatis s") << SequencerCmd("printAdress", "p &s") << SequencerCmd("printValue", "print s") ; 
	Sequencer->add(name() , s);

	// create Ui
	mWidget = UIGdbWatch::self();
    mWidget->treeWidget->setAlternatingRowColors(true);
}

//

GdbWatch::~GdbWatch()
{
	delete Connect;
	delete mWidget;
}

//

QString GdbWatch::name()
{
	return "GdbWatch";
}

//

QPointer<QWidget> GdbWatch::widget()
{
	return (QPointer<QWidget>)( mWidget );
}

//

QIcon GdbWatch::icon()
{
	return QIcon(":/icons/watch.png");
}

//

void GdbWatch::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect->call( i, id, s);
}

// Gdb status

void GdbWatch::gdbStarted()
{
	setWaitEndProcess(false);
	mWidget->treeWidget->clear();
}

//

void GdbWatch::gdbFinished()
{
	setWaitEndProcess(false);
	mWidget->treeWidget->clear();
}

//

void GdbWatch::gdbError(){}

// Taget status

void GdbWatch::targetLoaded(const int &, const QString &){}
void GdbWatch::targetNoLoaded(const int &, const QString &){}
void GdbWatch::targetRunning(const int & id, const QString & s){}

//

void GdbWatch::targetStopped(const int & id, const QString & s)
{
	setWaitEndProcess(true);
	currentVar = 0;
	Sequencer->start();
}

//

void GdbWatch::targetExited(const int & id, const QString & s)
{
	mWidget->treeWidget->clear();
}

// Parser status

void GdbWatch::error(const int &, const QString & s)
{
	showMessage(name() + " have generate error : " + s, 5000, _CRITICAL_);
	setWaitEndProcess(false);
}

//

void GdbWatch::done(const int &, const QString & s)
{
/*	if(findValue(s,"^done,interpreter") == "GdbScript")
	{
//		QMessageBox::warning(NULL,"Done message...", s);
		scp.data(findValue(s, "answerGdb"));
		//processReceive(findValue(s, "answerGdb"));
	}
*/
}

//

void GdbWatch::info(const int &, const QString &){}

void GdbWatch::prompt(const int &, const QString & s)
{
	setWaitEndProcess(false);
/*	if(findValue(s,"^prompt,interpreter") == "GdbScript")
	{
		scp.exec();
	}
*/
}

// Interpreters

void GdbWatch::onType(int id, QString s)
{
	QRegExp r("^type\\s+=\\s+(.*)$");

	if(r.exactMatch(findValue(s,"answerGdb")))
	{
		QStringList l = r.capturedTexts();
		
		QTreeWidgetItem * i;

		if(currentVar < mWidget->treeWidget->topLevelItemCount()) 
		{
			i = mWidget->treeWidget->topLevelItem ( currentVar);
			showColor(i, l.at(1));

		}
		else i = new QTreeWidgetItem(mWidget->treeWidget);


		i->setText(1,l.at(1));
	}
	// execute next command if have
	Sequencer->loop();
}

//

void GdbWatch::onValue(int id, QString s)
{
	QRegExp r("^\\$\\d+\\s+=\\s+(.*)$");

	if(r.exactMatch(findValue(s,"answerGdb")))
	{
		QStringList l = r.capturedTexts();
		
		QTreeWidgetItem * i;

		if(currentVar < mWidget->treeWidget->topLevelItemCount()) 
		{
			i = mWidget->treeWidget->topLevelItem ( currentVar);
			showColor(i, l.at(1));
		}
		else i = new QTreeWidgetItem(mWidget->treeWidget);

		i->setText(4,l.at(1));

		// decompil struct 
		WatchStruct w;
		w.decompilStrut(i,l.at(1)); 
	}
	Sequencer->loop();
}

//

void GdbWatch::onAddress(int id, QString s)
{
	QRegExp r("^\\$\\d+\\s+=\\s+(.*)$");

	if(r.exactMatch(findValue(s,"answerGdb")))
	{
		QStringList l = r.capturedTexts();
		
		QTreeWidgetItem * i;

		if(currentVar < mWidget->treeWidget->topLevelItemCount()) 
		{
			i = mWidget->treeWidget->topLevelItem ( currentVar);
			showColor(i, l.at(1));

		}
		else i = new QTreeWidgetItem(mWidget->treeWidget);

		i->setText(2,l.at(1));
	}
	Sequencer->loop();
}

//

void GdbWatch::showColor(QTreeWidgetItem *p, QString a)
{
	// toggle color (black / red if value in treeWidget is not egal than new value
	if(p->text(1) != a)
	{
		p->setForeground( 1, QBrush(Qt::red));
	}
	else
	{
		p->setForeground( 1 , QBrush(Qt::black));
	}
}

