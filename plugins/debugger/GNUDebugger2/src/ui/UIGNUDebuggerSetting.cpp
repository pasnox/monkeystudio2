#include "UIGNUDebuggerSetting.h"

#include <coremanager.h>
#include <settingsmanager.h>
#include "monkey.h"

#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>

#include "../kernel/gdbSetting.1.3.h"

UIGNUDebuggerSetting::UIGNUDebuggerSetting( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );

	Process = new GdbProcess(this);
	
	connect(Process, SIGNAL( commandReadyRead( const QByteArray & )), this , SLOT( commandReadyRead( const QByteArray & )));
	connect(Process, SIGNAL( started( )), this, SLOT(started()));
	connect(Process, SIGNAL( finished(  int , QProcess::ExitStatus  )), this, SLOT(finished( int , QProcess::ExitStatus)));
	connect(Process, SIGNAL( error ( QProcess::ProcessError )), this, SLOT(error(QProcess::ProcessError)));


	connect(bButtonPathGdb, SIGNAL( clicked()), this, SLOT(bClickedPathGdb()));
	connect(bButtonPathParsingFile, SIGNAL( clicked()), this, SLOT(bClickedPathParsingFile()));
	connect(bButtonManageFile, SIGNAL( clicked()), this, SLOT(bClickedManageFile()));


	editPathGdb->setText( GdbSetting::instance()->getPathGdb() );
	editPathParsingFile->setText( GdbSetting::instance()->getPathParseFile() );

	QVBoxLayout *vb = new QVBoxLayout(groupPlugins);

	QList<GdbAddOn> *l = GdbSetting::instance()->getAddOnList();
	foreach(GdbAddOn  p,  *l)
	{
		QCheckBox * cb = new QCheckBox( p.name );
		vb->addWidget(cb);	
		p.enable ? cb->setCheckState(Qt::Checked) : cb->setCheckState(Qt::Unchecked);
	}

//	mPathGdb = ;
	Process->setCommand(GdbSetting::instance()->getPathGdb());//mPathGdb);
	Process->startProcess();
}

//

void UIGNUDebuggerSetting::bClickedManageFile()
{
	UIManageParsingFile *d = UIManageParsingFile::self(this) ;
	d->exec();
}

//

void UIGNUDebuggerSetting::bClickedPathParsingFile()
{
	QString mPathParsingFile = QFileDialog::getOpenFileName(this, tr("Location of Parsing file"));
	editPathParsingFile->setText(mPathParsingFile);
}

//

void UIGNUDebuggerSetting::bClickedPathGdb()
{
	QString mPathGdb = QFileDialog::getOpenFileName(this, tr("Location of Gdb"));

	if(!mPathGdb.isEmpty())
	{
		editPathGdb->setText(mPathGdb);
		Process->setCommand(mPathGdb);
		Process->startProcess();
		
		Process->sendRawData("quit");
	}
}

void UIGNUDebuggerSetting::commandReadyRead( const QByteArray & s)
{
	// read version of GDB
	QStringList l = QString(s).split( pMonkeyStudio::getEol());
	if(l.count())
		labelCurrentVersion->setText(l.at(0));
	Process->sendRawData("quit");
}

void UIGNUDebuggerSetting::started()
{}

void UIGNUDebuggerSetting::finished( int , QProcess::ExitStatus  )
{}

// gdb no found
void UIGNUDebuggerSetting::error( QProcess::ProcessError )
{
	labelCurrentVersion->setText(tr("Version not found"));
}

void UIGNUDebuggerSetting::on_dbbButtons_clicked( QAbstractButton * button )
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
}

void UIGNUDebuggerSetting::saveSettings()
{
	GdbSetting::instance()->setPathGdb(editPathGdb->text());
	GdbSetting::instance()->setPathParseFile(editPathParsingFile->text());

	QList<QCheckBox*> cbs = groupPlugins->findChildren<QCheckBox*>();
	foreach(QCheckBox * c, cbs)
	{
		GdbSetting::instance()->setEnable( c->text() , (c->checkState () == Qt::Checked) ? true : false); 
	}
	GdbSetting::instance()->save();
}



