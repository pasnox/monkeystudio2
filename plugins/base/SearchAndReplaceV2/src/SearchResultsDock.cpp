#include "SearchResultsDock.h"
#include "SearchResultsModel.h"

#include <pIconManager.h>
#include <pMonkeyStudio.h>

#include <QHBoxLayout>
#include <QTreeView>

SearchResultsDock::SearchResultsDock( SearchThread* searchThread, QWidget* parent )
	: pDockWidget( parent )
{	
	setObjectName( metaObject()->className() );
	setWindowTitle( tr( "Search Results" ) );
	setWindowIcon( pIconManager::icon( "SearchAndReplaceV2.png", ":/icons" ) );
	
	QWidget* widget = new QWidget( this );
	mModel = new SearchResultsModel( searchThread, this );
	mView = new QTreeView( this );
	mView->setHeaderHidden( true );
	mView->setUniformRowHeights( true );
	mView->setModel( mModel );
	mLayout = new QHBoxLayout( widget );
	mLayout->setMargin( 5 );
	mLayout->setSpacing( 5 );
	mLayout->addWidget( mView );
	
	setWidget( widget );
	
	// mac
	pMonkeyStudio::showMacFocusRect( this, false, true );
	pMonkeyStudio::setMacSmallSize( this, true, true );
}
