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
	\file DockGNUDebugger.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class is a main class container. 
*/

#include "DockGNUDebugger.h"
#include <Settings.h>
#include <MonkeyCore.h>
#include <UIMain.h>
#include <pEditor.h>
#include "pMonkeyStudio.h"
#include <QueuedStatusBar.h>

#include <QFileDialog>

#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>

#include <QtDebug>



void DockGNUDebugger::projectOpened( XUPProjectItem* project )
{
	if(project)
	{
		QStringList l =   project->projectSettingsValues( "RESTORE_BREAKPOINTS" , QStringList() );
		emit setBreakpointOpened(l);
	}
}


void DockGNUDebugger::saveBreakpoint(QStringList l)
{
	XUPProjectItem *p = MonkeyCore::projectsManager()->currentProject();
	if(p )
	{
		p->setProjectSettingsValues("RESTORE_BREAKPOINTS" ,l);
		p->save();
	}
}

/*!
* \details Load current setting from Monkey .ini
*/
void DockGNUDebugger::loadSettings()
{
	Settings* s = MonkeyCore::settings();
	s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );
		GdbCore::Setting()->setPathGdb( s->value("PathGdb", "gdb").toString());
		GdbCore::Setting()->setPathScript(  s->value("PathScript", "./" ).toString());

	QStringList l = s->allKeys();
	foreach(QString k ,l)
	{
		if(k.startsWith("AddOn/"))
		{	
			bool e = s->value(k, true ).toBool();
			GdbCore::Setting()->setAddonsAvailable( k.remove("AddOn/"),  e);
		}
	}
	s->endGroup();
}

/*!
* \details Save current setting from Monkey .ini
*/
void DockGNUDebugger::saveSettings()
{
	Settings * s = MonkeyCore::settings();
		s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );
		s->setValue( "PathGdb", GdbCore::Setting()->getPathGdb());
		s->setValue( "PathScript", GdbCore::Setting()->getPathScript() );

	QHashIterator<QString, bool> i(GdbCore::Setting()->getAddonsAvailable());
	while (i.hasNext()) 
	{
		i.next();
		s->setValue( "AddOn/" + i.key(), i.value() ? true : false);
	}	
	s->endGroup();
}

/*!
* \details Create a new object.
*/
DockGNUDebugger::DockGNUDebugger( QWidget * w )
	: pDockWidget( w )

{

	// init resource (lib gdb is static)
	Q_INIT_RESOURCE(gdb_res);

	// create main container
	mainTabWidget = new QTabWidget(this);
	setWidget(mainTabWidget);

	// create rawLog
	rawLog = new QTextEdit(mainTabWidget);
	mainTabWidget->addTab(rawLog,"Raw Log");

	// init libGdb
	GdbCore::init(this);
	// get current setting from Mks ini file
	loadSettings();

	// create addon (auto add in kernelDispatcher)
	new GdbBreakpoint(this);
	new GdbBacktrace(this);
	new GdbRegister(this);
	new GdbToolTip(this);

	// create bridge Qsci
	new BridgeQsci(this);

	/* just view log */
	// Process
	connect(GdbCore::Process(), SIGNAL( commandReadyRead( const QString& )), this , SLOT( commandReadyRead( const QString& )));
	// Parser
	connect(GdbCore::Parser(), SIGNAL(done(int, QString)), this , SLOT(onDone(int, QString)));
	connect(GdbCore::Parser(), SIGNAL(prompt(int, QString)), this , SLOT(onPrompt(int, QString)));

	/* for view message from addon in statusbar */
	connect(GdbCore::KernelDispatcher(),SIGNAL(errorMessage(QString,int)), this , SLOT(onErrorMessage(QString, int)));
	connect(GdbCore::KernelDispatcher(),SIGNAL(defaultMessage(QString)), this , SLOT(onDefaultMessage(QString)));
	connect(GdbCore::Controler(),SIGNAL(errorMessage(QString,int)), this , SLOT(onErrorMessage(QString, int)));
	connect(GdbCore::Controler(),SIGNAL(defaultMessage(QString)), this , SLOT(onDefaultMessage(QString)));

	/* add button for remote gdb in toolBar */
	mActionList = GdbCore::Controler()->getActionsAvailable();
	foreach ( QAction* a, mActionList )
		MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addAction( a );

	/* add menu debugger */
	mMenu = MonkeyCore::menuBar()->menu("mGdb", tr("Debugger"));
	if(mMenu) foreach(QAction *a, mActionList) mMenu->addAction(a);

	/* add addOn in QTabWidget */
	foreach(QPointer< class GdbAddonBase> r, GdbCore::KernelDispatcher()->getAddonslist())
	{	// find if addOn is enable ?
		if(r) // make sure r is valid
		{
			r->setEnabled( GdbCore::Setting()->getStartUp( r->name() ));
			if(r->isEnabled() && r->widget()) 
				  mainTabWidget->addTab( r->widget(),r->icon(), r->name() );
		}
		else qDebug() << "Error Debugger plugin in GNDebugger Addon ptr = NULL";
	}
	// set Eof \r\n or \n
	QString s =  pMonkeyStudio::getEol();
	GdbCore::Process()->setEof(s);
	GdbCore::Parser()->setEof(s);

	// for restorering breakpoint
	connect (MonkeyCore::projectsManager(), SIGNAL(projectOpened( XUPProjectItem*  )) ,SLOT(projectOpened( XUPProjectItem*  )));
	connect (this, SIGNAL(setBreakpointOpened(QStringList)), GdbCore::KernelDispatcher()->findAddon("GdbBreakpoint"), SLOT(restoreBreakpoint(QStringList)));
	connect (GdbCore::KernelDispatcher()->findAddon("GdbBreakpoint"), SIGNAL(saveBreakpoint(QStringList)), SLOT(saveBreakpoint(QStringList)));
}

