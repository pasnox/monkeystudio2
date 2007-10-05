#include "pFileManager.h"
#include "pWorkspace.h"
#include "UIProjectsManager.h"

pFileManager::pFileManager( QObject* o )
	: QObject( o )
{
	// files
	connect( pWorkspace::instance(), SIGNAL( fileOpened( const QString& ) ), this, SIGNAL( fileOpened( const QString& ) ) );
	connect( pWorkspace::instance(), SIGNAL( fileClosed( const QString& ) ), this, SIGNAL( fileClosed( const QString& ) ) );
	connect( pWorkspace::instance(), SIGNAL( currentFileChanged( const QString& ) ), this, SIGNAL( currentFileChanged( const QString& ) ) );
	// projects
	connect( UIProjectsManager::instance(), SIGNAL( aboutToClose( ProjectItem* ) ), this, SIGNAL( aboutToClose( ProjectItem* ) ) );
	connect( UIProjectsManager::instance(), SIGNAL( closed( ProjectItem* ) ), this, SIGNAL( closed( ProjectItem* ) ) );
	connect( UIProjectsManager::instance(), SIGNAL( modifiedChanged( ProjectItem*, bool ) ), this, SIGNAL( modifiedChanged( ProjectItem*, bool ) ) );
	connect( UIProjectsManager::instance(), SIGNAL( currentChanged( ProjectItem* ) ), this, SIGNAL( currentChanged( ProjectItem* ) ) );
	connect( UIProjectsManager::instance(), SIGNAL( opened( ProjectItem* ) ), this, SIGNAL( opened( ProjectItem* ) ) );
}

pAbstractChild* pFileManager::openFile( const QString& s, const QPoint& p )
{ return pWorkspace::instance()->openFile( s, p ); }

void pFileManager::closeFile( const QString& s )
{ pWorkspace::instance()->closeFile( s ); }

void pFileManager::goToLine( const QString& s, const QPoint& p, bool b )
{ pWorkspace::instance()->goToLine( s, p, b ); }

void pFileManager::openProject( const QString& s )
{ UIProjectsManager::instance()->openProject( s ); }

void pFileManager::closeProject( const QString& s )
{ UIProjectsManager::instance()->closeProject( s ); }
