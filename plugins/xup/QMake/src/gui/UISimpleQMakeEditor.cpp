#include "UISimpleQMakeEditor.h"
#include "../../../core/XUPProjectItem.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

UISimpleQMakeEditor::UISimpleQMakeEditor( XUPProjectItem* project, QWidget* parent )
	: QDialog( parent )
{	
	setupUi( this );
	
	setWindowIcon( project->displayIcon() );
	setWindowTitle( windowTitle().append( " - " ).append( project->displayText() ) );
	
	foreach ( QRadioButton* rb, gbProjectType->findChildren<QRadioButton*>() )
	{
		connect( rb, SIGNAL( toggled( bool ) ), this, SLOT( projectTypeChanged() ) );
	}
	
#warning should use settings from qmake plugin setttings
	QListWidgetItem* coreItem = new QListWidgetItem( lwQtModules );
	coreItem->setText( "QtCore" );
	coreItem->setData( Qt::UserRole, "core" );
	
	QListWidgetItem* guiItem = new QListWidgetItem( lwQtModules );
	guiItem->setText( "QtGui" );
	guiItem->setData( Qt::UserRole, "gui" );
	
	QListWidgetItem* networkItem = new QListWidgetItem( lwQtModules );
	networkItem->setText( "QtNetwork" );
	networkItem->setData( Qt::UserRole, "network" );
	
	QListWidgetItem* svgItem = new QListWidgetItem( lwQtModules );
	svgItem->setText( "QtSvg" );
	svgItem->setData( Qt::UserRole, "svg" );
	
	QListWidgetItem* designerItem = new QListWidgetItem( lwModules );
	designerItem->setText( "Designer" );
	designerItem->setData( Qt::UserRole, "designer" );
	
	QListWidgetItem* phononItem = new QListWidgetItem( lwModules );
	phononItem->setText( "Phonon" );
	phononItem->setData( Qt::UserRole, "phonon" );
	
	QListWidgetItem* webkitkItem = new QListWidgetItem( lwModules );
	webkitkItem->setText( "WebKit" );
	webkitkItem->setData( Qt::UserRole, "webkit" );
	
	init( project );
	
	lwPages->setCurrentRow( 1 );
}

UISimpleQMakeEditor::~UISimpleQMakeEditor()
{
}

