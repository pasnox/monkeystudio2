#include "pOpenedFileExplorer.h"
#include "pWorkspace.h"
#include "pOpenedFileModel.h"

#include <pIconManager.h>
#include <pDockWidgetTitleBar.h>

pOpenedFileExplorer::pOpenedFileExplorer( pWorkspace* workspace )
	: pDockWidget( workspace )
{
	Q_ASSERT( workspace );
	mWorkspace = workspace;
	mModel = new pOpenedFileModel( workspace );
	setupUi( this );
	tvFiles->setModel( mModel );
	setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	
	// sort menu
	mSortMenu = new QMenu( this );
	QActionGroup* group = new QActionGroup( mSortMenu );
	
	group->addAction( tr( "Opening order" ) );
	group->addAction( tr( "File name" ) );
	group->addAction( tr( "URL" ) );
	group->addAction( tr( "Suffixes" ) );
	group->addAction( tr( "Custom" ) );
	mSortMenu->addActions( group->actions() );
	
	for ( int i = pOpenedFileModel::OpeningOrder; i < pOpenedFileModel::Custom +1; i++ )
	{
		QAction* action = group->actions().at( i );
		action->setData( (pOpenedFileModel::SortMode)i );
		action->setCheckable( true );
		
		if ( i == pOpenedFileModel::OpeningOrder )
		{
			action->setChecked( true );
		}
	}
	
	QAction* aSortMenu = new QAction( tr( "Sorting" ), this );
	aSortMenu->setMenu( mSortMenu );
	aSortMenu->setIcon( pIconManager::icon( "sort.png" ) );
	aSortMenu->setToolTip( aSortMenu->text() );
	
	QToolButton* tb = qobject_cast<QToolButton*>( titleBar()->addAction( aSortMenu, 0 ) );
	tb->setPopupMode( QToolButton::InstantPopup );
	titleBar()->addSeparator( 1 );
	
	connect( group, SIGNAL( triggered ( QAction* ) ), this, SLOT( sortTriggered ( QAction* ) ) );
	connect( workspace, SIGNAL( documentChanged( pAbstractChild* ) ), this, SLOT( documentChanged( pAbstractChild* ) ) );
	connect( workspace, SIGNAL( documentAboutToClose( pAbstractChild* ) ), this, SLOT( documentAboutToClose( pAbstractChild* ) ) );
	connect( workspace, SIGNAL( currentDocumentChanged( pAbstractChild* ) ), this, SLOT( currentDocumentChanged( pAbstractChild* ) ) );
}

pOpenedFileExplorer::~pOpenedFileExplorer()
{
}

void pOpenedFileExplorer::sortTriggered ( QAction* action )
{
	pOpenedFileModel::SortMode mode = (pOpenedFileModel::SortMode)action->data().toInt();
	mModel->setSortMode( mode );
}

void pOpenedFileExplorer::documentChanged( pAbstractChild* document )
{
	//
}

void pOpenedFileExplorer::documentAboutToClose( pAbstractChild* document )
{
	//
}

void pOpenedFileExplorer::currentDocumentChanged( pAbstractChild* )
{
	//
}
