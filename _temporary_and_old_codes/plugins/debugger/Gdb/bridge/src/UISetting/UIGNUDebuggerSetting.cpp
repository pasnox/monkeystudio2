/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
/*!
	\file UIGNUDebuggerSetting.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Main Ui for setting GNU debugger
*/


#include "UIGNUDebuggerSetting.h"


#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>

#include "../../../lib/gdb.h"
/*!
	\details Create new object
	\param parent of this object
*/
UIGNUDebuggerSetting::UIGNUDebuggerSetting( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );

	Process = new QProcess(this);
	
	connect(Process, SIGNAL( readyRead( )), this , SLOT( onReadyRead()),Qt::DirectConnection);
	connect(Process, SIGNAL( started( )), this, SLOT(started()),Qt::DirectConnection);
	connect(Process, SIGNAL( finished(  int , QProcess::ExitStatus  )), this, SLOT(finished( int , QProcess::ExitStatus)),Qt::DirectConnection);
	connect(Process, SIGNAL( error ( QProcess::ProcessError )), this, SLOT(error(QProcess::ProcessError)),Qt::DirectConnection);

	connect(bButtonPathGdb, SIGNAL( clicked()), this, SLOT(bClickedPathGdb()),Qt::DirectConnection);
	connect(bButtonPathScript, SIGNAL( clicked()), this, SLOT(bClickedPathScript()),Qt::DirectConnection);


	editPathGdb->setText( GdbCore::Setting()->getPathGdb() );
	editPathScript->setText( GdbCore::Setting()->getPathScript() );

	// show all addOn
	vb = new QVBoxLayout(groupPlugins);
	QHash<QString, bool > l = GdbCore::Setting()->getAddons();
	QHashIterator<QString, bool> i(l);
	while (i.hasNext()) 
	{
		i.next();
		QCheckBox * cb = new QCheckBox( i.key() );
		vb->addWidget(cb);	
		i.value() ? cb->setCheckState(Qt::Checked) : cb->setCheckState(Qt::Unchecked);
	}

	// get path Gdb and try run for show version
	QString mPathGdb = GdbCore::Setting()->getPathGdb();
	if(!mPathGdb.isEmpty())
		Process->start(mPathGdb);
}

//
/*!
	\details Button path script file clicked.
	Open Ui for this
*/
void UIGNUDebuggerSetting::bClickedPathScript()
{
}

//

/*!
	\details Button path Gdb file clicked.
	Open Ui for this
*/
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

/*! 
	\details Data from Gdb (it is not GdbProcess).
	Get the current version and quit Gdb.

	\code
		GNU gdb 6.7.50.20071127
		Copyright (C) 2007 Free Software Foundation, Inc.
		License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
		This is free software: you are free to change and redistribute it.
		There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
		and "show warranty" for details.
		This GDB was configured as "i686-pc-mingw32".
	\endcode
	
	The current version is "GNU gdb 6.7.50.20071127"

*/
void UIGNUDebuggerSetting::onReadyRead( )
{
	QByteArray b = Process->readAll();

	// read version of GDB
	QStringList l = QString(b).split( pMonkeyStudio::getEol());
	if(l.count())
		labelCurrentVersion->setText(l.at(0));

	Process->write("quit\r\n");
}

/*!
	\details Process Gdb correctly started 
*/
void UIGNUDebuggerSetting::started()
{}

/*!
	\details Process Gdb correctly finished
*/
void UIGNUDebuggerSetting::finished( int , QProcess::ExitStatus  )
{}

/*!
	\details Process crash
*/
void UIGNUDebuggerSetting::error( QProcess::ProcessError )
{
	labelCurrentVersion->setText(tr("Gdb not found"));
}

/*! 
	\details Apply button clicked
*/
void UIGNUDebuggerSetting::on_dbbButtons_clicked( QAbstractButton * button )
{
	if ( button == dbbButtons->button( QDialogButtonBox::Apply ) )
	{
		// here your code for apply/save
		// get the path
		GdbCore::Setting()->setPathGdb(editPathGdb->text());
		GdbCore::Setting()->setPathScript(editPathScript->text());

		// enable or not AddOn
		QList<QCheckBox*> cbs = groupPlugins->findChildren<QCheckBox*>();
		foreach(QCheckBox * c, cbs)
		{
			GdbCore::Setting()->setEnable( c->text() , (c->checkState () == Qt::Checked) ? true : false); 
		}
	}
}
