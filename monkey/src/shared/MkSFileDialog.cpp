#include "MkSFileDialog.h"
#include "XUPProjectItem.h"
#include "XUPProjectItemInfos.h"
#include "pMonkeyStudio.h"
#include "pTreeComboBox.h"
#include "XUPAddFiles.h"
#include "MonkeyCore.h"
#include "XUPProjectManager.h"

#include <QComboBox>

MkSFileDialog::MkSFileDialog( QWidget* parent, const QString& caption, const QString& directory, const QString& filter, bool textCodecEnabled, bool openReadOnlyEnabled )
	: pFileDialog( parent, caption, directory, filter, textCodecEnabled, openReadOnlyEnabled )
{
	mAddFiles = new XUPAddFiles( this );
	glDialog->addWidget( mAddFiles, 6, 0, 1, -1 );
	
	connect( mAddFiles, SIGNAL( currentScopeChanged( XUPItem* ) ), this, SLOT( currentScopeChanged( XUPItem* ) ) );
}

void MkSFileDialog::currentScopeChanged( XUPItem* scope )
{
	if ( scope )
	{
		QString projectPath = QDir( scope->project()->path() ).canonicalPath();
		
		if ( !directory().canonicalPath().startsWith( projectPath ) )
		{
			setDirectory( projectPath );
		}
	}
}

pFileDialogResult MkSFileDialog::getOpenProjects( QWidget* parent )
{
	pFileDialogResult result;
	QString caption = tr( "Choose project(s) to open" );
	QString dir = pMonkeyStudio::defaultProjectsDirectory();
	QString filter = XUPProjectItem::projectInfos()->projectsFilter();
	bool enabledTextCodec = true;
	bool enabledOpenReadOnly = false;
	
	MkSFileDialog fd( parent );
	setOpenFileNamesDialog( &fd, caption, dir, filter, enabledTextCodec, enabledOpenReadOnly, 0, 0 );
	fd.setTextCodec( pMonkeyStudio::defaultEncoding() );
	fd.mAddFiles->setVisible( false );
	
	if ( fd.exec() == QDialog::Accepted )
	{
		result[ "filenames" ] = fd.selectedFiles();
		result[ "codec" ] = fd.textCodec();
		result[ "openreadonly" ] = fd.openReadOnly();
	}
	
	return result;
}

pFileDialogResult MkSFileDialog::getProjectAddFiles( QWidget* parent )
{
	pFileDialogResult result;
	XUPProjectModel* model = MonkeyCore::projectsManager()->currentProjectModel();
	
	if ( model )
	{
		XUPProjectItem* curProject = MonkeyCore::projectsManager()->currentProject();
		QStringList operators = curProject->projectInfos()->operators( curProject->projectType() );
		QString caption = tr( "Choose file(s) to add to your project" );
		QString dir = pMonkeyStudio::defaultProjectsDirectory();
		QString filter = XUPProjectItem::projectInfos()->variableSuffixesFilter( curProject->projectType() );
		bool enabledTextCodec = false;
		bool enabledOpenReadOnly = false;
		
		MkSFileDialog fd( parent );
		setOpenFileNamesDialog( &fd, caption, dir, filter, enabledTextCodec, enabledOpenReadOnly, 0, 0 );
		fd.setTextCodec( pMonkeyStudio::defaultEncoding() );
		fd.mAddFiles->setModel( model );
		fd.mAddFiles->setCurrentScope( curProject );
		fd.mAddFiles->setOperators( operators );
		fd.mAddFiles->setCurrentOperator( operators.first() );
		
		if ( fd.exec() == QDialog::Accepted )
		{
			result[ "filenames" ] = fd.selectedFiles();
			result[ "scope" ] = QVariant::fromValue<XUPItem*>( fd.mAddFiles->currentScope() );
			result[ "operator" ] = fd.mAddFiles->currentOperator();
			result[ "import" ] = fd.mAddFiles->importExternalFiles();
			result[ "importpath" ] = fd.mAddFiles->importExternalFilesPath();
			result[ "directory" ] = fd.directory().absolutePath();
		}
	}
	
	return result;
}
