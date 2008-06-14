#include "QtAssistantConfig.h"
#include "QtAssistant.h"

// assistant include
#include "config.h"
#include "docuparser.h"

#include <QLibraryInfo>

static Config* mConfig = 0;

Config* QtAssistantConfig::instance( QtAssistant* plugin )
{
	const QString docPath = QDir::toNativeSeparators( QDir::cleanPath( plugin->settingsValue( "DocPath", QLibraryInfo::location( QLibraryInfo::DocumentationPath ) ).toString() ) );
	if ( !mConfig )
		mConfig = Config::loadConfig( QString() );
	// load default profile
	if ( mConfig->defaultProfileExists() )
	{
		mConfig->profil = new Profile();
		mConfig->loadDefaultProfile();
		mConfig->load();
		mConfig->setDocRebuild( false );
		// check profile validity
		const QString fn = QDir::toNativeSeparators( QDir::cleanPath( mConfig->docFiles().value( 0 ) ) );
		if ( fn.startsWith( docPath, Qt::CaseInsensitive ) )
			return mConfig;
		delete mConfig->profil;
	}
	mConfig->profil = Profile::createDefaultProfile( docPath );
	mConfig->saveProfile( mConfig->profil );
	mConfig->loadDefaultProfile();
	mConfig->setDocRebuild( true );
	mConfig->save();
	return mConfig;
}
