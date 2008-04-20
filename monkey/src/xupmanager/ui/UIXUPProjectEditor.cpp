#include "UIXUPProjectEditor.h"
#include "../XUPItem.h"
#include "../ProjectItemModel.h"
#include "../ScopedProjectItemModel.h"
#include "../ProjectEditorModel.h"
#include "UIAddVariable.h"
#include "../AddFilesDialog.h"

#include <coremanager.h>
#include <pluginsmanager.h>

#include <QDebug>

#include <QFileInfo>
#include <QHeaderView>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

void recursiveRemoveItem( XUPItem* it, bool deleteFiles = false )
{
	if ( it )
	{
		while ( it->rowCount() )
		{
			// get item to remove
			XUPItem* cit = it->child( 0 );
			// remove its children if needed
			if ( cit->hasChildren() )
				recursiveRemoveItem( cit );
			// if it's a file item remvoe fiel if needed
			if ( deleteFiles && cit->isType( "value" ) && it->fileVariables().contains( it->defaultValue() ) )
			{
				const QString fp = cit->filePath();
				if ( QFile::exists( fp ) && !QFile::remove( fp ) )
					QMessageBox::warning( 0, "Warning...", QObject::tr( "Can't delete file: %1" ).arg( fp ) );
			}
			// finally remove item
			cit->remove();
		}
	}
}

