#include "QtAssistantConfig.h"
#include "QtAssistant.h"

// assistant include
#include "config.h"
#include "docuparser.h"

#include <QLibraryInfo>

Config* QtAssistantConfig::instance( QtAssistant* plugin )
{
	// /usr/share/qt4/doc
	const QString profileFileName;
	const QString docPath = plugin->settingsValue( "DocPath", QLibraryInfo::location( QLibraryInfo::DocumentationPath ) ).toString();
	
	Config* config = new Config();
	if ( profileFileName.isEmpty() )
	{
        if ( !config->defaultProfileExists() )
		{
            config->profil = Profile::createDefaultProfile( docPath );
            config->saveProfile( config->profil );
        }
		else
		{
            config->profil = new Profile();
        }
        config->loadDefaultProfile();
        config->load();
		return config;
    }

    QFile file( profileFileName );
    if ( !file.exists() )
	{
        qWarning( "File does not exist: %s", qPrintable( profileFileName ) );
        return 0;
    }
    DocuParser* parser = DocuParser::createParser( profileFileName );
    if ( !parser )
	{
        qWarning( "Failed to create parser for file: %s", qPrintable( profileFileName ) );
        return 0;
    }
    if ( parser->parserVersion() < DocuParser::Qt320 )
	{
        qWarning( "File does not contain profile information" );
        return 0;
    }
    DocuParser320* profileParser = static_cast<DocuParser320*>( parser );
    parser->parse( &file );
    config->profil = profileParser->profile();
    if ( !config->profil )
	{
        qWarning( "Config::loadConfig(), no profile in: %s", qPrintable( profileFileName ) );
        return 0;
    }
    config->profil->setProfileType( Profile::UserProfile );
    config->profil->setDocuParser( profileParser );
    config->load();
	return config;
}
