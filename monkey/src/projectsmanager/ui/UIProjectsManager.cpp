/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UIProjectsManager.cpp
** Date      : 2008-01-14T00:37:02
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

#include "UIProjectsManager.h"
#include "pMenuBar.h"
#include "PluginsManager.h"
#include "pMonkeyStudio.h"
#include "pRecentsManager.h"
#include "ProjectsProxy.h"
#include "ProjectsModel.h"
#include "pFileManager.h"
#include "UITemplatesWizard.h"
#include "UIAddExistingFiles.h"
#include "MonkeyCore.h"

#include <QHeaderView>

using namespace pMonkeyStudio;

UIProjectsManager::UIProjectsManager( QWidget* w )
	: QDockWidget( w ), mProjects( new ProjectsModel( this ) ), mProxy( new ProjectsProxy( mProjects ) )
{
	setupUi( this );
	
	// set icons size
	tbButtons->setIconSize( QSize( 16, 16 ) );
	
	// set layout margin & spacing
	tbButtons->layout()->setMargin( 0 );
	tbButtons->layout()->setSpacing( 0 );
	
	// get menubar
	pMenuBar* mb = MonkeyCore::menuBar();
	// set buttons action
	tbButtons->addAction( mb->action( "mView/aFiltered" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/aNew" ) );
	tbButtons->addAction( mb->action( "mProject/aOpen" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/mSave/aCurrent" ) );
	tbButtons->addAction( mb->action( "mProject/mSave/aAll" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/mClose/aCurrent" ) );
	tbButtons->addAction( mb->action( "mProject/mClose/aAll" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/aSettings" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/aAddExistingFiles" ) );
	tbButtons->addAction( mb->action( "mProject/aRemove" ) );
	
	// set treeview view for combo list
	cbProjects->setModel( mProjects->projectsProxy() );
	
	// set projects properties
	tvProjects->header()->hide();
	
	// set proxy properties
	// set types to filter
	mProxy->setFilterRoles( QList<int>() << ProjectItem::ProjectType << ProjectItem::ValueType );
	// filter are negate
	mProxy->setNegateFilter( false );
	// apply filtering
	mProxy->setFiltering( true );
	// set view proxy
	tvProjects->setModel( mProxy );
	
	// set filter filtering state
	mb->action( "mView/aFiltered" )->setChecked( mProxy->isFiltering() );
	
	// connections
	connect( mProxy, SIGNAL( filteringChanged( bool ) ), mb->action( "mView/aFiltered" ), SLOT( setChecked( bool ) ) );
	connect( cbProjects, SIGNAL( activated( const QModelIndex& ) ), this, SLOT( cbProjects_activated( const QModelIndex& ) ) );
	connect( cbProjects, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( cbProjects_activated( const QModelIndex& ) ) );
	connect( tvProjects->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( tvProjects_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
}

ProjectItem* UIProjectsManager::currentItem() const
{ return mProjects->itemFromIndex( mProxy->mapToSource( tvProjects->currentIndex() ) ); }

ProjectItem* UIProjectsManager::currentProject() const
{
	// get current item
	ProjectItem* p = currentItem();
	// return project item
	return p ? p->project() : 0;
}

void UIProjectsManager::setCurrentProject( ProjectItem* p )
{ tvProjects->setCurrentIndex( p ? mProxy->mapFromSource( p->index() ) : QModelIndex() ); }

ProjectItemList UIProjectsManager::rootProjects() const
{ return mProjects->projects( false ); }

void UIProjectsManager::initializeProject( ProjectItem* it )
{
	// cancel if no item
	if ( !it )
		return;
	// clear selected item
	tvProjects->selectionModel()->clear();
	// append project item
	mProjects->appendRow( it );
	// set current project
	tvProjects->setCurrentIndex( mProxy->mapFromSource( it->index() ) );
	// connections
	foreach ( ProjectItem* p, ProjectItemList() << it << it->childrenProjects() )
	{
		connect( p, SIGNAL( aboutToClose() ), this, SLOT( internal_aboutToClose() ) );
		connect( p, SIGNAL( closed() ), this, SLOT( internal_closed() ) );
		connect( p, SIGNAL( modifiedChanged( bool ) ), this, SLOT( internal_modifiedChanged( bool ) ) );
		// emit opend signal
		emit opened( p );
	}
}

void UIProjectsManager::cbProjects_activated( const QModelIndex& i )
{ tvProjects->setCurrentIndex( mProxy->mapFromSource( mProjects->projectsProxy()->mapToSource( i ) ) ); }

void UIProjectsManager::tvProjects_currentChanged( const QModelIndex& c, const QModelIndex& p )
{
	// removing old project commands is needed
	ProjectItem* op = p.isValid() ? mProjects->itemFromIndex( mProxy->mapToSource( p ) ) : 0;
	if ( op )
		op->uninstallCommands();
	// get menubar
	pMenuBar* mb = MonkeyCore::menuBar();
	// get pluginsmanager
	PluginsManager* pm = MonkeyCore::pluginsManager();
	// get current project
	ProjectItem* cit = c.isValid() ? mProjects->itemFromIndex( mProxy->mapToSource( c ) ) : 0;
	ProjectItem* it = currentProject();
	// set compiler, debugger and interpreter
	pm->setCurrentBuilder( it ? it->builder() : 0 );
	pm->setCurrentCompiler( it ? it->compiler() : 0 );
	pm->setCurrentDebugger( it ? it->debugger() : 0 );
	pm->setCurrentInterpreter( it ? it->interpreter() : 0 );
	// anstall project self own commands
	if ( it )
		it->installCommands();
	// desactive project action
	mb->action( "mProject/mSave/aCurrent" )->setEnabled( it ? it->getModified() : false );
	mb->action( "mProject/mSave/aAll" )->setEnabled( it );
	mb->action( "mProject/mClose/aCurrent" )->setEnabled( it );
	mb->action( "mProject/mClose/aAll" )->setEnabled( it );
	mb->action( "mProject/aSettings" )->setEnabled( it );
	mb->action( "mProject/aAddExistingFiles" )->setEnabled( it );
	mb->action( "mProject/aRemove" )->setEnabled( cit && ( cit->getType() == ProjectItem::VariableType || cit->getType() == ProjectItem::ValueType ) );
	// select correct project in combobox
	cbProjects->setCurrentIndex( it ? mProjects->projectsProxy()->mapFromSource( it->index() ) : QModelIndex() );
	// emit currentChanged
	emit currentChanged( it );
}

void UIProjectsManager::on_tvProjects_doubleClicked( const QModelIndex& i )
{
	// get item
	ProjectItem* it = mProjects->itemFromIndex( mProxy->mapToSource( i ) );
	// if item and file exists, open it
	if ( it && QFile::exists( it->getFilePath() ) )
		MonkeyCore::fileManager()->openFile( it->getFilePath() );
}

void UIProjectsManager::on_tvProjects_customContextMenuRequested( const QPoint& p )
{
	if ( currentProject() )
	{
		// get menubar
		pMenuBar* mb = MonkeyCore::menuBar();
		
		// create menu
		QMenu m( this );
		
		// add menu commands
		m.addActions( mb->menu( "mProject" )->actions() );
		m.addSeparator();
		m.addActions( mb->menu( "mBuilder" )->actions() );
		m.addSeparator();
		m.addActions( mb->menu( "mDebugger" )->actions() );
		m.addSeparator();
		m.addActions( mb->menu( "mInterpreter" )->actions() );
		
		// show menu
		m.exec( tvProjects->mapToGlobal( p ) );
	}
}

void UIProjectsManager::internal_aboutToClose()
{ emit aboutToClose( qobject_cast<ProjectItem*>( sender() ) ); }

void UIProjectsManager::internal_closed()
{ emit closed( qobject_cast<ProjectItem*>( sender() ) ); }

void UIProjectsManager::internal_modifiedChanged( bool b )
{
	ProjectItem* p = qobject_cast<ProjectItem*>( sender() );
	if ( p && currentProject() == p )
		MonkeyCore::menuBar()->action( "mProject/mSave/aCurrent" )->setEnabled( p->getModified() );
	emit modifiedChanged( p, b );
}

bool UIProjectsManager::openProject( const QString& s )
{
	// look if project is already opened
	foreach ( ProjectItem* p, mProjects->projects( true ) )
	{
		if ( isSameFile( p->getValue(), s ) )
		{
			setCurrentProject( p );
			return true;
		}
	}
	// open project
	if ( ProjectItem* it = MonkeyCore::pluginsManager()->projectItem( s ) )
	{
		if ( it->open() )
		{
			initializeProject( it );
			return true;
		}
		else
			delete it;
	}
	else
		warning( tr( "Open Project..." ), tr( "There is no plugin that can manage this project :\n[%1]" ).arg( s ) );
	return false;
}

bool UIProjectsManager::closeProject( const QString& s )
{
	foreach ( ProjectItem* p, mProjects->projects( true ) )
	{
		if ( isSameFile( p->getValue(), s ) )
		{
			p->close();
			return true;
		}
	}
	return false;
}

void UIProjectsManager::projectNew_triggered()
{
	UITemplatesWizard* d = UITemplatesWizard::instance( this );
	d->setType( tr( "Projects" ) );
	d->exec();
}

void UIProjectsManager::projectOpen_triggered()
{
	// get last file open path
	const QString mPath = MonkeyCore::recentsManager()->recentProjectOpenPath();
	// get available filters
	QString mFilters = availableProjectsFilters();
	// prepend a all in one filter
	if ( !mFilters.isEmpty() )
	{
		QString s;
		foreach ( QStringList l, availableProjectsSuffixes().values() )
			s.append( l.join( " " ).append( " " ) );
		mFilters.prepend( QString( "All Supported Projects (%1);;" ).arg( s.trimmed() ) );
	}
	// open open file dialog
	QStringList l = getOpenFileNames( tr( "Choose the project(s) to open" ), mPath, mFilters, window() );
	// for each entry, open file
	foreach ( QString s, l )
	{
		if ( openProject( s ) )
			// append file to recents
			MonkeyCore::recentsManager()->addRecentProject( s );
		else
			// remove it from recents files
			MonkeyCore::recentsManager()->removeRecentProject( s );
	}
	// store file open path
	if ( !l.isEmpty() )
		MonkeyCore::recentsManager()->setRecentProjectOpenPath( QFileInfo( l.at( 0 ) ).path() );
}

void UIProjectsManager::projectSaveCurrent_triggered()
{
	ProjectItem* p = currentProject();
	if ( p )
		p->save( false );
}

void UIProjectsManager::projectSaveAll_triggered()
{
	foreach ( ProjectItem* p, mProjects->projects( false ) )
		p->saveAll( false );
}

void UIProjectsManager::projectCloseCurrent_triggered()
{
	if ( ProjectItem* p = currentProject() )
	{
		setCurrentProject( p );
		p->close();
	}
}

void UIProjectsManager::projectCloseAll_triggered()
{
	foreach ( ProjectItem* p, mProjects->projects( false ) )
	{
		setCurrentProject( p );
		p->close();
	}
}

void UIProjectsManager::projectSettings_triggered()
{
	if ( ProjectItem* it = currentProject() )
		it->editSettings();
}

void UIProjectsManager::projectAddExistingFiles_triggered()
{
	if ( ProjectItem* it = currentProject() )
	{
		UIAddExistingFiles d( mProjects->scopesProxy(), it, availableOperators(), this );
		if ( d.exec() && !d.selectedFiles().isEmpty() )
			d.currentItem()->project()->addExistingFiles( d.selectedFiles(), d.currentItem(), d.currentOperator() );
	}
}

void UIProjectsManager::projectRemove_triggered()
{
	// create dialog
	QMessageBox mb( QMessageBox::Question, tr( "Remove Item" ), tr( "Really remove item ?" ), QMessageBox::NoButton, window() );
	// add buttons
	mb.addButton( tr( "Remove Item" ), QMessageBox::AcceptRole );
	QPushButton* rd = mb.addButton( tr( "Remove Item && Delete Associated File" ), QMessageBox::AcceptRole );
	QPushButton* c = mb.addButton( QMessageBox::Cancel );
	// execute dialog
	mb.exec();
	// get button clicked
	QAbstractButton* b = mb.clickedButton();
	// do job
	if ( b && b != c )
	{
		// get item
		if ( ProjectItem* it = mProjects->itemFromIndex( mProxy->mapToSource( tvProjects->currentIndex() ) ) )
		{
			// get item associate file
			QFileInfo fi( it->getFilePath() );
			// remove item
			it->remove();
			// delete file if needed
			if ( b == rd && fi.exists() && fi.isFile() )
				if ( !QFile::remove( fi.absoluteFilePath() ) )
					warning( tr( "Delete File..." ), tr( "Can't delete file:\n%1" ).arg( fi.absoluteFilePath() ), window() );
		}
	}
}
