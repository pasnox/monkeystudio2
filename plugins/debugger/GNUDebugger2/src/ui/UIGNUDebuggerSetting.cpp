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

	Process = new QProcess(this);
	
	connect(Process, SIGNAL( readyRead( )), this , SLOT( onReadyRead(  )));
	connect(Process, SIGNAL( started( )), this, SLOT(started()));
	connect(Process, SIGNAL( finished(  int , QProcess::ExitStatus  )), this, SLOT(finished( int , QProcess::ExitStatus)));
	connect(Process, SIGNAL( error ( QProcess::ProcessError )), this, SLOT(error(QProcess::ProcessError)));


	connect(bButtonPathGdb, SIGNAL( clicked()), this, SLOT(bClickedPathGdb()));
	connect(bButtonPathParsingFile, SIGNAL( clicked()), this, SLOT(bClickedPathParsingFile()));
	connect(bButtonManageFile, SIGNAL( clicked()), this, SLOT(bClickedManageFile()));
	connect(bButtonPathScript, SIGNAL( clicked()), this, SLOT(bClickedPathScript()));


	editPathGdb->setText( GdbSetting::instance()->getPathGdb() );
	editPathParsingFile->setText( GdbSetting::instance()->getPathParseFile() );
	editPathScript->setText( GdbSetting::instance()->getPathScript() );

	vb = new QVBoxLayout(groupPlugins);

	QList<GdbAddOn> *l = GdbSetting::instance()->getAddOnList();
	foreach(GdbAddOn  p,  *l)
	{
		QCheckBox * cb = new QCheckBox( p.name );
		vb->addWidget(cb);	
		p.enable ? cb->setCheckState(Qt::Checked) : cb->setCheckState(Qt::Unchecked);
	}

	Process->start(GdbSetting::instance()->getPathGdb());
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
	if(!mPathParsingFile.isEmpty())
		editPathParsingFile->setText(mPathParsingFile);
}

//

void UIGNUDebuggerSetting::bClickedPathScript()
{
	QString mDir = QFileDialog::getExistingDirectory(this, tr("Location of Scripts"));
	if(!mDir.isEmpty())
		editPathScript->setText(mDir);
}

//

void UIGNUDebuggerSetting::bClickedPathGdb()
{
	QString mPathGdb = QFileDialog::getOpenFileName(this, tr("Location of Gdb"));

	if(!mPathGdb.isEmpty())
	{
		editPathGdb->setText(mPathGdb);
		Process->start(mPathGdb);
		Process->waitForStarted(1000);
		Process->write("quit\r\n");
	}
}

void UIGNUDebuggerSetting::onReadyRead( )
{
	QByteArray b = Process->readAll();

	// read version of GDB
	QStringList l = QString(b).split( pMonkeyStudio::getEol());
	if(l.count())
		labelCurrentVersion->setText(l.at(0));

	Process->write("quit\r\n");
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
	// get the path
	GdbSetting::instance()->setPathGdb(editPathGdb->text());
	GdbSetting::instance()->setPathParseFile(editPathParsingFile->text());
	GdbSetting::instance()->setPathScript(editPathScript->text());

	// enable or not AddOn
	QList<QCheckBox*> cbs = groupPlugins->findChildren<QCheckBox*>();
	foreach(QCheckBox * c, cbs)
	{
		GdbSetting::instance()->setEnable( c->text() , (c->checkState () == Qt::Checked) ? true : false); 
	}
	GdbSetting::instance()->save();
}
