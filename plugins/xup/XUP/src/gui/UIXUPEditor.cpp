#include "UIXUPEditor.h"
#include "XUPProjectItem.h"
#include "XUPProjectItemHelper.h"

#include <MkSFileDialog.h>
#include <pMonkeyStudio.h>
#include <MonkeyCore.h>
#include <PluginsManager.h>

#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

UIXUPEditor::UIXUPEditor( XUPProjectItem* project, QWidget* parent )
	: QDialog( parent )
{
	// init dialog
	setupUi( this );
	
	// set dialog infos
	setWindowIcon( project->displayIcon() );
	setWindowTitle( tr( "%1 Project Editor - %2" ).arg( PLUGIN_NAME ).arg( project->displayText() ) );
	
	// set size hint for page item ( left panel )
	for ( int i = 0; i < lwPages->count(); i++ )
	{
		QListWidgetItem* item = lwPages->item( i );
		item->setSizeHint( QSize( 154, 40 ) );
	}
	
	// commands
	const BasePluginTypeList types = BasePluginTypeList() << BasePlugin::iDebugger << BasePlugin::iInterpreter;
	const QStringList parsers = MonkeyCore::consoleManager()->parsersName();
	
	// commands editor
	ceEditor->setCommandTypes( types );
	ceEditor->setParsers( parsers );
	
	// init project settings dialog
	init( project );
	
	// set correct page
	lwPages->setCurrentRow( 0 );
}

UIXUPEditor::~UIXUPEditor()
{
}

void UIXUPEditor::updateMainFileComboBox( const QString& selectFile )
{
	cbMainFile->clear();
	
	const QStringList sources = mProject->sourceFiles();
	QMap<QString, QString> files;
	
	foreach ( const QString& file, sources )
	{
		const QString fileName = mProject->relativeFilePath( file );
		
		files[ fileName.toLower() ] = fileName;
	}
	
	cbMainFile->addItems( files.values() );
	const int index = cbMainFile->findText( mProject->relativeFilePath( selectFile ) );
	cbMainFile->setCurrentIndex( index );
}

void UIXUPEditor::init( XUPProjectItem* project )
{
	mProject = project;

	leProjectName->setText( mProject->attribute( "name" ) );
	updateMainFileComboBox( mProject->projectSettingsValue( "MAIN_FILE" ) );
	ceEditor->setCommands( XUPProjectItemHelper::projectCommands( mProject ) );
	ceEditor->setCurrentType( ceEditor->commandTypes().first() );
}

void UIXUPEditor::accept()
{
	ceEditor->finalize();
	mProject->setAttribute( "name", leProjectName->text() );
	mProject->setProjectSettingsValue( "MAIN_FILE", cbMainFile->currentText() );
	XUPProjectItemHelper::setProjectCommands( mProject, ceEditor->commands() );
	// close dialog
	QDialog::accept();
}
