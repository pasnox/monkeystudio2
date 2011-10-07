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
#include "SearchAndReplaceSettings.h"

SearchAndReplaceSettings::SearchAndReplaceSettings( SearchAndReplace* plugin, QWidget* parent )
    : QWidget( parent )
{
    Q_ASSERT( plugin );
    mPlugin = plugin;
    
    setupUi( this );
    
    loadSettings( mPlugin->settings() );
}

SearchAndReplaceSettings::~SearchAndReplaceSettings()
{
}

void SearchAndReplaceSettings::loadSettings( const SearchAndReplace::Settings& settings )
{
    cbReplaceSearchText->setChecked( settings.replaceSearchText );
    cbOnlyWhenNotVisible->setChecked( settings.onlyWhenNotVisible );
    cbOnlyWhenNotRegExp->setChecked( settings.onlyWhenNotRegExp );
    cbOnlyWhenNotEmpty->setChecked( settings.onlyWhenNotEmpty );
}

void SearchAndReplaceSettings::restoreDefault()
{
    loadSettings( SearchAndReplace::Settings() );
}

void SearchAndReplaceSettings::reject()
{
    close();
}

void SearchAndReplaceSettings::accept()
{
    SearchAndReplace::Settings settings;
    
    settings.replaceSearchText = cbReplaceSearchText->isChecked();
    settings.onlyWhenNotVisible = cbOnlyWhenNotVisible->isChecked();
    settings.onlyWhenNotRegExp = cbOnlyWhenNotRegExp->isChecked();
    settings.onlyWhenNotEmpty = cbOnlyWhenNotEmpty->isChecked();
    
    mPlugin->setSettings( settings );
    
    close();
}

void SearchAndReplaceSettings::on_dbbButtons_clicked( QAbstractButton* button )
{
    switch ( dbbButtons->standardButton( button ) )
    {
        case QDialogButtonBox::Help:
            //help();
            break;
        case QDialogButtonBox::RestoreDefaults:
            restoreDefault();
            break;
        case QDialogButtonBox::Ok:
            accept();
            break;
        case QDialogButtonBox::Cancel:
            reject();
            break;
        default:
            break;
    }
}