UIXUPProjectEditor::UIXUPProjectEditor( XUPItem* project, QWidget* parent )
	: QDialog( parent )
{
	// get proejct
	Q_ASSERT( project );
	mProject = project;

	// init dialog
	setupUi( this );
	setWindowTitle( windowTitle().arg( QFileInfo( mProject->projectFilePath() ).fileName() ) );
	lOperator->setVisible( false );
	cbOperator->setVisible( false );
	cbOperator->addItems( mProject->operators() );
	
	// tbAddProjectFiles
	QMenu* projectFilesMenu = new QMenu( tbModifyProjectFile );
	aModifyProjectFileFile = projectFilesMenu->addAction( tr( "As File..." ) );
	aModifyProjectFileValue = projectFilesMenu->addAction( tr( "As Value..." ) );
	tbModifyProjectFile->setMenu( projectFilesMenu );
	
	// tbOthersVariablesEdit
	QMenu* menu = new QMenu( tbOthersVariablesEdit );
	aOthersVariablesEditVariable = menu->addAction( tr( "Edit Variable..." ) );
	aOthersVariablesEditMultiline = menu->addAction( tr( "Edit Multiline Property..." ) );
	tbOthersVariablesEdit->setMenu( menu );
	
	// tbOthersValuesAdd actions
	QMenu* addMenu = new QMenu( tbOthersValuesAdd );
	aOthersValuesAddValue = addMenu->addAction( tr( "As Value..." ) );
	aOthersValuesAddFile = addMenu->addAction( tr( "As File..." ) );
	aOthersValuesAddPath = addMenu->addAction( tr( "As Path..." ) );
	tbOthersValuesAdd->setMenu( addMenu );
	
	// tbOthersValuesEdit actions
	QMenu* editMenu = new QMenu( tbOthersValuesEdit );
	aOthersValuesEditValue = editMenu->addAction( tr( "As Value..." ) );
	aOthersValuesEditFile = editMenu->addAction( tr( "As File..." ) );
	aOthersValuesEditPath = editMenu->addAction( tr( "As Path..." ) );
	tbOthersValuesEdit->setMenu( editMenu );

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
	tvProjectFiles->setModel( fpm );
	tvProjectFiles->header()->hide();
	lvOthersVariables->setModel( mVariablesModel );
	lvOthersValues->setModel( mValuesModel );
	
	// connections
	connect( lvOthersVariables->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( lvOthersVariables_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	connect( lvOthersValues->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( lvOthersValues_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	// doing here to avoid fucking bug
	connect( projectFilesMenu, SIGNAL( triggered( QAction* ) ), tbModifyProjectFile, SIGNAL( triggered( QAction* ) ) );
	connect( menu, SIGNAL( triggered( QAction* ) ), tbOthersVariablesEdit, SIGNAL( triggered( QAction* ) ) );
	connect( addMenu, SIGNAL( triggered( QAction* ) ), tbOthersValuesAdd, SIGNAL( triggered( QAction* ) ) );
	connect( editMenu, SIGNAL( triggered( QAction* ) ), tbOthersValuesEdit, SIGNAL( triggered( QAction* ) ) );
	
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
	plugin = mProject->projectSettingsValue( "BUILDER" );
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
	// choose project compiler
	plugin = mProject->projectSettingsValue( "COMPILER" );
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
	// choose project debugger
	plugin = mProject->projectSettingsValue( "DEBUGGER" );
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
	// choose project interpreter
	plugin = mProject->projectSettingsValue( "INTERPRETER" );
	id = cbInterpreters->findText( plugin );
	if ( id == -1 && cbInterpreters->count() )
		id = 0;
	cbInterpreters->setCurrentIndex( id );
	swInterpreters->setCurrentIndex( id );
	gbInterpreters->setChecked( !plugin.isEmpty() );
}

void UIXUPProjectEditor::insertPage( int id, QWidget* page )
{
	// check page validity
	if ( !page )
		return;
	// check index
	if ( id == -1 )
		id = cbPages->count();
	// insert page in stack
	swPages->insertWidget( id, page );
	// insert item in combobox
	cbPages->insertItem( id, page->windowIcon(), page->windowTitle() );
}

XUPItem* UIXUPProjectEditor::currentScope() const
{
	QModelIndex index = mScopedModel->mapToSource( cbScope->currentIndex() );
	return mProjectModel->itemFromIndex( index );
}

QString UIXUPProjectEditor::currentOperator() const
{ return cbOperator->currentText(); }

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

void UIXUPProjectEditor::on_tbAddScope_clicked()
{
	bool ok;
	const QString sn = QInputDialog::getText( window(), tr( "Enter the scope value..." ), tr( "Enter the scope value you want to create :" ), QLineEdit::Normal, QString(), &ok );
	if ( ok && !sn.isEmpty() )
	{
		// get current scope
		XUPItem* curScope = currentScope();
		// create item
		XUPItem* scope = curScope->clone( false );
		scope->setDomElement( curScope->domElement().ownerDocument().createElement( "scope" ) );
		curScope->domElement().appendChild( scope->domElement() );
		scope->setValue( scope->valueName(), sn );
		scope->setValue( "nested", "true" );
		// append it
		curScope->appendRow( scope );
		// if it's in a scope, set the scope not nested
		if ( curScope != mProject )
			curScope->setValue( "nested", "false" );
		// set it current
		cbScope->setCurrentIndex( mScopedModel->mapFromSource( scope->index() ) );
	}
}

void UIXUPProjectEditor::on_tbRemoveScope_clicked()
{
	if ( XUPItem* scope = currentScope() )
	{
		// we can't remove proejct itself
		if ( scope == mProject )
			return;
		// request suer confirm
		if ( QMessageBox::question( window(), tr( "Remove scope..." ), tr( "A you sure you want to remove this scope ?\nAll children items will be removed too." ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
		{
			// recursively remove items
			bool b = QMessageBox::question( window(), tr( "Remove files..." ), tr( "Do you want to delete the files that are associated with items ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes;
			recursiveRemoveItem( scope, b );
			scope->remove();
		}
	}
}

void UIXUPProjectEditor::on_cbScope_currentChanged( const QModelIndex& idx )
{
	const QModelIndex index = mScopedModel->mapToSource( idx );
	if ( XUPItem* it = mProjectModel->itemFromIndex( index ) )
	{
		// enable remove scope according to current scope
		tbRemoveScope->setEnabled( it != mProject );
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
		// emit current scope changed
		emit currentScopeChanged( it );
	}
}

void UIXUPProjectEditor::on_cbOperator_currentIndexChanged( const QString& text )
{ emit currentOperatorChanged( text ); }

void UIXUPProjectEditor::on_tbAddProjectFiles_clicked()
{
	AddFilesDialog d( mScopedModel, currentScope(), window() );
	if ( d.exec() == QDialog::Accepted && !d.selectedFiles().isEmpty() )
		d.currentItem()->addFiles( d.selectedFiles(), d.currentItem(), d.currentOperator() );
}

void UIXUPProjectEditor::on_tbRemoveProjectFile_clicked()
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

void UIXUPProjectEditor::on_tbModifyProjectFile_clicked()
{ on_tbModifyProjectFile_triggered( aModifyProjectFileFile ); }

void UIXUPProjectEditor::on_tbModifyProjectFile_triggered( QAction* action )
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
				// get value
				if ( action == aModifyProjectFileValue )
				{
					bool ok;
					s = QInputDialog::getText( window(), tr( "Waiting new value..." ), tr( "Enter the new content for this file value :" ), QLineEdit::Normal, it->defaultValue(), &ok );
					if ( !ok )
						s.clear();
				}
				else if ( action == aModifyProjectFileFile )
					s = QFileDialog::getOpenFileName( window(), tr( "Choose a new file name" ), it->filePath() ); 
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
	// if valid scope
	if ( XUPItem* scope = currentScope() )
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
			// if it's in a scope, set the scope not nested
			if ( scope != mProject )
				scope->setValue( "nested", "false" );
			// set it current item
			lvOthersVariables->setCurrentIndex( mVariablesModel->mapFromSource( vit->index() ) );
		}
	}
}

void UIXUPProjectEditor::on_tbOthersVariablesEdit_clicked()
{ on_tbOthersVariablesEdit_triggered( aOthersVariablesEditVariable ); }

void UIXUPProjectEditor::on_tbOthersVariablesEdit_triggered( QAction* action )
{
	if ( XUPItem* vit = currentVariable() )
	{
		if ( action == aOthersVariablesEditVariable )
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
		else if ( action == aOthersVariablesEditMultiline )
		{
			const QStringList items = QStringList() << "true" << "false";
			bool ok;
			const QString s = QInputDialog::getItem( window(), tr( "Edit Property..." ), tr( "Select the new property value :" ), items, items.indexOf( vit->value( "multiline", "false" ) ), false, &ok );
			if ( ok && !s.isEmpty() )
				vit->setValue( "multiline", s );
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
		recursiveRemoveItem( vit );
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
{ on_tbOthersValuesAdd_triggered( aOthersValuesAddValue ); }

void UIXUPProjectEditor::on_tbOthersValuesAdd_triggered( QAction* action )
{
	if ( XUPItem* cv = currentVariable() )
	{
		const QString title = tr( "Add a value..." );
		bool ok = true;
		QString val;
		if ( action == aOthersValuesAddValue )
			val	= QInputDialog::getText( window(), title, tr( "Enter the value :" ), QLineEdit::Normal, QString(), &ok );
		else if ( action == aOthersValuesAddFile )
			val = mProject->relativeFilePath( QFileDialog::getOpenFileName( window(), tr( "Choose a file" ), mProject->projectPath() ) );
		else if ( action == aOthersValuesAddPath )
			val = mProject->relativeFilePath( QFileDialog::getExistingDirectory( window(), tr( "Choose a path" ), mProject->projectPath() ) );
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
			if ( QMessageBox::question( window(), tr( "Update Property..." ), tr( "Do you want to update the multiline property of this value ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
				on_tbOthersVariablesEdit_triggered( aOthersVariablesEditMultiline );
			// set it current item
			lvOthersValues->setCurrentIndex( mValuesModel->mapFromSource( cv->index() ) );
		}
	}
}

void UIXUPProjectEditor::on_tbOthersValuesEdit_clicked()
{ on_tbOthersValuesEdit_triggered( aOthersValuesEditValue ); }

void UIXUPProjectEditor::on_tbOthersValuesEdit_triggered( QAction* action )
{
	if ( XUPItem* cv = currentValue() )
	{
		const QString title = tr( "Edit a value..." );
		bool ok = true;
		QString val;
		if ( action == aOthersValuesEditValue )
			val	= QInputDialog::getText( window(), title, tr( "Edit the value :" ), QLineEdit::Normal, cv->defaultValue(), &ok );
		else if ( action == aOthersValuesEditFile )
			val = mProject->relativeFilePath( QFileDialog::getOpenFileName( window(), tr( "Choose a file" ), cv->defaultInterpretedValue() ) );
		else if ( action == aOthersValuesEditPath )
			val = mProject->relativeFilePath( QFileDialog::getExistingDirectory( window(), tr( "Choose a path" ), cv->defaultInterpretedValue() ) );
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
			recursiveRemoveItem( vit );
			// update view
			lvOthersVariables_currentChanged( lvOthersVariables->currentIndex(), QModelIndex() );
		}
	}
}

void UIXUPProjectEditor::accept()
{
	// save plugins configuration
	QString plugin;
	// builder
	plugin = cbBuilders->currentText();
	if ( !gbBuilders->isChecked() )
		plugin.clear();
	mProject->setProjectSettingsValue( "BUILDER", plugin );
	// compiler
	plugin = cbCompilers->currentText();
	if ( !gbCompilers->isChecked() )
		plugin.clear();
	mProject->setProjectSettingsValue( "COMPILER", plugin );
	// debugger
	plugin = cbDebuggers->currentText();
	if ( !gbDebuggers->isChecked() )
		plugin.clear();
	mProject->setProjectSettingsValue( "DEBUGGER", plugin );
	// interpreter
	plugin = cbInterpreters->currentText();
	if ( !gbInterpreters->isChecked() )
		plugin.clear();
	mProject->setProjectSettingsValue( "INTERPRETER", plugin );
	// update menu actions
	mProject->uninstallCommands();
	mProject->installCommands();
	// accept dialog
	QDialog::accept();
}
