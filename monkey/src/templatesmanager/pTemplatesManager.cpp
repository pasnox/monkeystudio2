/*****************************************************
* AUTHOR   : Nox P@sNox
* PROJECT  : 
* FILENAME : pTemplatesManager.cpp
* DATE     : 2007/07/28
* TIME     : 15:17
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#include "pTemplatesManager.h"
#include "pSettings.h"
#include "pMonkeyStudio.h"
#include "pFileManager.h"
#include "ProjectItem.h"
#include "MonkeyCore.h"

using namespace pMonkeyStudio;

pTemplatesManager::pTemplatesManager( QObject* o )
	: QObject( o )
{}

void pTemplatesManager::setTemplatesPath( const QStringList& l )
{ MonkeyCore::settings()->setValue( "Templates/DefaultDirectories", l ); }

QStringList pTemplatesManager::templatesPath() const
{ return MonkeyCore::settings()->value( "Templates/DefaultDirectories" ).toStringList(); }

pTemplate pTemplatesManager::getTemplate( const QString& s )
{
	// open ini file
	QSettings set( s, QSettings::IniFormat );
	
	// fill template infos
	pTemplate t;
	t.Name = QFileInfo( s ).dir().dirName();
	t.Language = set.value( "Language", tr( "Other" ) ).toString();
	t.Type = set.value( "Type", tr( "Wrong template type" ) ).toString();
	t.Description = set.value( "Description", tr( "No desctiption" ) ).toString();
	t.Icon = set.value( "Icon" ).toString();
	t.Script = set.value( "Script" ).toString();
	t.DirPath = QFileInfo( s ).absolutePath().append( "/" );
	t.Files = set.value( "Files" ).toStringList();
	t.FilesToOpen = set.value( "FilesToOpen" ).toStringList();
	t.ProjectsToOpen = set.value( "ProjectsToOpen" ).toStringList();
	t.FilesToAdd = set.value( "FilesToAdd" ).toStringList();
	t.Variables = VarList();
	
	// set template vars
	foreach( QString v, set.value( "Variables" ).toStringList() )
		t.Variables[v] = set.value( v +"Values" ).toStringList();
	
	// return template
    return t;
}

TemplateList pTemplatesManager::getTemplates()
{
	TemplateList l;
	foreach( QString p, templatesPath() )
		foreach ( QFileInfo f, getFiles( QDir( unTokenizeHome( QDir::isRelativePath( p ) ? qApp->applicationDirPath().append( "/%1" ).arg( p ) : p ) ), QStringList( "template.ini" ), true ) )
			l << getTemplate( f.absoluteFilePath() );
	return l;
}

bool pTemplatesManager::realiseTemplate( ProjectItem* it, const QString& o, const pTemplate& t, const VariablesManager::Dictionary& d )
{
	// get destination
	QString dest = d["Destination"];
	
	// check destination
	if ( dest.isEmpty() )
	{
		warning( tr( "Error..." ), tr( "Destination not set." ) );
		return false;
	}
	
	// check destination exists
	if ( !QDir( dest ).exists() )
	{
		if ( !QDir().mkpath( dest ) )
		{
			warning( tr( "Error..." ), tr( "Can't create destination '%1'" ).arg( dest ) );
			return false;
		}
	}
	
	// append final slashe
	if ( !dest.endsWith( "/" ) )
		dest.append( '/' );
	
	// replace values in files/projects to open
	QStringList fo, po, fa;
	for ( int i = 0; i < t.FilesToOpen.count(); i++ )
		fo << VariablesManager::instance()->replaceAllVariables( t.FilesToOpen.at( i ), d );
	for ( int i = 0; i < t.ProjectsToOpen.count(); i++ )
		po << VariablesManager::instance()->replaceAllVariables( t.ProjectsToOpen.at( i ), d );
	for ( int i = 0; i < t.FilesToAdd.count(); i++ )
		fa << VariablesManager::instance()->replaceAllVariables( t.FilesToAdd.at( i ), d );
	
	// get files
	QHash<QString, QString> files;
	foreach( QString f, t.Files )
	{
		// check if sources and destination are differents
		QString sf, df;
		sf = f;
		df = f;
		if ( f.contains( '|' ) )
		{
			sf = f.section( '|', 0, 0 );
			df = f.section( '|', 1, 1 );
		}
		
		// process variables
		QString s = VariablesManager::instance()->replaceAllVariables( df, d );
		
		// check value validity
		if ( s.isEmpty() )
		{
			warning( tr( "Error..." ), tr( "Empty filename detected for file %1" ).arg( sf ) );
			return false;
		}
		
		// append file to list
		files[sf] = s;
	}
	
	// create files
	foreach( QString f, files.keys() )
	{
		// get source file
		QString k = QFile::exists( QString( "%1%2" ).arg( t.DirPath, f ) ) ? f : VariablesManager::instance()->replaceAllVariables( f, d );
		
		// get file name
		QString s = QString( "%1%2" ).arg( dest, files[f] );
		
		// check file destination exists
		QDir fd( QFileInfo( s ).path() );
		if ( !fd.exists() )
		{
			if ( !fd.mkpath( fd.path() ) )
			{
				warning( tr( "Error..." ), tr( "Can't create destination '%1'" ).arg( fd.path() ) );
				return false;
			}
		}
		
		// copy file
		if ( !QFile::copy( QString( "%1%2" ).arg( t.DirPath, k ), s ) )
		{
			warning( tr( "Error..." ), tr( "Can't copy '%1%2' to '%3'" ).arg( t.DirPath, k, s ) );
			return false;
		}
		
		// open file
		QFile file( s );
		if ( !file.open( QIODevice::ReadWrite | QIODevice::Text ) )
		{
			warning( tr( "Error..." ), tr ( "Can't edit file %1: %2" ).arg( s, file.errorString() ) );
			return false;
		}
		
		// get contents
		QString c = QString::fromLocal8Bit( file.readAll() );
		
		// reset file
		file.resize( 0 );
		
		// write process contents
		file.write( VariablesManager::instance()->replaceAllVariables( c, d ).toLocal8Bit() );
		
		// close file
		file.close();
		
		// open files if needed
        if ( fo.contains( files[f] ) )
            MonkeyCore::fileManager()->openFile( s );
        if ( po.contains( files[f] ) )
            MonkeyCore::fileManager()->openProject( s );
		
		// add files to project if needed
		if ( it )
			it->project()->addExistingFile( s, it, o );
	}
	
	// return process state
	return true;
}
