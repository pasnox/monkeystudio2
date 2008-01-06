#include "pRecentsManager.h"
#include "MonkeyCore.h"
#include "pSettings.h"
#include "pMenuBar.h"

#include <QAction>
#include <QFileInfo>

pRecentsManager::pRecentsManager( QObject* p )
	: QObject( p )
{
	// set maximum
	setMaxRecentFiles( maxRecentFiles() );
	setMaxRecentProjects( maxRecentProjects() );

	// update actions
	updateRecentFiles();
	updateRecentProjects();

	// connections
	connect( MonkeyCore::menuBar()->menu( "mFile/mRecents" ), SIGNAL( triggered( QAction* ) ), this, SLOT( recentFiles_triggered( QAction* ) ) );
	connect( MonkeyCore::menuBar()->menu( "mProject/mRecents" ), SIGNAL( triggered( QAction* ) ), this, SLOT( recentProjects_triggered( QAction* ) ) );
}

int pRecentsManager::maxRecentFiles() const
{ return pSettings::value( "Recents/MaxFiles", 15 ).toInt(); }

int pRecentsManager::maxRecentProjects() const
{ return pSettings::value( "Recents/MaxProjects", 15 ).toInt(); }

QString pRecentsManager::recentFileOpenPath() const
{ return pSettings::value( "Recents/FileOpenPath" ).toString(); }

QString pRecentsManager::recentProjectOpenPath() const
{ return pSettings::value( "Recents/ProjectOpenPath" ).toString(); }

void pRecentsManager::setMaxRecentFiles( int i )
{
	if ( i != pSettings::value( "Recents/MaxFiles" ).toInt() )
	{
		pSettings::setValue( "Recents/MaxFiles", i );
		updateRecentFiles();
	}
}

void pRecentsManager::setMaxRecentProjects( int i )
{
	if ( i != pSettings::value( "Recents/MaxProjects" ).toInt() )
	{
		pSettings::setValue( "Recents/MaxProjects", i );
		updateRecentProjects();
	}
}

void pRecentsManager::setRecentFileOpenPath( const QString& s )
{ pSettings::setValue( "Recents/FileOpenPath", s ); }

void pRecentsManager::setRecentProjectOpenPath( const QString& s )
{ pSettings::setValue( "Recents/ProjectOpenPath", s ); }

void pRecentsManager::recentFiles_triggered( QAction* a )
{
	if ( a->text() != tr( "&Clear" ) )
		emit openFileRequested( a->data().toString() );
	else if ( a->text() == tr( "&Clear" ) )
	{
		pSettings::setValue( "Recents/Files", QStringList() );
		updateRecentFiles();
	}
}

void pRecentsManager::updateRecentFiles()
{
	// clears actions
	foreach ( QAction* a, mRecentFiles )
		a->deleteLater();
	mRecentFiles.clear();
	// get recents files
	QStringList l = pSettings::value( "Recents/Files" ).toStringList();
	for ( int i = 0; i < maxRecentFiles(); i++ )
	{
		if ( i < l.count() )
		{
			QFileInfo f( l.at( i ).simplified() );
			if ( f.exists() )
			{
				// create action
				QString s = QString( "%1 %2" ).arg( i +1 ).arg( f.fileName() );
				QAction* a = new QAction( s, this );
				a->setData( l.at( i ) );
				a->setStatusTip( l.at( i ) );
				mRecentFiles.append( a );
				// add action
				MonkeyCore::menuBar()->menu( "mFile/mRecents" )->addAction( a );
			}
		}
	}
}

void pRecentsManager::addRecentFile( const QString& s )
{
	// get recents files
	QStringList f = pSettings::value( "Recents/Files" ).toStringList();
	// remove s and prepend it
	f.removeAll( s );
	f.prepend( s );
	// remove files > maxrecentfiles
	while ( f.size() > maxRecentFiles() )
		f.removeLast();
	// store recents files
	pSettings::setValue( "Recents/Files", f );
	// update menu
	updateRecentFiles();
}

void pRecentsManager::removeRecentFile( const QString& s )
{
	// get recents files
	QStringList f = pSettings::value( "Recents/Files" ).toStringList();
	// remove s
	f.removeAll( s );
	// remove files > maxrecent files
	while ( f.size() > maxRecentFiles() )
		f.removeLast();
	// store recents files
	pSettings::setValue( "Recents/Files", f );
	// update menu
	updateRecentFiles();
}

void pRecentsManager::recentProjects_triggered( QAction* a )
{
	if ( a->text() != tr( "&Clear" ) )
		emit openProjectRequested( a->data().toString() );
	else if ( a->text() == tr( "&Clear" ) )
	{
		pSettings::setValue( "Recents/Projects", QStringList() );
		updateRecentProjects();
	}
}

void pRecentsManager::updateRecentProjects()
{
	// clear actions
	foreach ( QAction* a, mRecentProjects )
		a->deleteLater();
	mRecentProjects.clear();
	// get recents projects
	QStringList l = pSettings::value( "Recents/Projects" ).toStringList();
	for ( int i = 0; i < maxRecentProjects(); i++ )
	{
		if ( i < l.count() )
		{
			QFileInfo f( l.at( i ).simplified() );
			if ( f.exists() )
			{
				// create action
				QString s = QString( "%1 %2" ).arg( i +1 ).arg( f.fileName() );
				QAction* a = new QAction( s, this );
				a->setData( l.at( i ) );
				a->setStatusTip( l.at( i ) );
				mRecentProjects.append( a );
				// add action
				MonkeyCore::menuBar()->menu( "mProject/mRecents" )->addAction( a );
			}
		}
	}
}

void pRecentsManager::addRecentProject( const QString& s )
{
	// get recent proejcts
	QStringList f = pSettings::value( "Recents/Projects" ).toStringList();
	// remove s and prepend it
	f.removeAll( s );
	f.prepend( s );
	// remvoe proejcts > maxrecentprojects
	while ( f.size() > maxRecentProjects() )
		f.removeLast();
	// store recents projects
	pSettings::setValue( "Recents/Projects", f );
	// update menu
	updateRecentProjects();
}

void pRecentsManager::removeRecentProject( const QString& s )
{
	// get recents projects
	QStringList f = pSettings::value( "Recents/Projects" ).toStringList();
	// remove s
	f.removeAll( s );
	// remove files > maxrecentsproejcts
	while ( f.size() > maxRecentProjects() )
		f.removeLast();
	// store recents proejcts
	pSettings::setValue( "Recents/Projects", f );
	// update menu
	updateRecentProjects();
}
