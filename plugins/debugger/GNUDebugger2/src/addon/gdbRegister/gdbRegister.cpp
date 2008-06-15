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

#include "gdbRegister.h"
#include <QMessageBox>
#include <QFileInfo>


GdbRegister::GdbRegister(QObject * parent) : GdbCore(parent)
{
	// new connexion 
	Connect = new GdbConnectTemplate<GdbRegister>;

	setEnabled(true);
	setWaitEndProcess(false);

	/*
	eax            0x1	1
	
	*/

	interpreterRegister = GdbCore::Parser()->addInterpreter(
		QRegExp("^info registers"),
		QRegExp("^\\w+\\s+0x\\w+\\s+.*"),
		"^info,interpreter=\"" + name() + "\",event=\"Register\",answerGdb=\"");

	// connect interpreter to function
	Connect->add(this, interpreterRegister, &GdbRegister::onRegister);



	Sequencer = new GdbSequencer(this);
	QList<SequencerCmd> s = QList<SequencerCmd>() << SequencerCmd("inforegister", "info registers")  ; 
	Sequencer->add(name() , s);

	mWidget = UIGdbRegister::self();
    mWidget->treeWidget->setAlternatingRowColors(true);

	numRegister = 0;
}

//

GdbRegister::~GdbRegister()
{
	delete Connect;
	delete mWidget;
}

//

QString GdbRegister::name()
{
	return "GdbRegister";
}

//

QPointer<QWidget> GdbRegister::widget()
{
	return (QPointer<QWidget>)( mWidget );
}

//

QIcon GdbRegister::icon()
{
	return QIcon(":/icons/cpu.png");
}


void GdbRegister::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect->call( i, id, s);
}

// Gdb status

void GdbRegister::gdbStarted()
{
	setWaitEndProcess(false);
	mWidget->treeWidget->clear();
}

//

void GdbRegister::gdbFinished()
{
	setWaitEndProcess(false);
	mWidget->treeWidget->clear();
}

//

void GdbRegister::gdbError(){}

// Taget status

void GdbRegister::targetLoaded(const int &, const QString &){}
void GdbRegister::targetNoLoaded(const int &, const QString &){}

//

void GdbRegister::targetRunning(const int & id, const QString & s)
{
}

//

void GdbRegister::targetStopped(const int & id, const QString & s)
{
	setWaitEndProcess(true);
	Sequencer->start();
}

//

void GdbRegister::targetExited(const int & id, const QString & s)
{
	mWidget->treeWidget->clear();
}

// Parser status

void GdbRegister::error(const int &, const QString & s)
{
	showMessage(name() + " have generate error : " + s, 5000, _CRITICAL_);
	setWaitEndProcess(false);
}

void GdbRegister::done(const int &, const QString &){}
void GdbRegister::info(const int &, const QString &){}

void GdbRegister::prompt(const int &, const QString &)
{
	numRegister = 0;
	setWaitEndProcess(false);
}

// Interpreters

void GdbRegister::onRegister(int id, QString s)
{

	/*
		#0  qMain (argc=1, argv=0x3d4c20) at src/main.cpp:65
	*/

	QRegExp r("^(\\w+)\\s+(0x\\w+)\\s+(.*)$");

	if(r.exactMatch(findValue(s,"answerGdb")))
	{
		QStringList l = r.capturedTexts();
		
		QTreeWidgetItem * i;

		if(numRegister < mWidget->treeWidget->topLevelItemCount()) 
		{
			i = mWidget->treeWidget->topLevelItem ( numRegister);
			showColor(i, l.at(2));

		}
		else i = new QTreeWidgetItem(mWidget->treeWidget);

		i->setText(0,l.at(1));
		i->setText(1,l.at(2));
		i->setText(2,l.at(3));
		numRegister++;
	}
}

//

void GdbRegister::showColor(QTreeWidgetItem *p, QString a)
{
	// toggle color (black / red if value in treeWidget is not egal than new value
	if(p->text(1) != a)
	{
		p->setForeground( 1, QBrush(Qt::red));
		p->setForeground( 2, QBrush(Qt::red));
	}
	else
	{
		p->setForeground( 1 , QBrush(Qt::black));
		p->setForeground( 2 , QBrush(Qt::black));
	}
}