/*!
* \details Delete main container
*/
DockGNUDebugger:: ~DockGNUDebugger()
{
	saveSettings();
	// delete all addons instance because Kernel is static
	GdbCore::KernelDispatcher()->clear();
	//delete all static class
	GdbCore::clear();
	// delete menu
	MonkeyCore::menuBar()->deleteMenu("mGdb");
}
 

void DockGNUDebugger::onDefaultMessage(QString s)
{
	MonkeyCore::statusBar()->setMessage( s);
}


void DockGNUDebugger::onErrorMessage(QString s, int t)
{
	MonkeyCore::statusBar()->appendMessage( s, t);
}


/*!
* \details Calling when Gdb has an answer or Gdb has new datas, after it call GdbParser for parse this string.
* \param d is raw data from Gdb
*/
void DockGNUDebugger::commandReadyRead(  const QString& d)
{
	rawLog->setTextColor(QColor(0,0,255));
	rawLog->append( d );
	rawLog->setTextColor(QColor(0,0,0));
}



// Target

/*!
* \details Target is loaded correctly. Dispatch now this event to all AddOn
* \param id is an Id of string
* \param st is the string.
*/
void DockGNUDebugger::onTargetLoaded(int id, QString st)
{
	Q_UNUSED (st);
	Q_UNUSED (id);
}

/*!
* \details Target is no loaded correctly (format is not correct). Dispatch this event to all AddOn.
* \param id is an Id of string
* \param st is the string.
*/
void DockGNUDebugger::onTargetNoLoaded(int id, QString st)
{
	Q_UNUSED (st);
	Q_UNUSED (id);
}

//

/*!
* \details Target is running. Dispatch this event to all AddOn.
* \param id is an Id of string
* \param st is the string.
*/

void DockGNUDebugger::onTargetRunning(int id, QString st)
{
	Q_UNUSED (st);
	Q_UNUSED (id);
}

/*!
* \details Target is stopped (breakpoint for example). Dispatch this event to all AddOn.
* \param id is an Id of string
* \param st is the string.
*/
void DockGNUDebugger::onTargetStopped(int id, QString st)
{
	Q_UNUSED (st);
	Q_UNUSED (id);
}

	
/*!
* \details Target is exited correctly. Dispatch this event to all AddOn.
* \param id is an Id of string
* \param st is the string.
*/
void DockGNUDebugger::onTargetExited(int id, QString st)
{
	Q_UNUSED (st);
	Q_UNUSED (id);
}



/*!
* \details Info event.	Calling when GdbParser emit info signal.
* \param id is an Id of string
* \param st is the Info string.
*/
void DockGNUDebugger::onDone(int id, QString st)
{
	rawLog->setTextColor(QColor(255,255,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));
}

/*!
* \details Prompt event. Calling when GdbParser emit prompt signal.
* \param id egal 0
* \param st is the Prompt string.
*/
void DockGNUDebugger::onPrompt(int id, QString st)
{
//	disableStep = false;
	rawLog->setTextColor(QColor(255,0,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));
}



