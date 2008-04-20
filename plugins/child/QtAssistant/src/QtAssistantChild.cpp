/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Child Plugins
** FileName  : QtAssistantChild.cpp
** Date      : 2008-01-14T00:57:10
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
#include "QtAssistantChild.h"

#include <coremanager.h>

// assistant include
#include "mainwindow.h"
#include "helpdialog.h"
#include "tabbedbrowser.h"
#include "helpwindow.h"

#include <QVBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>

QtAssistantChild::QtAssistantChild( QObject* )
	: pAbstractChild()
{
	// init main window
	mMain = new MainWindow;
	// modify some mMain widget
	mMain->menuBar()->hide();
	mMain->setIconSize( QSize( 16, 16 ) );
	mMain->statusBar()->hide();
	// get dock pointeur
	mDock = mMain->findChild<QDockWidget*>();
	// layout
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 0 );
	vl->setSpacing( 0 );
	// connection
	connect( mMain->helpDialog(), SIGNAL( showLink( const QString& ) ), this, SLOT( showLink( const QString& ) ) );
	connect( mMain->browsers()->findChild<QTabWidget*>( "tab" ), SIGNAL( currentChanged( int ) ), this, SLOT( currentChanged( int ) ) );
	connect( mMain->browsers()->currentBrowser(), SIGNAL( sourceChanged( const QUrl& ) ), this, SLOT( showLink( const QUrl& ) ) );
	// add assistant as widget child
	vl->addWidget( mMain );
	setWindowTitle( mMain->windowTitle() );
}

QtAssistantChild::~QtAssistantChild()
{ delete mDock; }

void QtAssistantChild::helpWindow_destroyed( QObject* )
{ emit currentFileChanged( currentFile() ); }

void QtAssistantChild::currentChanged( int i )
{
	// disconnect signals
	foreach ( HelpWindow* w, browsersList() )
	{
		disconnect( w, SIGNAL( sourceChanged( const QUrl& ) ), this, SLOT( showLink( const QUrl& ) ) );
		disconnect( w, SIGNAL( destroyed( QObject* ) ), this, SLOT( helpWindow_destroyed( QObject* ) ) );
		w->setAttribute( Qt::WA_DeleteOnClose );
	}
	
	//connect signal for current browser
	if ( HelpWindow* w = mMain->browsers()->currentBrowser() )
	{
		connect( w, SIGNAL( sourceChanged( const QUrl& ) ), this, SLOT( showLink( const QUrl& ) ) );
		connect( w, SIGNAL( destroyed( QObject* ) ), this, SLOT( helpWindow_destroyed( QObject* ) ) );
	}
	
	// set title
	QString c = mMain->browsers()->findChild<QTabWidget*>( "tab" )->tabText( i );
	setWindowTitle( tr( "Qt Assistant" ).append( c.isNull() ? QString::null : c.prepend( " [" ).append( "]" ) ) );
	setWindowModified( false );
}

void QtAssistantChild::showLink( const QString& s )
{
	// get workspace
	pWorkspace* w = MonkeyCore::workspace();
	
	// check if child is in workspace
	if ( w->indexOf( this ) != -1 )
		w->setCurrentDocument( this );
	else
	{
		w->addDocument( this, tr( "Qt Assistant" ) );
		setAttribute( Qt::WA_DeleteOnClose, false );
		if ( !isVisible() )
			setVisible( true );
	}
	
	// set title
	currentChanged( mMain->browsers()->findChild<QTabWidget*>( "tab" )->currentIndex() );
	
	// tell filename has changed
	emit currentFileChanged( !s.isEmpty() ? s : currentFile() );
}

void QtAssistantChild::showLink( const QUrl& u )
{ showLink( u.toString() ); }

QList<HelpWindow*> QtAssistantChild::browsersList() const
{ return mMain->browsers()->findChild<QTabWidget*>( "tab" )->findChildren<HelpWindow*>(); }

QStringList QtAssistantChild::files() const
{
	QStringList l;
	foreach ( HelpWindow* w, browsersList() )
		l << w->source().toString();
	return l;
}

QPoint QtAssistantChild::cursorPosition() const
{ return QPoint( -1, -1 ); }

void QtAssistantChild::showFile( const QString& s )
{
	foreach ( HelpWindow* w, browsersList() )
	{
		if ( w->source().toString() == s )
		{
			mMain->browsers()->findChild<QTabWidget*>( "tab" )->setCurrentWidget( w );
			return;
		}
	}
}

QString QtAssistantChild::currentFile() const
{ return mMain->browsers()->currentBrowser()->source().toString(); }

QString QtAssistantChild::currentFileName() const
{ return QFileInfo( currentFile() ).fileName(); }

bool QtAssistantChild::isModified() const
{ return false; }

bool QtAssistantChild::isUndoAvailable() const
{ return false; }

bool QtAssistantChild::isRedoAvailable() const
{ return false; }

bool QtAssistantChild::isPasteAvailable() const
{ return false; }

bool QtAssistantChild::isCopyAvailable() const
{ return false; }

bool QtAssistantChild::isModified( const QString& ) const
{ return false; }

void QtAssistantChild::saveFile( const QString& ) {}

void QtAssistantChild::saveFiles() {}

void QtAssistantChild::printFile( const QString& ) {}

void QtAssistantChild::quickPrintFile( const QString& ) {}

void QtAssistantChild::undo() {}

void QtAssistantChild::redo() {}

void QtAssistantChild::cut() {}

void QtAssistantChild::copy() {}

void QtAssistantChild::paste() {}

void QtAssistantChild::searchReplace() {}

void QtAssistantChild::goTo() {}

bool QtAssistantChild::isSearchReplaceAvailable() const
{ return false; }

bool QtAssistantChild::isGoToAvailable() const
{ return false; }

bool QtAssistantChild::isPrintAvailable() const
{ return false; }

MainWindow* QtAssistantChild::main()
{ return mMain; }

QDockWidget* QtAssistantChild::dock()
{ return mDock; }
