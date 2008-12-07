#include "UIPHPQtEditor.h"
#include "XUPProjectItem.h"

#include <MkSFileDialog.h>
#include <pMonkeyStudio.h>
#include <MonkeyCore.h>
#include <PluginsManager.h>

#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

UIPHPQtEditor::UIPHPQtEditor( XUPProjectItem* project, QWidget* parent )
	: QDialog( parent )
{
	// init dialog
	setupUi( this );
	
	// set dialog infos
	setWindowIcon( project->displayIcon() );
	setWindowTitle( windowTitle().append( " - " ).append( project->displayText() ) );
	
	// set size hint for page item ( left panel )
	for ( int i = 0; i < lwPages->count(); i++ )
	{
		QListWidgetItem* item = lwPages->item( i );
		item->setSizeHint( QSize( 154, 40 ) );
	}
	
	// values editor actions
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
	
	// init proejct settings dialog
	init( project );
	
	// set correct page
	lwPages->setCurrentRow( 0 );
}

UIPHPQtEditor::~UIPHPQtEditor()
{
}

void UIPHPQtEditor::updateProjectFiles()
{
	int pType = mProject->projectType();
	foreach ( const QString& variable, mFileVariables )
	{
		QTreeWidgetItem* topItem = mProjectFilesItems.value( variable );
		QStringList files = mValues[ variable ];
		
		if ( topItem && files.isEmpty() )
		{
			delete mProjectFilesItems.take( variable );
		}
		else if ( !files.isEmpty() )
		{
			if ( !topItem )
			{
				topItem = new QTreeWidgetItem( twFiles, QTreeWidgetItem::UserType +1 );
				topItem->setText( 0, mProject->projectInfos()->displayText( pType, variable ) );
				topItem->setIcon( 0, mProject->projectInfos()->displayIcon( pType, variable ) );
				mProjectFilesItems[ variable ] = topItem;
			}
			
			for ( int i = 0; i < topItem->childCount(); i++ )
			{
				QTreeWidgetItem* item = topItem->child( i );
				QString fn = item->data( 0, Qt::UserRole ).toString();
				if ( files.contains( fn ) )
				{
					files.removeAll( fn );
				}
			}
			
			foreach ( const QString& fn, files )
			{
				QTreeWidgetItem* item = new QTreeWidgetItem( topItem, QTreeWidgetItem::UserType );
				item->setText( 0, fn );
				item->setData( 0, Qt::UserRole, fn );
				item->setIcon( 0, mProject->projectInfos()->displayIcon( XUPProjectItem::XUPProject, "FILES" ) );
			}
		}
	}
}

void UIPHPQtEditor::updateValuesEditorVariables()
{
	QListWidgetItem* curItem = lwOthersVariables->selectedItems().value( 0 );
	const QString curVariable = curItem ? curItem->text() : QString::null;
	curItem = 0;
	
	lwOthersVariables->clear();
	lwOthersValues->clear();
	
	foreach ( const QString& variable, mValues.keys() )
	{
		if ( !mManagedVariables.contains( variable ) )
		{
			lwOthersVariables->addItem( variable );
			
			if ( variable == curVariable )
			{
				curItem = lwOthersVariables->item( lwOthersVariables->count() -1 );
				curItem->setSelected( true );
			}
		}
	}
}

void UIPHPQtEditor::updateValuesEditorValues( const QString& variable )
{
	const QStringList values = mValues[ variable ];
	
	lwOthersValues->clear();
	lwOthersValues->addItems( values );
}

void UIPHPQtEditor::init( XUPProjectItem* project )
{
	mFileVariables = project->projectInfos()->fileVariables( project->projectType() );
	mProject = project;
	mValues.clear();
	mManagedVariables.clear();
	mManagedVariables << "FORMS" << "PHP_FILES";
	
	leProjectName->setText( mProject->attribute( "name" ) );
	QString mainFile = mProject->projectSettingsValue( "MAIN_FILE", "src/main.php" );

	// loading datas from variables of root scope
	foreach ( XUPItem* child, mProject->childrenList() )
	{
		if ( child->type() == XUPItem::Variable )
		{
			QString variableName = child->attribute( "name" );
			
			foreach ( XUPItem* value, child->childrenList() )
			{
				XUPItem::Type type = value->type();
				QStringList values = mValues[ variableName ];
				
				if ( type != XUPItem::Value && type != XUPItem::File && type != XUPItem::Path )
				{
					continue;
				}
				
				values << value->attribute( "content" );
				mValues[ variableName ] = values;
			}
		}
	}

	updateProjectFiles();
	updateValuesEditorVariables();
}

