#include "UIXUPProjectEditor.h"
#include "XUPItem.h"
#include "ProjectItemModel.h"
#include "ScopedProjectItemModel.h"

#include <QDebug>

#include <QFileInfo>
#include <QHeaderView>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

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
	const QStringList l = QFileDialog::getOpenFileNames( window(), tr( "Choose the files to add to your project" ), mProject->projectPath() );
	if ( !l.isEmpty() )
	{
		QModelIndex idx = mProject->model()->scopedModel()->mapToSource( cbScope->currentIndex() );
		XUPItem* pit = mProject->model()->itemFromIndex( idx );
		pit->addFiles( l, 0, cbOperator->currentText() );
	}
}

void UIXUPProjectEditor::on_pbRemoveProjectFile_clicked()
{
	FilteredProjectItemModel* fm = mProject->model()->filteredModel();
	FilesProjectModel* fpm = qobject_cast<FilesProjectModel*>( tvProjectFiles->model() );
	QModelIndex idx = tvProjectFiles->selectionModel()->selectedIndexes().value( 0 );
	if ( idx.isValid() )
	{
		if ( XUPItem* it = mProject->model()->itemFromIndex( fm->mapToSource( fpm->mapToSource( idx ) ) ) )
		{
			if ( it->isType( "value" ) )
			{
				if ( QMessageBox::question( 0, "Question...", tr( "Are you sur you want to remove this value ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
				{
					QString fp = it->filePath();
					if ( QFile::exists( fp ) && QMessageBox::question( 0, "Question...", tr( "Do you want to delete the associate file ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
						if ( !QFile::remove( fp ) )
							QMessageBox::warning( 0, "Warning...", tr( "Can't delete file: %1" ).arg( fp ) );
					// get parent
					XUPItem* parent = it->parent();
					// remove item
					it->remove();
					// delete parent is empty
					if ( !parent->hasChildren() )
						parent->remove();
				}
			}
		}
	}
}

void UIXUPProjectEditor::on_pbModifyProjectFile_clicked()
{
	FilteredProjectItemModel* fm = mProject->model()->filteredModel();
	FilesProjectModel* fpm = qobject_cast<FilesProjectModel*>( tvProjectFiles->model() );
	QModelIndex idx = tvProjectFiles->selectionModel()->selectedIndexes().value( 0 );
	if ( idx.isValid() )
	{
		if ( XUPItem* it = mProject->model()->itemFromIndex( fm->mapToSource( fpm->mapToSource( idx ) ) ) )
		{
			if ( it->isType( "value" ) )
			{
				QString s;
				// prepare dialog
				QMessageBox mb( window() );
				mb.setIcon( QMessageBox::Question );
				mb.setText( tr( "Modify a file." ) );
				mb.setInformativeText( tr( "Choose you either want to Edit or Browse the file." ) );
				mb.setDetailedText( tr( "Clicking the 'Edit text value' button will popup you an input dialog where you can directly change the content of the value.\nClicking the 'Browse for a file' button will popup a file dialog where you can browse directly for a file." ) );
				QAbstractButton* eb = mb.addButton( tr( "Edit text value" ), QMessageBox::AcceptRole );
				QAbstractButton* bb = mb.addButton( tr( "Browse for a file" ), QMessageBox::RejectRole );
				// execute dialog
				mb.exec();
				if ( mb.clickedButton() == eb )
				{
					bool ok;
					const QString v = QInputDialog::getText( window(), tr( "Waiting new value..." ), tr( "Enter the new content for this file value :" ), QLineEdit::Normal, it->defaultValue(), &ok );
					if ( ok && !v.isEmpty() )
						s = v;
				}
				else if ( mb.clickedButton() == bb )
				{
					const QString v = QFileDialog::getOpenFileName( window(), tr( "Choose a new file name" ), it->defaultInterpretedValue() ); 
					if ( !v.isEmpty() )
						s = v;
				}
				// apply new value if needed
				if ( !s.isEmpty() )
					it->setValue( it->valueName(), it->relativeFilePath( s ) );
			}
		}
	}
}
