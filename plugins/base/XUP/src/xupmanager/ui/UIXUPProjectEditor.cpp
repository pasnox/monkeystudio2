#include "UIXUPProjectEditor.h"

#include "XUPItem.h"
#include "ProjectItemModel.h"
#include "ScopedProjectItemModel.h"

#include <QFileInfo>
#include <QHeaderView>

UIXUPProjectEditor::UIXUPProjectEditor( XUPItem* project, QWidget* parent )
	: QDialog( parent )
{
	// get proejct
	Q_ASSERT( project );
	mProject = project;

	// init dialog
	setupUi( this );
	setWindowTitle( windowTitle().arg( QFileInfo( mProject->projectFilePath() ).fileName() ) );

	// update gui
	ScopedProjectItemModel* sm = mProject->model()->scopedModel();
	FilteredProjectItemModel* fm = mProject->model()->filteredModel();
	cbScope->setModel( sm );
	cbScope->setRootIndex( sm->mapFromSource( mProject->index().parent() ) );
	cbScope->setCurrentIndex( sm->mapFromSource( mProject->index() ) );
	cbOperator->addItems( mProject->operators() );

	FilteredProjectItem* fit = fm->itemFromIndex( fm->mapFromSource( mProject->index() ) );
	SingleFilteredProjectModel* sfpm = new SingleFilteredProjectModel( fit, this );
	tvProjectFiles->setModel( sfpm );
	tvProjectFiles->setRootIndex( sfpm->mapFromSource( fm->mapFromSource( mProject->index() ) ) );
	tvProjectFiles->header()->hide();
}
