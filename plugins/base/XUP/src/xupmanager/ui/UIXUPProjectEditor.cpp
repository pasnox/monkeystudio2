#include "UIXUPProjectEditor.h"

#include "XUPItem.h"
#include "ProjectItemModel.h"
#include "FilteredProjectItemModel.h"
#include "ScopedProjectItemModel.h"

#include <QFileInfo>

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
	ProjectItemModel* pim = mProject->model();
	cbScope->setModel( sm );
	cbScope->setRootIndex( sm->mapFromSource( mProject->index().parent() ) );
	cbScope->setCurrentIndex( sm->mapFromSource( mProject->index() ) );
	cbOperator->addItems( mProject->operators() );
	tvProjectFiles->setModel( pim );
	tvProjectFiles->setRootIndex( mProject->index().parent() );
}
