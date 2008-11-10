#include "MkSFileDialog.h"
#include "XUPProjectItem.h"
#include "XUPProjectItemInfos.h"
#include "pMonkeyStudio.h"

MkSFileDialog::MkSFileDialog( QWidget* parent, const QString& caption, const QString& directory, const QString& filter, bool textCodecEnabled, bool openReadOnlyEnabled )
	: pFileDialog( parent, caption, directory, filter, textCodecEnabled, openReadOnlyEnabled )
{
}

pFileDialogResult MkSFileDialog::getOpenProjects( QWidget* parent )
{
	QString caption = tr( "Choose project(s) to open" );
	QString dir = pMonkeyStudio::defaultProjectsDirectory();
	QString filter = XUPProjectItem::projectInfos()->projectsFilter();
	bool enabledTextCodec = true;
	bool enabledOpenReadOnly = false;
	pFileDialogResult result;
	
	pFileDialog fd( parent );
	setOpenFileNamesDialog( &fd, caption, dir, filter, enabledTextCodec, enabledOpenReadOnly, 0, 0 );
	fd.setTextCodec( pMonkeyStudio::defaultEncoding() );
	
	if ( fd.exec() == QDialog::Accepted )
	{
		result[ "filenames" ] = fd.selectedFiles();
		result[ "codec" ] = fd.textCodec();
		result[ "openreadonly" ] = fd.openReadOnly();
	}
	
	return result;
}
