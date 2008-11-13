#include "UISimpleQMakeEditor.h"
#include "XUPProjectItem.h"

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
	mConfigGui.clear();
	mFileVariables = project->projectInfos()->fileVariables( project->projectType() );
	mPathVariables = project->projectInfos()->pathVariables( project->projectType() );
	QString value;
	QStringList values;
	mProject = project;
	mValues.clear();
	
	foreach ( QAbstractButton* ab, wCompilerSettings->findChildren<QAbstractButton*>() )
	{
		if ( !ab->statusTip().isEmpty() )
		{
			mConfigGui << ab->statusTip();
		}
	}
	
	mConfigGui << "app_bundle" << "lib_bundle" << "embed_manifest_exe" << "embed_manifest_dll"
		<< "designer" << "plugin" << "shared" << "dll" << "static" << "staticlib";
	
	QtVersionManager mQtManager;
	lwQtVersion->clear();
	lwQtModules->clear();
	lwModules->clear();
	
	mQtVersion = mQtManager.version( mProject->projectSettingsValue( "QT_VERSION" ) );
	
	// qt versions
	foreach ( const QtVersion& qv, mQtManager.versions() )
	{
		QListWidgetItem* it = new QListWidgetItem( qv.Version, lwQtVersion );
		it->setData( Qt::UserRole, QVariant::fromValue( qv ) );
		
		if ( qv.Default )
			it->setBackground( QBrush( Qt::green ) );
		
		if ( qv == mQtVersion )
			it->setSelected( true );
	}
	
	// qt modules
	foreach ( const QtItem& mi, mQtManager.modules() )
	{
		QListWidgetItem* it = new QListWidgetItem( mi.Text, lwQtModules );
		it->setData( Qt::UserRole, QVariant::fromValue( mi ) );
	}
	
	// configuration
	foreach ( const QtItem& ci, mQtManager.configurations() )
	{
		if ( !mConfigGui.contains( ci.Value, Qt::CaseInsensitive ) )
		{
			if ( ci.Text.toLower() != "lib only" && ci.Text.toLower() != "x11 only" && ci.Text.toLower() != "mac os x only" )
			{
				QListWidgetItem* it = new QListWidgetItem( ci.Text, lwModules );
				it->setData( Qt::UserRole, QVariant::fromValue( ci ) );
				
				if ( ci.Value.isEmpty() && ci.Variable.isEmpty() )
				{
					QFont font( it->font() );
					font.setBold( true );
					it->setFont( font );
					it->setBackground( QBrush( QColor( Qt::darkBlue ) ) );
					it->setForeground( QBrush( QColor( Qt::white ) ) );
					it->setTextAlignment( Qt::AlignCenter );
					it->setFlags( it->flags() ^ Qt::ItemIsUserCheckable );
				}
				else
					it->setCheckState( Qt::Unchecked );
			}
		}
	}

	// loading
	
	foreach ( XUPItem* child, mProject->childrenList() )
	{
		if ( child->type() == XUPItem::Variable )
		{
			QString variableName = child->attribute( "name" );
			QString op = child->attribute( "operator", "=" );
			
			if ( op != "=" && op != "+=" && op != "*=" )
			{
				continue;
			}
			
			foreach ( XUPItem* value, child->childrenList() )
			{
				XUPItem::Type type = value->type();
				
				if ( type != XUPItem::Value && type != XUPItem::File && type != XUPItem::Path )
				{
					continue;
				}
				
				QString val = QString( mValues[ variableName ].trimmed() +" " +value->attribute( "content" ) ).trimmed();
				mValues[ variableName ] = val;
			}
		}
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
		QtItem mi = item->data( Qt::UserRole ).value<QtItem>();
		
		if ( values.contains( mi.Value ) )
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
		QtItem ci = item->data( Qt::UserRole ).value<QtItem>();
		
		if ( values.contains( ci.Value ) )
		{
			item->setCheckState( Qt::Checked );
		}
		else if ( !ci.Value.isEmpty() )
		{
			item->setCheckState( Qt::Unchecked );
		}
	}
	
	// compiler settings
	values = project->splitMultiLineValue( mValues[ "CONFIG" ] );
	
	foreach ( QAbstractButton* ab, wCompilerSettings->findChildren<QAbstractButton*>() )
	{
		if ( !ab->statusTip().isEmpty() )
		{
			if ( ab == cbBuildAll )
			{
				ab->setChecked( values.contains( ab->statusTip() ) && values.contains( "debug_and_release" ) );
			}
			else
			{
				ab->setChecked( values.contains( ab->statusTip() ) );
			}
		}
		else if ( ab == cbBundle )
		{
			ab->setChecked( values.contains( "app_bundle" ) || values.contains( "lib_bundle" ) );
		}
		else if ( ab == cbManifest )
		{
			ab->setChecked( values.contains( "embed_manifest_exe" ) || values.contains( "embed_manifest_dll" ) );
		}
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
				foreach ( XUPItem* child, variableItem->childrenList() )
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
				foreach ( XUPItem* child, variableItem->childrenList() )
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
				foreach ( XUPItem* child, variableItem->childrenList() )
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
	
	qWarning() << mValues;
}