XUPItem* UIPHPQtEditor::getUniqueVariableItem( const QString& variableName, bool create )
{
	const QStringList mOperators = QStringList( "=" );
	XUPItemList variables = mProject->getVariables( mProject, variableName, 0, false );
	XUPItem* variableItem = 0;
	
	// remove duplicate variables
	foreach ( XUPItem* variable, variables )
	{
		QString op = variable->attribute( "operator", "=" );
		
		if ( !variableItem && mOperators.contains( op ) )
		{
			variableItem = variable;
		}
		else if ( mOperators.contains( op ) )
		{
			variable->parent()->removeChild( variable );
		}
	}
	
	// create it if needed
	if ( !variableItem && create )
	{
		variableItem = mProject->addChild( XUPItem::Variable );
		variableItem->setAttribute( "name", variableName );
	}
	
	// return item
	return variableItem;
}

void UIPHPQtEditor::on_tbAddFile_clicked()
{
	pFileDialogResult result = MkSFileDialog::getProjectAddFiles( window(), false );
	
	if ( !result.isEmpty() )
	{
		QStringList files = result[ "filenames" ].toStringList();
		
		// import files if needed
		if ( result[ "import" ].toBool() )
		{
			const QString projectPath = mProject->path();
			const QString importPath = result[ "importpath" ].toString();
			const QString importRootPath = result[ "directory" ].toString();
			QDir dir( importRootPath );
			
			for ( int i = 0; i < files.count(); i++ )
			{
				if ( !files.at( i ).startsWith( projectPath ) )
				{
					QString fn = QString( files.at( i ) ).remove( importRootPath ).replace( "\\", "/" );
					fn = QDir::cleanPath( QString( "%1/%2/%3" ).arg( projectPath ).arg( importPath ).arg( fn ) );
					
					if ( dir.mkpath( QFileInfo( fn ).absolutePath() ) && QFile::copy( files.at( i ), fn ) )
					{
						files[ i ] = fn;
					}
				}
			}
		}
		
		// add files
		foreach ( QString fn, files )
		{
			fn = mProject->relativeFilePath( fn );
			
			QString variable = mProject->projectInfos()->variableNameForFileName( mProject->projectType(), fn );
			
			if ( !mValues[ variable ].contains( fn ) )
			{
				mValues[ variable ] += " " +fn;
			}
		}
		
		updateProjectFiles();
	}
}

void UIPHPQtEditor::on_tbEditFile_clicked()
{
	QTreeWidgetItem* item = twFiles->selectedItems().value( 0 );
	if ( item && twFiles->indexOfTopLevelItem( item ) == -1 )
	{
		bool ok;
		QString oldValue = item->data( 0, Qt::UserRole ).toString();
		QString fn = QInputDialog::getText( this, tr( "Edit file name" ), tr( "Type a new name for this file" ), QLineEdit::Normal, oldValue, &ok );
		if ( ok && !fn.isEmpty() )
		{
			QString variable = mProject->projectInfos()->variableNameForFileName( mProject->projectType(), fn );
			
			item->setText( 0, fn );
			item->setData( 0, Qt::UserRole, fn );
			
			mValues[ variable ].removeAll( oldValue );
			mValues[ variable ] << fn;
			
			updateProjectFiles();
		}
	}
}

