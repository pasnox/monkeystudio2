/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pToolsManager.cpp
** Date      : 2008-01-14T00:37:18
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
#include "pToolsManager.h"
#include "ui/UIToolsEdit.h"
#include "ui/UIDesktopTools.h"
#include "../pMonkeyStudio.h"
#include "../coremanager/MonkeyCore.h"
#include "../settingsmanager/Settings.h"
#include "../consolemanager/pConsoleManager.h"
#include "../queuedstatusbar/QueuedStatusBar.h"

#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QTimer>

using namespace pMonkeyStudio;

QFileIconProvider* pToolsManager::mIconProvider = 0L;

pToolsManager::pToolsManager( QObject* p )
	: QObject( p )
{
	// initialise action
	initializeTools();
	// action connection
	connect( MonkeyCore::menuBar()->action( "mTools/aEditUser" ), SIGNAL( triggered() ), this, SLOT( editTools_triggered() ) );
	connect( MonkeyCore::menuBar()->action( "mTools/aEditDesktop" ), SIGNAL( triggered() ), this, SLOT( editTools_triggered() ) );
	connect( MonkeyCore::menuBar()->menu( "mTools/mUserTools" ), SIGNAL( triggered( QAction* ) ), this, SLOT( toolsMenu_triggered( QAction* ) ) );
	connect( MonkeyCore::menuBar()->menu( "mTools/mDesktopTools" ), SIGNAL( triggered( QAction* ) ), this, SLOT( toolsMenu_triggered( QAction* ) ) );
}

pToolsManager::~pToolsManager()
{ delete mIconProvider; }

const QFileIconProvider* pToolsManager::iconProvider()
{ return mIconProvider ? mIconProvider : ( mIconProvider = new QFileIconProvider() ); }

const QList<pTool> pToolsManager::tools( ToolType t )
{
	// tools list
	QList<pTool> l;
	// got settings
	pSettings* s = MonkeyCore::settings();
	// create action
	int n = s->beginReadArray( "Tools" );
	for ( int i = 0; i < n; i++ )
	{
		s->setArrayIndex( i );
		if ( t == ttAll || ( t == ttUserEntry && !s->value( "DesktopEntry" ).toBool() ) || t == ttDesktopEntry && s->value( "DesktopEntry" ).toBool() )
			l << pTool( s->value( "Caption" ).toString(), s->value( "FileIcon" ).toString(), s->value( "FilePath" ).toString(), s->value( "WorkingPath" ).toString(), s->value( "DesktopEntry" ).toBool(), s->value( "UseConsoleManager" ).toBool() );
	}
	s->endArray();
	// return list
	return l;
}

void pToolsManager::initializeTools()
{
	// get menu bar
	pMenuBar* mb = MonkeyCore::menuBar();
	// clear action
	mb->menu( "mTools/mUserTools" )->clear();
	mb->menu( "mTools/mDesktopTools" )->clear();
	// initialize tools
	foreach ( pTool t, tools() )
	{
		QAction* ac;
		if ( t.DesktopEntry )
			ac = mb->action( QString( "mTools/mDesktopTools/%1" ).arg( t.Caption ), t.Caption, QIcon( t.FileIcon ), QString::null, t.toString() );
		else
			ac = mb->action( QString( "mTools/mUserTools/%1" ).arg( t.Caption ), t.Caption, QIcon( t.FileIcon ), QString::null, t.toString() );
		if ( ac->icon().isNull() )
			ac->setIcon( iconProvider()->icon( t.FilePath ) );
		ac->setStatusTip( t.FilePath );
		ac->setData( QVariant::fromValue<pTool>( t ) );
	}
}

void pToolsManager::editTools_triggered()
{
	QAction* a = qobject_cast<QAction*>( sender() );
	if ( ( a == MonkeyCore::menuBar()->action( "mTools/aEditUser" ) ? UIToolsEdit::instance()->exec() : UIDesktopTools::instance()->exec() ) )
		initializeTools();
}

void pToolsManager::toolsMenu_triggered( QAction* a )
{
	pConsoleManager* cm = MonkeyCore::consoleManager();
	const pTool tool = a->data().value<pTool>();
	QString filePath = cm->processInternalVariables( tool.FilePath );
	QString workingPath = cm->processInternalVariables( tool.WorkingPath );
	bool b = false;
	if ( filePath.isEmpty() )
		b = false;
	else if ( tool.UseConsoleManager )
	{
		pCommand cmd;
		cmd.setText( tool.Caption );
		cmd.setCommand( tool.FilePath );
		cmd.setWorkingDirectory( tool.WorkingPath );
		cm->addCommand( cmd );
		b = true;
	}
	else if ( workingPath.isEmpty() && QFile::exists( filePath ) )
		b = QDesktopServices::openUrl( QUrl::fromLocalFile( filePath ) );
	else if ( workingPath.isEmpty() )
		b = QProcess::startDetached( filePath );
	else
	{
		QProcess* p = new QProcess( this );
		connect( p, SIGNAL( finished( int ) ), p, SLOT( deleteLater() ) );
		p->setWorkingDirectory( workingPath );
		p->start( filePath );
		b = p->waitForStarted();
	}
	if ( !b )
		MonkeyCore::statusBar()->appendMessage( tr( "Error trying to start tool :\n'%1'" ).arg( filePath ), 2500 );
}