void UISimpleQMakeEditor::updateProjectFiles()
{
	int pType = mProject->projectType();
	foreach ( const QString& variable, mFileVariables )
	{
		QTreeWidgetItem* topItem = mProjectFilesItems.value( variable );
		QStringList files = mProject->splitMultiLineValue( mValues[ variable ] );
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

void UISimpleQMakeEditor::init( XUPProjectItem* project )
{
	mFileVariables = project->projectInfos()->fileVariables( project->projectType() );
	mPathVariables = project->projectInfos()->pathVariables( project->projectType() );
	QString value;
	QStringList values;
	mProject = project;
	mValues.clear();
	
	mValues[ "TEMPLATE" ] = project->interpretVariable( "TEMPLATE" );
	mValues[ "CONFIG" ] = project->interpretVariable( "CONFIG" );
	mValues[ "QT" ] = project->interpretVariable( "QT" );
	mValues[ "TARGET" ] = project->interpretVariable( "TARGET" );
	mValues[ "DESTDIR" ] = project->interpretVariable( "DESTDIR" );
	mValues[ "DLLDESTDIR" ] = project->interpretVariable( "DLLDESTDIR" );
	
	foreach ( const QString& variable, mFileVariables )
	{
		mValues[ variable ] = project->interpretVariable( variable );
	}
	
	// update gui
	
	// project
	value = mValues[ "TEMPLATE" ];
	if ( value == "app" )
	{
		rbApplication->setChecked( true );
	}
	else if ( value == "subdirs" )
	{
		rbSubdirs->setChecked( true );
	}
	else if ( value == "lib" )
	{
		values = project->splitMultiLineValue( mValues[ "CONFIG" ] );
		if ( values.contains( "designer" ) )
		{
			rbQtDesignerPlugin->setChecked( true );
		}
		else if ( values.contains( "plugin" ) )
		{
			rbQtPlugin->setChecked( true );
		}
		else if ( values.contains( "shared" ) || values.contains( "dll" ) )
		{
			rbSharedLib->setChecked( true );
		}
		else if ( values.contains( "static" ) || values.contains( "staticlib" ) )
		{
			rbStaticLib->setChecked( true );
		}
	}
	
	if ( !rbSubdirs->isChecked() )
	{
		leProjectName->setText( mValues[ "TARGET" ] );
		if ( rbApplication->isChecked() || rbStaticLib->isChecked() )
		{
			leProjectTarget->setText( mValues[ "DESTDIR" ] );
		}
		else
		{
			leProjectTarget->setText( mValues[ "DLLDESTDIR" ] );
		}
	}
	
	// modules
	values = project->splitMultiLineValue( mValues[ "QT" ] );
	for ( int i = 0; i < lwQtModules->count(); i++ )
	{
		QListWidgetItem* item = lwQtModules->item( i );
		if ( values.contains( item->data( Qt::UserRole ).toString() ) )
		{
			item->setCheckState( Qt::Checked );
		}
		else
		{
			item->setCheckState( Qt::Unchecked );
		}
	}
	
	values = project->splitMultiLineValue( mValues[ "CONFIG" ] );
	for ( int i = 0; i < lwModules->count(); i++ )
	{
		QListWidgetItem* item = lwModules->item( i );
		if ( values.contains( item->data( Qt::UserRole ).toString() ) )
		{
			item->setCheckState( Qt::Checked );
		}
		else
		{
			item->setCheckState( Qt::Unchecked );
		}
	}
	
	// compiler settings
	values = project->splitMultiLineValue( mValues[ "CONFIG" ] );
	if ( values.contains( "debug_and_release" ) )
	{
		rbDebugRelease->setChecked( true );
		if ( values.contains( "build_all" ) )
		{
			cbBuildAll->setChecked( true );
		}
	}
	else if ( values.contains( "debug" ) )
	{
		rbDebug->setChecked( true );
	}
	else if ( values.contains( "release" ) )
	{
		rbRelease->setChecked( true );
	}
	
	if ( values.contains( "exceptions" ) )
	{
		cbExceptions->setChecked( true );
	}
	
	if ( values.contains( "rtti" ) )
	{
		cbRtti->setChecked( true );
	}
	
	if ( values.contains( "stl" ) )
	{
		cbStl->setChecked( true );
	}
	
	if ( values.contains( "thread" ) )
	{
		cbThread->setChecked( true );
	}
	
	if ( values.contains( "app_bundle" ) || values.contains( "lib_bundle" ) )
	{
		cbBundle->setChecked( true );
	}
	
	if ( values.contains( "ppc" ) )
	{
		cbPpc->setChecked( true );
	}
	
	if ( values.contains( "x86" ) )
	{
		cbX86->setChecked( true );
	}
	
	if ( values.contains( "x11" ) )
	{
		cbX11->setChecked( true );
	}
	
	if ( values.contains( "warn_on" ) )
	{
		rbWarningOn->setChecked( true );
	}
	
	if ( values.contains( "warn_off" ) )
	{
		rbWarningOff->setChecked( true );
	}
	
	if ( values.contains( "console" ) )
	{
		cbConsole->setChecked( true );
	}
	
	if ( values.contains( "embed_manifest_dll" ) || values.contains( "embed_manifest_exe" ) )
	{
		cbManifest->setChecked( true );
	}
	
	if ( values.contains( "windows" ) )
	{
		cbWindows->setChecked( true );
	}
	
	updateProjectFiles();
}

XUPItem* UISimpleQMakeEditor::getUniqueVariableItem( const QString& variableName, bool create )
{
	XUPItemList variables = mProject->getVariables( mProject, variableName, 0, false );
	if ( variables.isEmpty() )
	{
		if ( create )
		{
			XUPItem* variableItem = mProject->addChild( XUPItem::Variable );
			variableItem->setAttribute( "name", variableName );
			return variableItem;
		}
		
		return 0;
	}
	else if ( variables.count() == 1 )
	{
		return variables.first();
	}
	else
	{
		foreach ( XUPItem* variable, variables )
		{
			if ( variable != variables.first() )
			{
				variable->parent()->removeChild( variable );
			}
		}
	}
	
	return variables.first();
}

void UISimpleQMakeEditor::projectTypeChanged()
{
	bool isSubdirs = rbSubdirs->isChecked();
	leProjectName->setDisabled( isSubdirs );
	leProjectTarget->setDisabled( isSubdirs );
	tbProjectTarget->setDisabled( isSubdirs );
}

void UISimpleQMakeEditor::on_tbProjectTarget_clicked()
{
	QString path = QFileDialog::getExistingDirectory( this, tr( "Choose a target path for your project" ), mProject->filePath( leProjectTarget->text() ) );
	if ( !path.isEmpty() )
	{
		leProjectTarget->setText( mProject->relativeFilePath( path ) );
	}
}

void UISimpleQMakeEditor::on_tbAddFile_clicked()
{
	const QString filters = mProject->projectInfos()->variableSuffixesFilter( mProject->projectType() );
	QStringList files = QFileDialog::getOpenFileNames( this, tr( "Choose the file(s) to add to your project" ), mProject->path(), filters );
	
	if ( !files.isEmpty() )
	{
		foreach ( QString fn, files )
		{
			fn = mProject->relativeFilePath( fn );
			
			if ( fn.contains( " " ) )
			{
				fn.prepend( '"' ).append( '"' );
			}
			
			QString variable = mProject->projectInfos()->variableNameForFileName( mProject->projectType(), fn );
			
			if ( !mValues[ variable ].contains( fn ) )
			{
				mValues[ variable ] += " " +fn;
			}
		}
		
		updateProjectFiles();
	}
}

void UISimpleQMakeEditor::on_tbEditFile_clicked()
{
	QTreeWidgetItem* item = twFiles->selectedItems().value( 0 );
	if ( item && twFiles->indexOfTopLevelItem( item ) == -1 )
	{
		bool ok;
		QString oldValue = item->data( 0, Qt::UserRole ).toString();
		QString fn = QInputDialog::getText( this, tr( "Edit filen ame" ), tr( "Type a new name for this file" ), QLineEdit::Normal, oldValue, &ok );
		if ( ok && !fn.isEmpty() )
		{
			QString variable = mProject->projectInfos()->variableNameForFileName( mProject->projectType(), fn );
			
			item->setText( 0, fn );
			item->setData( 0, Qt::UserRole, fn );
			
			mValues[ variable ].remove( oldValue ).append( " " +fn );
			
			updateProjectFiles();
		}
	}
}

void UISimpleQMakeEditor::on_tbRemoveFile_clicked()
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
			
			mValues[ variable ].remove( fn );
			delete item;
		}
		
		if ( !selectedItems.isEmpty() )
		{
			updateProjectFiles();
		}
	}
}

