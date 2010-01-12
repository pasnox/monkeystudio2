#include "pEnvironmentVariablesManager.h"

#include <QProcess>

const QString pEnvironmentVariablesManager::mSettingsKey( "EnvironmentVariables" );

pEnvironmentVariablesManager::pEnvironmentVariablesManager( QObject* parent )
	: pSettings( parent, "EnvironmentVariables", "1.0.0" )
{
}

pEnvironmentVariablesModel::Variables pEnvironmentVariablesManager::variables() const
{
	pEnvironmentVariablesManager* _this = const_cast<pEnvironmentVariablesManager*>( this );
	pEnvironmentVariablesModel::Variables items;
	const int count = _this->beginReadArray( mSettingsKey );
	
	for ( int i = 0; i < count; i++ )
	{
		_this->setArrayIndex( i );
		
		items[ value( "Name" ).toString() ] = pEnvironmentVariablesModel::Variable( value( "Name" ).toString(),
																					value( "Value" ).toString(),
																					value( "Enabled" ).toBool() );
	}
	
	_this->endArray();
	
	if ( items.isEmpty() )
	{
		items = pEnvironmentVariablesModel::stringListToVariables( QProcess::systemEnvironment() );
	}
	else
	{
		mergeNewVariables( items );
	}
	
	return items;
}

void pEnvironmentVariablesManager::setVariables( const pEnvironmentVariablesModel::Variables& variables )
{
	beginWriteArray( mSettingsKey );
	
	for ( int i = 0; i < variables.count(); i++ )
	{
		setArrayIndex( i );
		const pEnvironmentVariablesModel::Variable& variable = (variables.begin() +i).value();
		
		setValue( "Name", variable.name );
		setValue( "Value", variable.value );
		setValue( "Enabled", variable.enabled );
	}
	
	endArray();
}

bool pEnvironmentVariablesManager::mergeNewVariables( pEnvironmentVariablesModel::Variables& variables ) const
{
	const pEnvironmentVariablesModel::Variables newVariables = pEnvironmentVariablesModel::stringListToVariables( QProcess::systemEnvironment() );
	bool hasNew = false;
	
	foreach ( const QString& name, newVariables.keys() )
	{
		if ( variables.contains( name ) )
		{
			continue;
		}
		
		hasNew = true;
		variables[ name ] = newVariables[ name ];
	}
	
	return hasNew;
}

QStringList pEnvironmentVariablesManager::variables( bool keepDisabled ) const
{
	return pEnvironmentVariablesModel::variablesToStringList( variables(), keepDisabled );
}
