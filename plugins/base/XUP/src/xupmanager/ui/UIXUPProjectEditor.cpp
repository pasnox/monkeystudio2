#include "UIXUPProjectEditor.h"
#include "XUPItem.h"
#include "ProjectItemModel.h"
#include "ScopedProjectItemModel.h"
#include "ProjectEditorModel.h"
#include "UIAddVariable.h"

#include "MonkeyCore.h"
#include "PluginsManager.h"

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

	// get models
	mProjectModel = mProject->model();
	mScopedModel = mProject->model()->scopedModel();
	mFilteredModel = mProject->model()->filteredModel();
	FilteredProjectItem* fit = mFilteredModel->itemFromIndex( mFilteredModel->mapFromSource( mProject->index() ) );
	FilesProjectModel* fpm = new FilesProjectModel( fit, this );
	mVariablesModel = new ProjectEditorModel( project, ProjectEditorModel::vtVariables, this );
	mValuesModel = new ProjectEditorModel( project, ProjectEditorModel::vtValues, this );
	
	// set models
	cbScope->setModel( mScopedModel );
	cbOperator->addItems( mProject->operators() );
	tvProjectFiles->setModel( fpm );
	tvProjectFiles->header()->hide();
	lvOthersVariables->setModel( mVariablesModel );
	lvOthersValues->setModel( mValuesModel );
	
	// connections
	connect( lvOthersVariables->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( lvOthersVariables_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	connect( lvOthersValues->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( lvOthersValues_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	
	// update gui
	cbScope->setRootIndex( mScopedModel->mapFromSource( mProject->index().parent() ) );
	cbScope->setCurrentIndex( mScopedModel->mapFromSource( mProject->index() ) );
	tvProjectFiles->setRootIndex( fpm->mapFromSource( fit->index() ) );
	lvOthersVariables->setRootIndex( mVariablesModel->mapFromSource( project->index() ) );
	lvOthersVariables->setCurrentIndex( lvOthersVariables->rootIndex().child( 0, 0 ) );
	
	// temp variable
	QString plugin;
	int id;
	
	// builders
	foreach ( BuilderPlugin* bp, MonkeyCore::pluginsManager()->plugins<BuilderPlugin*>( PluginsManager::stAll ) )
	{
		cbBuilders->addItem( bp->infos().Name );
		swBuilders->addWidget( bp->settingsWidget() );
	}
	// choose project builder
	plugin = mProject->value( "builder" );
	id = cbBuilders->findText( plugin );
	if ( id == -1 && cbBuilders->count() )
		id = 0;
	cbBuilders->setCurrentIndex( id );
	swBuilders->setCurrentIndex( id );
	gbBuilders->setChecked( !plugin.isEmpty() );
	
	// compilers
	foreach ( CompilerPlugin* cp, MonkeyCore::pluginsManager()->plugins<CompilerPlugin*>( PluginsManager::stAll ) )
	{
		cbCompilers->addItem( cp->infos().Name );
		swCompilers->addWidget( cp->settingsWidget() );
	}
	// choose project builder
	plugin = mProject->value( "compiler" );
	id = cbCompilers->findText( plugin );
	if ( id == -1 && cbCompilers->count() )
		id = 0;
	cbCompilers->setCurrentIndex( id );
	swCompilers->setCurrentIndex( id );
	gbCompilers->setChecked( !plugin.isEmpty() );
	
	// debuggers
	foreach ( DebuggerPlugin* dp, MonkeyCore::pluginsManager()->plugins<DebuggerPlugin*>( PluginsManager::stAll ) )
	{
		cbDebuggers->addItem( dp->infos().Name );
		swDebuggers->addWidget( dp->settingsWidget() );
	}
	// choose project builder
	plugin = mProject->value( "debugger" );
	id = cbDebuggers->findText( plugin );
	if ( id == -1 && cbDebuggers->count() )
		id = 0;
	cbDebuggers->setCurrentIndex( id );
	swDebuggers->setCurrentIndex( id );
	gbDebuggers->setChecked( !plugin.isEmpty() );
	
	// interpreters
	foreach ( InterpreterPlugin* ip, MonkeyCore::pluginsManager()->plugins<InterpreterPlugin*>( PluginsManager::stAll ) )
	{
		cbInterpreters->addItem( ip->infos().Name );
		swInterpreters->addWidget( ip->settingsWidget() );
	}
	// choose project builder
	plugin = mProject->value( "interpreter" );
	id = cbInterpreters->findText( plugin );
	if ( id == -1 && cbInterpreters->count() )
		id = 0;
	cbInterpreters->setCurrentIndex( id );
	swInterpreters->setCurrentIndex( id );
	gbInterpreters->setChecked( !plugin.isEmpty() );
}

XUPItem* UIXUPProjectEditor::currentScope() const
{
	QModelIndex index = mScopedModel->mapToSource( cbScope->currentIndex() );
	return mProjectModel->itemFromIndex( index );
}

XUPItem* UIXUPProjectEditor::currentVariable() const
{
	QModelIndex index = lvOthersVariables->currentIndex();
	if ( index.parent() != lvOthersVariables->rootIndex() )
		index = QModelIndex();
	index = mVariablesModel->mapToSource( index );
	return mProjectModel->itemFromIndex( index );
}

XUPItem* UIXUPProjectEditor::currentValue() const
{
	QModelIndex index = lvOthersValues->currentIndex();
	if ( index.parent() != lvOthersValues->rootIndex() )
		index = QModelIndex();
	index = mValuesModel->mapToSource( index );
	return mProjectModel->itemFromIndex( index );
}

void UIXUPProjectEditor::on_cbScope_currentChanged( const QModelIndex& idx )
{
	const QModelIndex index = mScopedModel->mapToSource( idx );
	if ( XUPItem* it = mProjectModel->itemFromIndex( index ) )
	{
		// set model root item
		mVariablesModel->setRootItem( it );
		// set view root index
		lvOthersVariables->setRootIndex( mVariablesModel->mapFromSource( it->index() ) );
		// get current index
		QModelIndex idx = lvOthersVariables->currentIndex();
		// if not valid try to get first child
		if ( !idx.isValid() || idx.parent() != lvOthersVariables->rootIndex() )
			idx = lvOthersVariables->rootIndex().child( 0, 0 );
		// if valid set it as current
		if ( idx.isValid() )
			lvOthersVariables->setCurrentIndex( idx );
		// else simulate event
		else
			lvOthersVariables_currentChanged( idx, idx );
	}
}

void UIXUPProjectEditor::on_cbOperator_currentIndexChanged( const QString& /*text*/ )
{}

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
	QModelIndex idx = tvProjectFiles->currentIndex();
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
	FilesProjectModel* fpm = qobject_cast<FilesProjectModel*>( tvProjectFiles->model() );
	QModelIndex idx = tvProjectFiles->currentIndex();
	if ( idx.isValid() )
	{
		if ( XUPItem* it = mProjectModel->itemFromIndex( mFilteredModel->mapToSource( fpm->mapToSource( idx ) ) ) )
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
					const QString v = QFileDialog::getOpenFileName( window(), tr( "Choose a new file name" ), it->filePath() ); 
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

void UIXUPProjectEditor::lvOthersVariables_currentChanged( const QModelIndex& current, const QModelIndex& )
{
	// enable values groupbox according to index validity
	gbOthersValues->setEnabled( current.isValid() );
	tbOthersVariablesEdit->setEnabled( current.isValid() );
	tbOthersVariablesRemove->setEnabled( current.isValid() );
	// get xup item
	if ( current.isValid() )
	{
		const QModelIndex index = mVariablesModel->mapToSource( current );
		if ( XUPItem* it = mProjectModel->itemFromIndex( index ) )
		{
			mValuesModel->setRootItem( it );
			lvOthersValues->setRootIndex( mValuesModel->mapFromSource( it->index() ) );
		}
	}
	else
	{
		const QModelIndex varRootIndex = mVariablesModel->mapToSource( lvOthersVariables->rootIndex() );
		mValuesModel->setRootItem( mVariablesModel->rootItem() );
		lvOthersValues->setRootIndex( mValuesModel->mapFromSource( varRootIndex ) );
	}
	// set current value
	lvOthersValues_currentChanged( QModelIndex(), QModelIndex() );
	if ( XUPItem* vit = currentVariable() )
		if ( XUPItem* cv = vit->child( 0 ) )
			lvOthersValues->setCurrentIndex( mValuesModel->mapFromSource( cv->index() ) );
}

void UIXUPProjectEditor::on_tbOthersVariablesAdd_clicked()
{
	// init dialog
	UIAddVariable d( window() );
	//d.setVariablesName(  );
	d.setOperators( mProject->operators() );
	// execute dialog
	if ( d.exec() == QDialog::Accepted )
	{
		// get var / op
		const QString vn = d.getVariableName();
		const QString op = d.getOperator();
		// create item
		if ( XUPItem* scope = currentScope() )
		{
			// check if it already exists
			foreach ( XUPItem* cit, scope->children( false, true ) )
			{
				if ( cit->isType( "variable" ) && cit->defaultValue() == vn && cit->value( "operator", "=" ) == op )
				{
					if ( QMessageBox::question( window(), tr( "Add a variable..." ), tr( "A variable with the same name and operator already exists in this scope, do you want to use it instead ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
					{
						lvOthersVariables->setCurrentIndex( mVariablesModel->mapFromSource( cit->index() ) );
						return;
					}
					else
						break;
				}
			}
			// create item
			XUPItem* vit = scope->clone( false );
			vit->setDomElement( scope->domElement().ownerDocument().createElement( "variable" ) );
			scope->domElement().appendChild( vit->domElement() );
			vit->setValue( vit->valueName(), vn );
			vit->setValue( "operator", op );
			// append it
			scope->appendRow( vit );
			// set it current item
			lvOthersVariables->setCurrentIndex( mVariablesModel->mapFromSource( vit->index() ) );
		}
	}
}

void UIXUPProjectEditor::on_tbOthersVariablesEdit_clicked()
{
	if ( XUPItem* vit = currentVariable() )
	{
		// init dialog
		UIAddVariable d( window() );
		d.setWindowTitle( tr( "Edit a variable..." ) );
		d.setVariablesName( QStringList( vit->defaultValue() ) );
		d.setCurrentVariableName( vit->defaultValue() );
		d.setOperators( vit->operators() );
		d.setCurrentOperator( vit->value( "operator", "=" ) );
		// execute dialog
		if ( d.exec() == QDialog::Accepted )
		{
			// get var / op
			const QString vn = d.getVariableName();
			const QString op = d.getOperator();
			// check if it already exists
			foreach ( XUPItem* cit, vit->parent()->children( false, true ) )
			{
				if ( cit->isType( "variable" ) && cit->defaultValue() == vn && cit->value( "operator", "=" ) == op && cit != vit )
				{
					if ( QMessageBox::question( window(), d.windowTitle(), tr( "A variable with the same name and operator already exists in this scope, proceed anyway ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
						return;
					else
						break;
				}
			}
			// update item
			vit->setValue( vit->valueName(), vn );
			vit->setValue( "operator", op );
		}
	}
}

void UIXUPProjectEditor::on_tbOthersVariablesRemove_clicked()
{
	if ( XUPItem* vit = currentVariable() )
	{
		// confirm user request
		if ( QMessageBox::question( window(), tr( "Remove a variable..." ), tr( "A you sure you want to remove this variable and all its content ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::No )
			return;
		// delete childs
		while ( vit->rowCount() )
			vit->child( 0 )->remove();
		// delete variable
		vit->remove();
		// update view
		on_cbScope_currentChanged( cbScope->currentIndex() );
	}
}

void UIXUPProjectEditor::lvOthersValues_currentChanged( const QModelIndex& current, const QModelIndex& )
{
	// enable button according to index validity
	tbOthersValuesEdit->setEnabled( current.isValid() );
	tbOthersValuesRemove->setEnabled( current.isValid() );
}

void UIXUPProjectEditor::on_tbOthersValuesAdd_clicked()
{
	if ( XUPItem* cv = currentVariable() )
	{
		const QString title = tr( "Add a value..." );
		bool ok;
		const QString val = QInputDialog::getText( window(), title, tr( "Enter the value :" ), QLineEdit::Normal, QString(), &ok );
		if ( ok && !val.isEmpty() )
		{
			// check if value already exists
			foreach ( XUPItem* cit, cv->children( false, false ) )
			{
				if ( cit->isType( "value" ) && cit->defaultValue() == val )
				{
					if ( QMessageBox::question( window(), title, tr( "A value with the same content already exists, add anyway ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
					{
						lvOthersValues->setCurrentIndex( mValuesModel->mapFromSource( cit->index() ) );
						return;
					}
					else
						break;
				}
			}
			// create item
			XUPItem* vit = cv->clone( false );
			vit->setDomElement( cv->domElement().ownerDocument().createElement( "value" ) );
			cv->domElement().appendChild( vit->domElement() );
			vit->setValue( vit->valueName(), val );
			// append it
			cv->appendRow( vit );
			// set variable multiline if needed
			if ( cv->rowCount() > 1 )
				cv->setValue( "multiline", "true" );
			// set it current item
			lvOthersValues->setCurrentIndex( mValuesModel->mapFromSource( cv->index() ) );
		}
	}
}

void UIXUPProjectEditor::on_tbOthersValuesEdit_clicked()
{
	if ( XUPItem* cv = currentValue() )
	{
		const QString title = tr( "Edit a value..." );
		bool ok;
		const QString val = QInputDialog::getText( window(), title, tr( "Edit the value :" ), QLineEdit::Normal, cv->defaultValue(), &ok );
		if ( ok && !val.isEmpty() )
		{
			// check if value already exists
			foreach ( XUPItem* cit, cv->parent()->children( false, true ) )
			{
				if ( cit->isType( "value" ) && cit->defaultValue() == val && cit != cv )
				{
					if ( QMessageBox::question( window(), title, tr( "A value with the same content already exists in this variable, proceed anyway ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
						return;
					else
						break;
				}
			}
			// update item
			cv->setValue( cv->valueName(), val );
		}
	}
}

void UIXUPProjectEditor::on_tbOthersValuesRemove_clicked()
{
	if ( XUPItem* vit = currentValue() )
	{
		// confirm user request
		if ( QMessageBox::question( window(), tr( "Remove a value..." ), tr( "A you sure you want to remove this value ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::No )
			return;
		// delete value
		vit->remove();
		// update view
		lvOthersVariables_currentChanged( lvOthersVariables->currentIndex(), QModelIndex() );
	}
}

void UIXUPProjectEditor::on_tbOthersValuesClear_clicked()
{
	if ( XUPItem* vit = currentVariable() )
	{
		// request suer confirm
		if ( QMessageBox::question( window(), tr( "Clear values..." ), tr( "A you sure you want to clear these values ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
		{
			// delete items
			while ( vit->rowCount() )
				vit->child( 0 )->remove();
			// update view
			lvOthersVariables_currentChanged( lvOthersVariables->currentIndex(), QModelIndex() );
		}
	}
}
