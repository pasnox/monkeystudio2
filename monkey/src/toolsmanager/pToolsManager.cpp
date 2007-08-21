#include "pToolsManager.h"
#include "pSettings.h"
#include "pMenuBar.h"
#include "UIToolsEdit.h"
#include "UIDesktopTools.h"
#include "pMonkeyStudio.h"

#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QTimer>

QFileIconProvider* pToolsManager::mIconProvider = 0L;

pToolsManager::pToolsManager( QObject* p )
	: QObject( p )
{
	// initialise action
	initializeTools();
	// action connection
	connect( pMenuBar::instance()->action( "mTools/aEditUser" ), SIGNAL( triggered() ), this, SLOT( editTools_triggered() ) );
	connect( pMenuBar::instance()->action( "mTools/aEditDesktop" ), SIGNAL( triggered() ), this, SLOT( editTools_triggered() ) );
	connect( pMenuBar::instance()->menu( "mTools/mUserTools" ), SIGNAL( triggered( QAction* ) ), this, SLOT( toolsMenu_triggered( QAction* ) ) );
	connect( pMenuBar::instance()->menu( "mTools/mDesktopTools" ), SIGNAL( triggered( QAction* ) ), this, SLOT( toolsMenu_triggered( QAction* ) ) );
}

pToolsManager::~pToolsManager()
{ delete mIconProvider; }

const QFileIconProvider* pToolsManager::iconProvider()
{
	return mIconProvider ? mIconProvider : ( mIconProvider = new QFileIconProvider() );
}

const QList<pTool> pToolsManager::tools( ToolType t )
{
	// tools list
	QList<pTool> l;
	// got settings
	pSettings* s = pSettings::instance();
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
	QMenu* mu = pMenuBar::instance()->menu( "mTools/mUserTools" );
	QMenu* md = pMenuBar::instance()->menu( "mTools/mDesktopTools" );
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
	if ( ( a == pMenuBar::instance()->action( "mTools/aEditUser" ) ? UIToolsEdit::instance()->exec() : UIDesktopTools::instance()->exec() ) )
		initializeTools();
}

void pToolsManager::toolsMenu_triggered( QAction* a )
{
	bool b = false;
	if ( a->data().toString().isEmpty() && QFile::exists( a->statusTip() ) )
		b = QDesktopServices::openUrl( QUrl::fromLocalFile( a->statusTip() ) );
	else if ( a->data().toString().isEmpty() )
		b = QProcess::startDetached( a->statusTip() );
	else
	{
		QProcess* p = new QProcess( this );
		connect( p, SIGNAL( finished( int ) ), p, SLOT( deleteLater() ) );
		p->setWorkingDirectory( a->data().toString() );
		p->start( a->statusTip() );
		b = p->waitForStarted();
	}
	if ( !b )
		pMonkeyStudio::warning( tr( "Tools Error..." ), tr( "Error trying to start tools:\n%1" ).arg( a->statusTip() ) );
}
