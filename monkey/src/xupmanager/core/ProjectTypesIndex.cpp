#include "xupmanager/core/ProjectTypesIndex.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "pIconManager.h"
#include "pMonkeyStudio.h"

#include <QDir>
#include <QDebug>

DocumentFilterMap::DocumentFilterMap( const QString& iconsPath )
{
	mIconsPath = iconsPath;
}

DocumentFilterMap::~DocumentFilterMap()
{
}

QString DocumentFilterMap::variableDisplayText( const QString& variableName ) const
{
	const QString text = value( variableName ).label;
	return text.isEmpty() ? variableName : text;
}

QString DocumentFilterMap::variableDisplayIcon( const QString& variableName ) const
{
	QString icon = value( variableName ).icon;
	QString filePath;
	
	if ( icon.isEmpty() ) {
		icon = QString( "%1.png" ).arg( variableName.toLower() );
	}
	
	filePath = QString( "%1/%2" ).arg( iconsPath() ).arg( icon );
	
	if ( !QFile::exists( filePath ) ) {
		filePath = QString( "%1/%2" ).arg( defaultIconsPath() ).arg( icon );
	}
	
	return QDir::cleanPath( filePath );
}

QStringList DocumentFilterMap::splitValue( const QString& value ) const
{
	//return value.split( " " );
	QStringList tmpValues = value.split( " ", QString::SkipEmptyParts );
	bool inStr = false;
	QStringList multivalues;
	QString ajout;

	for ( int ku = 0; ku < tmpValues.size(); ku++ ) {
		if ( tmpValues.value( ku ).startsWith( '"' ) ) {
			inStr = true;
		}
		
		if ( inStr ) {
			if( ajout != "" ) {
				ajout += " ";
			}
			
			ajout += tmpValues.value( ku );
			
			if ( tmpValues.value( ku ).endsWith('"') ) {
					multivalues += ajout;
					ajout = "";
					inStr = false;
			}
		}
		else {
			multivalues += tmpValues.value( ku );
		}
	}

	return multivalues;
}

QString DocumentFilterMap::fileNameVariable( const QString& fileName ) const
{
	foreach ( const QString& name, keys() ) {
		const DocumentFilter& filter = (*this)[ name ];
		
		if ( filter.type != DocumentFilter::File ) {
			continue;
		}
		
		if ( QDir::match( filter.filters, fileName ) ) {
			return name;
		}
	}
	
	return QString::null;
}

QStringList DocumentFilterMap::knownVariables() const
{
	QStringList variables = keys().toSet().toList();
	
	for ( int i = 0; i < variables.count(); i++ ) {
		const QString& variable = variables[ i ];
		
		if ( variable.contains( "project", Qt::CaseInsensitive ) ) {
			variables.removeAt( i );
		}
	}
	
	return variables;
}

QString DocumentFilterMap::defaultIconsPath() const
{
	return ":/items";
}

QString DocumentFilterMap::iconsPath() const
{
	return mIconsPath;
}

QString DocumentFilterMap::sourceFileNameFilter() const
{
	return pMonkeyStudio::buildFileDialogFilter( *this, true, true );
}

QStringList DocumentFilterMap::filteredVariables() const
{
	QMap<int, QString> variables;
	
	foreach ( const QString& name, keys() ) {
		const DocumentFilter& filter = (*this)[ name ];
		
		if ( filter.filtered ) {
			// use insert multi in case on many variable with same weight, ie: all are -1 weight.
			variables.insertMulti( filter.weight, name );
		}
	}
	
	return variables.values();
}

QStringList DocumentFilterMap::fileVariables() const
{
	QMap<int, QString> variables;
	
	foreach ( const QString& name, keys() ) {
		const DocumentFilter& filter = (*this)[ name ];
		
		if ( filter.type == DocumentFilter::File ) {
			// use insert multi in case on many variable with same weight, ie: all are -1 weight.
			variables.insertMulti( filter.weight, name );
		}
	}
	
	return variables.values();
}

QStringList DocumentFilterMap::pathVariables() const
{
	QMap<int, QString> variables;
	
	foreach ( const QString& name, keys() ) {
		const DocumentFilter& filter = (*this)[ name ];
		
		if ( filter.type == DocumentFilter::Path ) {
			// use insert multi in case on many variable with same weight, ie: all are -1 weight.
			variables.insertMulti( filter.weight, name );
		}
	}
	
	return variables.values();
}

ProjectTypesIndex::ProjectTypesIndex( QObject* parent )
	: QObject( parent )
{
}

void ProjectTypesIndex::registerType( const QString& projectType, const QMetaObject* projectMetaObject, const DocumentFilterMap& filters )
{
	mRegisteredProjectItems[ projectType ] = projectMetaObject;
	mFilters[ projectType ] = &filters;
}

void ProjectTypesIndex::unRegisterType( const QString& projectType )
{
	mRegisteredProjectItems.remove( projectType );
	mFilters.remove( projectType );
}

const DocumentFilterMap& ProjectTypesIndex::documentFilters( const QString& projectType ) const
{
	return *( mFilters[ projectType ] );
}

bool ProjectTypesIndex::fileIsAProject( const QString& fileName ) const
{
	foreach ( const QString& projectType, mFilters.keys() ) {
		foreach ( const DocumentFilter& filter, *( mFilters[ projectType ] ) ) {
			if ( filter.type != DocumentFilter::Project ) {
				continue;
			}
			
			if ( QDir::match( filter.filters, fileName ) ) {
				return true;
			}
		}
	}
	
	return false;
}

XUPProjectItem* ProjectTypesIndex::newProjectItem( const QString& fileName ) const
{
	foreach ( const QString& projectType, mFilters.keys() ) {
		foreach ( const DocumentFilter& filter, *( mFilters[ projectType ] ) ) {
			if ( QDir::match( filter.filters, fileName ) ) {
				return qobject_cast<XUPProjectItem*>( mRegisteredProjectItems[ projectType ]->newInstance() );
			}
		}
	}
	
	return 0;
}

XUPProjectItem* ProjectTypesIndex::newProjectItemByType( const QString& type ) const
{
	const QMetaObject* mo = mRegisteredProjectItems.value( type );
	return mo ? qobject_cast<XUPProjectItem*>( mo->newInstance() ) : 0;
}

QMap<QString, QStringList> ProjectTypesIndex::suffixes() const
{
	QMap<QString, QStringList> suffixes;
	
	foreach ( const QString& projectType, mFilters.keys() ) {
		foreach ( const DocumentFilter& filter, *( mFilters[ projectType ] ) ) {
			suffixes[ filter.label ] = filter.filters;
		}
	}
	
	return suffixes;
}
