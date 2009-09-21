#include "pOpenedFileExplorer.h"
#include "pWorkspace.h"

#include <MonkeyCore.h>
#include <pIconManager.h>
#include <pDockWidgetTitleBar.h>

#include <QWidgetAction>
#include <QComboBox>

class pOpenedFileAction : public QWidgetAction
{
	Q_OBJECT

public:
	pOpenedFileAction( pOpenedFileExplorer* parent, QAbstractItemModel* model )
		: QWidgetAction( parent )
	{
		mOpenedFileExplorer = parent;
		mModel = model;
	}

protected:
	pOpenedFileExplorer* mOpenedFileExplorer;
	QAbstractItemModel* mModel;
	
	virtual QWidget* createWidget( QWidget* parent )
	{
		QComboBox* combo = new QComboBox( parent );
		combo->setMaxVisibleItems( 50 );
		combo->setSizeAdjustPolicy( QComboBox::AdjustToContents );
		combo->setAttribute( Qt::WA_MacSmallSize );
		combo->setModel( mModel );
		
		connect( combo, SIGNAL( currentIndexChanged( int ) ), mOpenedFileExplorer, SLOT( setCurrentIndex( int ) ) );
		connect( mOpenedFileExplorer, SIGNAL( currentIndexChanged( int ) ), combo, SLOT( setCurrentIndex( int ) ) );
		
		return combo;
	}
};

pOpenedFileExplorer::pOpenedFileExplorer( pWorkspace* workspace )
	: pDockWidget( workspace )
{
	Q_ASSERT( workspace );
	mWorkspace = workspace;
	mModel = new pOpenedFileModel( workspace );
	aComboBox = new pOpenedFileAction( this, mModel );
	setupUi( this );
	setFocusProxy( tvFiles );
	setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	tvFiles->setModel( mModel );
	tvFiles->setAttribute( Qt::WA_MacShowFocusRect, false );
	tvFiles->setAttribute( Qt::WA_MacSmallSize );
	
	
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
	
	tvFiles->viewport()->setAcceptDrops( true );
	
	connect( group, SIGNAL( triggered ( QAction* ) ), this, SLOT( sortTriggered ( QAction* ) ) );
	connect( workspace, SIGNAL( documentChanged( pAbstractChild* ) ), this, SLOT( documentChanged( pAbstractChild* ) ) );
	connect( workspace, SIGNAL( currentDocumentChanged( pAbstractChild* ) ), this, SLOT( currentDocumentChanged( pAbstractChild* ) ) );
	connect( mModel, SIGNAL( documentMoved( pAbstractChild* ) ), this, SLOT( currentDocumentChanged( pAbstractChild* ) ) );
	connect( mModel, SIGNAL( sortModeChanged( pOpenedFileModel::SortMode ) ), this, SLOT( sortModeChanged( pOpenedFileModel::SortMode ) ) );
	connect( tvFiles->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( selectionModel_selectionChanged( const QItemSelection&, const QItemSelection& ) ) );
}

QAction* pOpenedFileExplorer::comboBoxAction() const
{
	return aComboBox;
}

void pOpenedFileExplorer::setCurrentIndex( int row )
{
	const QModelIndex index = mModel->index( row, 0 );
	selectedIndexChanged( index );
}

void pOpenedFileExplorer::sortTriggered ( QAction* action )
{
	pOpenedFileModel::SortMode mode = (pOpenedFileModel::SortMode)action->data().toInt();
	mModel->setSortMode( mode );
}

void pOpenedFileExplorer::documentChanged( pAbstractChild* document )
{
	Q_UNUSED( document );
}

void pOpenedFileExplorer::currentDocumentChanged( pAbstractChild* document )
{
	const QModelIndex index = mModel->index( document );
	selectedIndexChanged( index );
}

void pOpenedFileExplorer::sortModeChanged( pOpenedFileModel::SortMode mode )
{
	foreach ( QAction* action, mSortMenu->actions() )
	{
		if ( action->data().toInt() == mode )
		{
			if ( !action->isChecked() )
			{
				action->setChecked( true );
			}
			
			return;
		}
	}
}

void pOpenedFileExplorer::selectedIndexChanged( const QModelIndex& index )
{
	pAbstractChild* document = mModel->document( index );
	
	if ( tvFiles->currentIndex() != index )
	{
		tvFiles->setCurrentIndex( index );
	}
	
	mWorkspace->setCurrentDocument( document );
	setFocus(); // setting active mdi window still the focus
	emit currentIndexChanged( index );
	emit currentIndexChanged( index.row() );
}

void pOpenedFileExplorer::selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
	Q_UNUSED( deselected );
	const QModelIndex index = selected.indexes().value( 0 );
	selectedIndexChanged( index );
}

void pOpenedFileExplorer::on_tvFiles_customContextMenuRequested( const QPoint& pos )
{
	QMenu menu;
	menu.addAction( MonkeyCore::menuBar()->action( "mFile/mClose/aCurrent" ) );
	menu.addAction( MonkeyCore::menuBar()->action( "mFile/mSave/aCurrent" ) );
	menu.addSeparator();
	menu.addAction( mSortMenu->menuAction() );
	menu.exec( tvFiles->mapToGlobal( pos ) );
}

#include "pOpenedFileExplorer.moc"