void UIPHPQtEditor::on_tbRemoveFile_clicked()
{
	QList<QTreeWidgetItem*> selectedItems = twFiles->selectedItems();
	
	if ( selectedItems.count() > 0 )
	{
		if ( QMessageBox::question( this, tr( "Remove files" ), tr( "Are you sure you want to remove all the selected files ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
		{
			return;
		}
		
		foreach ( QTreeWidgetItem* item, selectedItems )
		{
			if ( item->type() == QTreeWidgetItem::UserType +1 )
			{
				continue;
			}
			
			const QString variable = mProjectFilesItems.key( item->parent() );
			const QString fn = item->data( 0, Qt::UserRole ).toString();
			
			mValues[ variable ].removeAll( fn );
			delete item;
		}
		
		if ( !selectedItems.isEmpty() )
		{
			updateProjectFiles();
		}
	}
}

void UIPHPQtEditor::on_lwOthersVariables_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous )
{
	// enable/disable actions
	gbOthersValues->setEnabled( current );
	tbOthersVariablesEdit->setEnabled( current );
	tbOthersVariablesRemove->setEnabled( current );
	
	// save previous variable datas
	if ( previous )
	{
		const QString variable = previous->text();
		QStringList values;
		
		for ( int i = 0; i < lwOthersValues->count(); i++ )
		{
			values << lwOthersValues->item( i )->text();
		}
		
		mValues[ variable ] = values;
	}
	
	// update values view
	const QString variable = current ? current->text() : QString::null;
	updateValuesEditorValues( variable );
}

void UIPHPQtEditor::on_tbOthersVariablesAdd_clicked()
{
	bool ok;
	const QStringList variables = mProject->projectInfos()->knowVariables( mProject->projectType() );
	const QString variable = QInputDialog::getItem( window(), tr( "Add variable..." ), tr( "Select a variable name or enter a new one" ), variables, 0, true, &ok );
	
	if ( !variable.isEmpty() && ok )
	{
		if ( !mValues.keys().contains( variable ) && !mManagedVariables.contains( variable ) )
		{
			QListWidgetItem* item = new QListWidgetItem( variable, lwOthersVariables );
			lwOthersVariables->setCurrentItem( item );
			
			mValues[ variable ].clear();
			mVariablesToRemove.removeAll( variable );
		}
		else
		{
			pMonkeyStudio::information( tr( "Information..." ), tr( "This variable already exists or is filtered out." ), window() );
		}
	}
}

void UIPHPQtEditor::on_tbOthersVariablesEdit_clicked()
{
	QListWidgetItem* item = lwOthersVariables->currentItem();
	
	if ( !item )
	{
		return;
	}
	
	bool ok;
	QString oldVariable = item->text();
	QString variable = QInputDialog::getText( window(), tr( "Edit variable..." ), tr( "Enter a new name for this variable" ), QLineEdit::Normal, oldVariable, &ok );
	
	if ( !variable.isEmpty() && ok )
	{
		if ( !mValues.keys().contains( variable ) && !mManagedVariables.contains( variable ) )
		{
			item->setText( variable );
			
			mValues.remove( oldVariable );
			if ( !mVariablesToRemove.contains( oldVariable ) )
			{
				mVariablesToRemove << oldVariable;
			}
		}
		else
		{
			pMonkeyStudio::information( tr( "Information..." ), tr( "This variable already exists or is filtered out." ), window() );
		}
	}
}

void UIPHPQtEditor::on_tbOthersVariablesRemove_clicked()
{
	QListWidgetItem* item = lwOthersVariables->currentItem();
	
	if ( !item )
	{
		return;
	}
	
	// confirm user request
	if ( pMonkeyStudio::question( tr( "Remove a variable..." ), tr( "A you sure you want to remove this variable and all its content ?" ) ) )
	{
		QString variable = item->text();
		
		lwOthersValues->clear();
		delete item;
		
		mValues.remove( variable );
		if ( !mVariablesToRemove.contains( variable ) )
		{
			mVariablesToRemove << variable;
		}
	}
}

void UIPHPQtEditor::on_lwOthersValues_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous )
{
	// enable button according to item validity
	tbOthersValuesEdit->setEnabled( current );
	tbOthersValuesRemove->setEnabled( current );
	Q_UNUSED( previous );
}

void UIPHPQtEditor::on_tbOthersValuesAdd_clicked()
{
	on_tbOthersValuesAdd_triggered( aOthersValuesAddValue );
}

void UIPHPQtEditor::on_tbOthersValuesAdd_triggered( QAction* action )
{
	QListWidgetItem* variableItem = lwOthersVariables->currentItem();
	
	if ( variableItem )
	{
		const QString title = tr( "Add a value..." );
		bool ok = true;
		QString val;
		
		if ( action == aOthersValuesAddValue )
		{
			val	= QInputDialog::getText( window(), title, tr( "Enter the value :" ), QLineEdit::Normal, QString(), &ok );
			if ( !ok )
			{
				val.clear();
			}
		}
		else if ( action == aOthersValuesAddFile )
		{
			val = QFileDialog::getOpenFileName( window(), tr( "Choose a file" ), mProject->path() );
			if ( !val.isEmpty() )
			{
				val = mProject->relativeFilePath( val );
			}
		}
		else if ( action == aOthersValuesAddPath )
		{
			val = QFileDialog::getExistingDirectory( window(), tr( "Choose a path" ), mProject->path() );
			if ( !val.isEmpty() )
			{
				val = mProject->relativeFilePath( val );
			}
		}
		
		if ( !val.isEmpty() )
		{
			// check if value exists
			for ( int i = 0; i < lwOthersValues->count(); i++ )
			{
				QListWidgetItem* valueItem = lwOthersValues->item( i );
				
				if ( valueItem->text() == val )
				{
					lwOthersValues->setCurrentItem( valueItem );
					return;
				}
			}
			
			// create value item
			QListWidgetItem* valueItem = new QListWidgetItem( val, lwOthersValues );
			lwOthersValues->setCurrentItem( valueItem );
		}
	}
}

void UIPHPQtEditor::on_tbOthersValuesEdit_clicked()
{
	on_tbOthersValuesEdit_triggered( aOthersValuesEditValue );
}

void UIPHPQtEditor::on_tbOthersValuesEdit_triggered( QAction* action )
{
	QListWidgetItem* valueItem = lwOthersValues->currentItem();
	
	if ( valueItem )
	{
		const QString title = tr( "Edit a value..." );
		bool ok = true;
		QString oldValue = valueItem->text();
		QString val;
		
		if ( action == aOthersValuesEditValue )
		{
			val	= QInputDialog::getText( window(), title, tr( "Edit the value :" ), QLineEdit::Normal, oldValue, &ok );
			if ( !ok )
			{
				val.clear();
			}
		}
		else if ( action == aOthersValuesEditFile )
		{
			val = QFileDialog::getOpenFileName( window(), tr( "Choose a file" ), oldValue );
			if ( !val.isEmpty() )
			{
				val = mProject->relativeFilePath( val );
			}
		}
		else if ( action == aOthersValuesEditPath )
		{
			val = QFileDialog::getExistingDirectory( window(), tr( "Choose a path" ), oldValue );
			if ( !val.isEmpty() )
			{
				val = mProject->relativeFilePath( val );
			}
		}
		
		if ( !val.isEmpty() )
		{
			// check if value exists
			for ( int i = 0; i < lwOthersValues->count(); i++ )
			{
				QListWidgetItem* item = lwOthersValues->item( i );
				
				if ( item->text() == val )
				{
					lwOthersValues->setCurrentItem( item );
					return;
				}
			}
			
			// update item
			valueItem->setText( val );
		}
	}
}

void UIPHPQtEditor::on_tbOthersValuesRemove_clicked()
{
	QListWidgetItem* valueItem = lwOthersValues->currentItem();
	
	if ( valueItem )
	{
		// confirm user request
		if ( !pMonkeyStudio::question( tr( "Remove a value..." ), tr( "A you sure you want to remove this value ?" ) ) )
		{
			return;
		}
		
		// remove value
		delete valueItem;
	}
}

void UIPHPQtEditor::on_tbOthersValuesClear_clicked()
{
	QListWidgetItem* variableItem = lwOthersVariables->currentItem();
	
	if ( variableItem )
	{
		// request user confirm
		if ( pMonkeyStudio::question( tr( "Clear values..." ), tr( "A you sure you want to clear these values ?" ) ) )
		{
			lwOthersValues->clear();
		}
	}
}

void UIPHPQtEditor::accept()
{
	// tell about variables to remove
	foreach ( const QString& variable, mVariablesToRemove )
	{
		mValues[ variable ].clear();
	}
	
	foreach ( const QString& variable, mValues.keys() )
	{
		bool isEmpty = mValues[ variable ].isEmpty();
		XUPItem* variableItem = getUniqueVariableItem( variable, !isEmpty );
		
		if ( !variableItem )
		{
			continue;
		}
		
		if ( !isEmpty )
		{
			if ( mFileVariables.contains( variable ) )
			{
				// get values
				QStringList values = mValues[ variable ];
				
				// update variable
				variableItem->setAttribute( "operator", "=" );
				variableItem->setAttribute( "multiline", "true" );
				
				// remove all child
				foreach ( XUPItem* child, variableItem->childrenList() )
				{
					if ( child->type() == XUPItem::File )
					{
						QString value = child->attribute( "content" );
						if ( values.contains( value ) )
						{
							values.removeAll( value );
						}
						else if ( !values.contains( value ) )
						{
							variableItem->removeChild( child );
						}
					}
				}
				
				// add new ones
				foreach ( const QString& v, values )
				{
					XUPItem* value = variableItem->addChild( XUPItem::File );
					value->setAttribute( "content", v );
				}
			}
			else
			{
				// update variable
				variableItem->setAttribute( "operator", "=" );
				variableItem->setAttribute( "multiline", "false" );
				
				// remove all child values
				foreach ( XUPItem* child, variableItem->childrenList() )
				{
					if ( child->type() == XUPItem::Value )
					{
						variableItem->removeChild( child );
					}
				}
				
				// add new ones
				foreach ( const QString& v, mValues[ variable ] )
				{
					XUPItem* value = variableItem->addChild( XUPItem::Value );
					value->setAttribute( "content", v );
				}
			}
		}
		else if ( isEmpty && variableItem && variableItem->childCount() > 0 )
		{
			// remove all child values
			foreach ( XUPItem* child, variableItem->childrenList() )
			{
				if ( child->type() == XUPItem::Value || child->type() == XUPItem::File || child->type() == XUPItem::Path )
				{
					variableItem->removeChild( child );
				}
			}
		}
		
		// remove empty variable
		if ( variableItem->childCount() == 0 )
		{
			variableItem->parent()->removeChild( variableItem );
		}
	}
	
	// xup settings
	mProject->setAttribute( "name", leProjectName->text() );
	mProject->setProjectSettingsValue( "MAIN_FILE", cbProjectMainFile->currentText() );
	
	// update menu actions
	mProject->uninstallCommands();
	mProject->installCommands();
	
	// close dialog
	QDialog::accept();
}
