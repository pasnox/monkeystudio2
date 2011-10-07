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
#ifndef PENVIRONMENTVARIABLESMANAGER_H
#define PENVIRONMENTVARIABLESMANAGER_H

#include "pEnvironmentVariablesModel.h"

class pEnvironmentVariablesManager
{   
public:
    virtual ~pEnvironmentVariablesManager();
    
    bool load();
    bool save();
    
    pEnvironmentVariablesModel::Variables variables() const;
    void setVariables( const pEnvironmentVariablesModel::Variables& variables );
    
    bool mergeNewVariables( pEnvironmentVariablesModel::Variables& variables ) const;
    pEnvironmentVariablesModel::Variables mergeNewVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
    bool removeUnmodifiedVariables( pEnvironmentVariablesModel::Variables& variables ) const;
    pEnvironmentVariablesModel::Variables removeUnmodifiedVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
    QStringList variables( bool keepDisabled ) const;

protected:
    static const QString mSettingsKey;
    bool mInitialized;
    mutable pEnvironmentVariablesModel::Variables mVariables;
    
    virtual bool writeVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
    virtual bool readVariables( pEnvironmentVariablesModel::Variables& variables ) const;
};

#endif // PENVIRONMENTVARIABLESMANAGER_H
