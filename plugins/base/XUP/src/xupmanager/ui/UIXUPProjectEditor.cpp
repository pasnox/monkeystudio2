#include "UIXUPProjectEditor.h"
#include "AddFilesDialog.h"
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
	FilesProjectModel* fpm = new FilesProjectModel( fit, this );
	tvProjectFiles->setModel( fpm );
	tvProjectFiles->setRootIndex( fpm->mapFromSource( fit->index() ) );
	tvProjectFiles->header()->hide();
}

void UIXUPProjectEditor::on_pbAddProjectFiles_clicked()
{
	AddFilesDialog d( mProject->model()->scopedModel(), mProject, window() );
	if ( d.exec() && !d.selectedFiles().isEmpty() )
		d.currentItem()->addFiles( d.selectedFiles(), d.currentItem(), d.currentOperator() );
}
