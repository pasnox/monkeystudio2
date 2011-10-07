/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#include "pEnvironmentVariablesManager.h"

#include "pSettings.h"

#include <QProcess>
#include <QCoreApplication>

const QString pEnvironmentVariablesManager::mSettingsKey( "EnvironmentVariables" );

pEnvironmentVariablesManager::~pEnvironmentVariablesManager()
{
}

bool pEnvironmentVariablesManager::load()
{
    return readVariables( mVariables );
}

bool pEnvironmentVariablesManager::save()
{
    return writeVariables( mVariables );
}

pEnvironmentVariablesModel::Variables pEnvironmentVariablesManager::variables() const
{
    pEnvironmentVariablesModel::Variables variables = mVariables;
    
    if ( variables.isEmpty() )
    {
        variables = pEnvironmentVariablesModel::stringListToVariables( QProcess::systemEnvironment() );
    }
    else
    {
        mergeNewVariables( variables );
    }
    
    return variables;
}

void pEnvironmentVariablesManager::setVariables( const pEnvironmentVariablesModel::Variables& variables )
{
    mVariables = removeUnmodifiedVariables( variables );
}

bool pEnvironmentVariablesManager::mergeNewVariables( pEnvironmentVariablesModel::Variables& variables ) const
{
    const pEnvironmentVariablesModel::Variables newVariables = pEnvironmentVariablesModel::stringListToVariables( QProcess::systemEnvironment() );
    bool modified = false;
    
    foreach ( const QString& name, newVariables.keys() )
    {
        if ( variables.contains( name ) )
        {
            continue;
        }
        
        modified = true;
        variables[ name ] = newVariables[ name ];
    }
    
    return modified;
}

pEnvironmentVariablesModel::Variables pEnvironmentVariablesManager::mergeNewVariables( const pEnvironmentVariablesModel::Variables& variables ) const
{
    pEnvironmentVariablesModel::Variables items = variables;
    mergeNewVariables( items );
    return items;
}

bool pEnvironmentVariablesManager::removeUnmodifiedVariables( pEnvironmentVariablesModel::Variables& variables ) const
{
    const pEnvironmentVariablesModel::Variables sysVariables = pEnvironmentVariablesModel::stringListToVariables( QProcess::systemEnvironment() );
    bool modified = false;
    
    foreach ( const pEnvironmentVariablesModel::Variable& variable, variables )
    {
        if ( !variable.enabled )
        {
            continue;
        }
        
        if ( sysVariables.contains( variable.name ) && variable.value == sysVariables[ variable.name ].value )
        {
            variables.remove( variable.name );
            modified = true;
        }
    }
    
    return modified;
}

pEnvironmentVariablesModel::Variables pEnvironmentVariablesManager::removeUnmodifiedVariables( const pEnvironmentVariablesModel::Variables& variables ) const
{
    pEnvironmentVariablesModel::Variables items = variables;
    removeUnmodifiedVariables( items );
    return items;
}

QStringList pEnvironmentVariablesManager::variables( bool keepDisabled ) const
{
    return pEnvironmentVariablesModel::variablesToStringList( variables(), keepDisabled );
}

bool pEnvironmentVariablesManager::writeVariables( const pEnvironmentVariablesModel::Variables& variables ) const
{
    pSettings settings( qApp, "EnvironmentVariables", "1.0.0" );
    
    settings.remove( mSettingsKey );
    
    settings.beginWriteArray( mSettingsKey );
    
    for ( int i = 0; i < variables.count(); i++ )
    {
        settings.setArrayIndex( i );
        const pEnvironmentVariablesModel::Variable& variable = (variables.begin() +i).value();
        
        settings.setValue( "Name", variable.name );
        settings.setValue( "Value", variable.value );
        settings.setValue( "Enabled", variable.enabled );
    }
    
    settings.endArray();
    
    return true;
}

bool pEnvironmentVariablesManager::readVariables( pEnvironmentVariablesModel::Variables& variables ) const
{
    pSettings settings( qApp, "EnvironmentVariables", "1.0.0" );
    
    const int count = settings.beginReadArray( mSettingsKey );
    
    for ( int i = 0; i < count; i++ )
    {
        settings.setArrayIndex( i );
        
        variables[ settings.value( "Name" ).toString() ] =
            pEnvironmentVariablesModel::Variable( settings.value( "Name" ).toString(),
                settings.value( "Value" ).toString(), settings.value( "Enabled" ).toBool() );
    }
    
    settings.endArray();
    
    return true;
}
