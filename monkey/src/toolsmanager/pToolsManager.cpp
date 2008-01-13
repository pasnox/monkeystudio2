#include "pToolsManager.h"
#include "pSettings.h"
#include "pMenuBar.h"
#include "UIToolsEdit.h"
#include "UIDesktopTools.h"
#include "pMonkeyStudio.h"
#include "pConsoleManager.h"
#include "MonkeyCore.h"

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
			l << pTool( s->value( "Caption" ).toString(), s->value( "FileIcon" ).toString(), s->value( "FilePath" ).toString(), s->value( "WorkingPath" ).toString(), s->value( "DesktopEntry" ).toBool() );
	}
	s->endArray();
	// return list
	return l;
}

void pToolsManager::initializeTools()
{
	// got menu
	QMenu* mu = MonkeyCore::menuBar()->menu( "mTools/mUserTools" );
	QMenu* md = MonkeyCore::menuBar()->menu( "mTools/mDesktopTools" );
	// clear action
	mu->clear();
	md->clear();
	// initialize tools
	foreach ( pTool t, tools() )
	{
		QAction* ac = new QAction( QIcon( t.FileIcon ), t.Caption, t.DesktopEntry ? md : mu );
		if ( ac->icon().isNull() )
			ac->setIcon( iconProvider()->icon( t.FilePath ) );
		ac->setStatusTip( t.FilePath );
		ac->setData( t.WorkingPath );
		ac->setProperty( "DesktopEntry", t.DesktopEntry );
		// add to menu
		if ( t.DesktopEntry )
			md->addAction( ac );
		else
			mu->addAction( ac );
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
	QString t = cm->processInternalVariables( a->statusTip() );
	QString w = cm->processInternalVariables( a->data().toString() );
	bool b = false;
	if ( w.isEmpty() && QFile::exists( t ) )
		b = QDesktopServices::openUrl( QUrl::fromLocalFile( t ) );
	else if ( w.isEmpty() )
		b = QProcess::startDetached( t );
	else
	{
		QProcess* p = new QProcess( this );
		connect( p, SIGNAL( finished( int ) ), p, SLOT( deleteLater() ) );
		p->setWorkingDirectory( w );
		p->start( t );
		b = p->waitForStarted();
	}
	if ( !b )
		warning( tr( "Tools Error..." ), tr( "Error trying to start tool :\n%1" ).arg( t ) );
}
