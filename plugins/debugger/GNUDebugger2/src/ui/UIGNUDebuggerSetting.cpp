#include "UIGNUDebuggerSetting.h"

#include <coremanager.h>
#include <settingsmanager.h>
#include "monkey.h"

#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>


UIGNUDebuggerSetting::UIGNUDebuggerSetting( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );

	// default
	loadSettings();

	Process = new GdbProcess(this);
	
	connect(Process, SIGNAL( commandReadyRead( const QByteArray & )), this , SLOT( commandReadyRead( const QByteArray & )));
	connect(Process, SIGNAL( started( )), this, SLOT(started()));
	connect(Process, SIGNAL( finished(  int , QProcess::ExitStatus  )), this, SLOT(finished( int , QProcess::ExitStatus)));
	connect(Process, SIGNAL( error ( QProcess::ProcessError )), this, SLOT(error(QProcess::ProcessError)));

	Process->setCommand(mPathGdb);

	connect(bButtonPath, SIGNAL( clicked()), this, SLOT(bClicked()));

	Process->startProcess();

	editPath->setText(mPathGdb);

	// add setting 
	QVBoxLayout *l = new QVBoxLayout();

	Settings* s = MonkeyCore::settings();
	s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );

	foreach(QString p, mPluginList)
	{
		bool e = s->value( p, true).toBool();
		if(p.startsWith("AddOn"))
		{
			QCheckBox *a = new QCheckBox( p.right( p.length() - p.indexOf("/") - 1) );
			l->addWidget(a);	
			e ? a->setCheckState(Qt::Checked) : a->setCheckState(Qt::Unchecked);
		}
	}
	s->endGroup();
	groupPlugins->setLayout(l);
}


void UIGNUDebuggerSetting::bClicked()
{
	mPathGdb = QFileDialog::getOpenFileName(this, tr("Location of Gdb"));

	if(!mPathGdb.isEmpty())
	{
		editPath->setText(mPathGdb);
		Process->sendRawData("quit");
		Process->stopProcess();

		Process->setCommand(mPathGdb);
		Process->startProcess();
	}
}

void UIGNUDebuggerSetting::commandReadyRead( const QByteArray & s)
{
	// read version of GDB
	QStringList l = QString(s).split( pMonkeyStudio::getEol());

	labelCurrentVersion->setText(l.at(0));
	Process->sendRawData("quit");
	Process->stopProcess();
}

void UIGNUDebuggerSetting::started()
{
}

void UIGNUDebuggerSetting::finished( int , QProcess::ExitStatus  )
{
}

// gdb no found
void UIGNUDebuggerSetting::error( QProcess::ProcessError )
{
	labelCurrentVersion->setText("Gdb no found");
}

void UIGNUDebuggerSetting::on_dbbButtons_clicked( QAbstractButton* button )
{
	if ( button == dbbButtons->button( QDialogButtonBox::Apply ) )
	{
		// here your code for apply/save
		saveSettings();
		// restart is requier !
	}
}


void UIGNUDebuggerSetting::loadSettings()
{
	Settings* s = MonkeyCore::settings();
	s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );
	mPathGdb = s->value("PathGdb", "gdb").toString();
	mPluginList = s->allKeys();
	s->endGroup();

 }

void UIGNUDebuggerSetting::saveSettings()
{
	Settings * s = MonkeyCore::settings();
	s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );
		s->setValue( "PathGdb", editPath->text() );
		
		QList<QCheckBox*> cbs = groupPlugins->findChildren<QCheckBox*>();
		foreach(QCheckBox * c, cbs)
		{
			if(c->checkState () == Qt::Checked) 
				s->setValue("AddOn/" + c->text(), true);
			else
				s->setValue("AddOn/" + c->text(), false);
		}
	s->endGroup();
}