void UISimpleQMakeEditor::accept()
{
	QString tmplate;
	QStringList config;
	QStringList qt;
	QStringList target;
	QString destdir;
	QString dlldestdir;
	
	// project
	if ( rbApplication->isChecked() )
	{
		tmplate = "app";
	}
	else if ( rbSubdirs->isChecked() )
	{
		tmplate = "subdirs";
	}
	else
	{
		tmplate = "lib";
		if ( rbQtDesignerPlugin->isChecked() )
		{
			config << "designer";
		}
		else if ( rbQtPlugin->isChecked() )
		{
			config << "plugin";
		}
		else if ( rbSharedLib->isChecked() )
		{
			config << "dll";
		}
		else if ( rbStaticLib->isChecked() )
		{
			config << "staticlib";
		}
	}
	
	if ( !rbSubdirs->isChecked() )
	{
		target << leProjectName->text();
		if ( rbApplication->isChecked() || rbStaticLib->isChecked() )
		{
			destdir = leProjectTarget->text();
		}
		else
		{
			dlldestdir = leProjectTarget->text();
		}
	}
	
	// modules
	for ( int i = 0; i < lwQtModules->count(); i++ )
	{
		QListWidgetItem* item = lwQtModules->item( i );
		if ( item->checkState() == Qt::Checked )
		{
			qt << item->data( Qt::UserRole ).toString();
		}
	}
	
	for ( int i = 0; i < lwModules->count(); i++ )
	{
		QListWidgetItem* item = lwModules->item( i );
		if ( item->checkState() == Qt::Checked )
		{
			config << item->data( Qt::UserRole ).toString();
		}
	}
	
	// compiler settings
	foreach ( QAbstractButton* ab, wCompilerSettings->findChildren<QAbstractButton*>() )
	{
		if ( !ab->isChecked() || !ab->isEnabled() )
		{
			continue;
		}
		
		if ( !ab->statusTip().isEmpty() )
		{
			config << ab->statusTip();
		}
		else
		{
			if ( ab == cbManifest )
			{
				if ( tmplate == "app" )
				{
					config << "embed_manifest_exe";
				}
				else if ( tmplate == "lib" )
				{
					config << "embed_manifest_dll";
				}
			}
			else if ( ab == cbBundle )
			{
				if ( tmplate == "app" )
				{
					config << "app_bundle";
				}
				else if ( tmplate == "lib" )
				{
					config << "lib_bundle";
				}
			}
		}
	}
	
	mValues[ "TEMPLATE" ] = tmplate;
	mValues[ "CONFIG" ] = config.join( " " );
	mValues[ "QT" ] = qt.join( " " );
	mValues[ "TARGET" ] = target.join( " " );
	mValues[ "DESTDIR" ] = destdir;
	mValues[ "DLLDESTDIR" ] = dlldestdir;
	
	foreach ( const QString& variable, mValues.keys() )
	{
		bool isEmpty = mValues[ variable ].trimmed().isEmpty();
		XUPItem* variableItem = getUniqueVariableItem( variable, !isEmpty );
		if ( !variableItem )
		{
			continue;
		}
		
		if ( !isEmpty )
		{
			if ( mFileVariables.contains( variable ) || mPathVariables.contains( variable ) )
			{
				// get child type
				XUPItem::Type type = mFileVariables.contains( variable ) ? XUPItem::File : XUPItem::Path;
				// get values
				QStringList values = mProject->splitMultiLineValue( mValues[ variable ] );
				
				// update variable
				variableItem->setAttribute( "operator", "=" );
				variableItem->setAttribute( "multiline", "true" );
				
				// remove all child
				foreach ( XUPItem* child, variableItem->children() )
				{
					if ( child->type() == type )
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
					XUPItem* value = variableItem->addChild( type );
					value->setAttribute( "content", v );
				}
			}
			else if ( variable == "CONFIG" )
			{
				// update variable
				variableItem->setAttribute( "operator", "+=" );
				variableItem->setAttribute( "multiline", "false" );
				
				// remove all child values
				foreach ( XUPItem* child, variableItem->children() )
				{
					if ( child->type() == XUPItem::Value )
					{
						variableItem->removeChild( child );
					}
				}
				
				// add new one
				XUPItem* value = variableItem->addChild( XUPItem::Value );
				value->setAttribute( "content", mValues[ variable ] );
			}
			else
			{
				// update variable
				variableItem->setAttribute( "operator", "=" );
				variableItem->setAttribute( "multiline", "false" );
				
				// remove all child values
				foreach ( XUPItem* child, variableItem->children() )
				{
					if ( child->type() == XUPItem::Value )
					{
						variableItem->removeChild( child );
					}
				}
				
				// add new one
				XUPItem* value = variableItem->addChild( XUPItem::Value );
				value->setAttribute( "content", mValues[ variable ] );
			}
		}
		else if ( isEmpty && variableItem && variableItem->childCount() > 0 )
		{
			// remove all child values
			foreach ( XUPItem* child, variableItem->children() )
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
	
	qWarning() << mValues;
}
