/****************************************************************************
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
#include "pluginsmanager/ui/UICLIToolSettings.h"
#include "ui_UICLIToolSettings.h"
#include "pluginsmanager/CLIToolPlugin.h"

UICLIToolSettings::UICLIToolSettings( CLIToolPlugin* p, QWidget* w )
    : QWidget( w ), ui( new Ui_UICLIToolSettings ), mPlugin( p )
{
    Q_ASSERT( mPlugin );
    
    mDefault = mPlugin->defaultCommand();
    mReset = mPlugin->command();
    
    setAttribute( Qt::WA_DeleteOnClose );
    
    ui->setupUi( this );
    ui->ceCommand->setCommand( mReset );
}

UICLIToolSettings::~UICLIToolSettings()
{
    delete ui;
}

void UICLIToolSettings::restoreDefault()
{
    ui->ceCommand->setCommand( mDefault );
}

void UICLIToolSettings::reset()
{
    ui->ceCommand->setCommand( mReset );
}

void UICLIToolSettings::save()
{
    mPlugin->setCommand( ui->ceCommand->command() );
}

void UICLIToolSettings::on_dbbButtons_clicked( QAbstractButton* button )
{
    switch ( ui->dbbButtons->standardButton( button ) ) {
        case QDialogButtonBox::Reset:
            reset();
            break;
        case QDialogButtonBox::RestoreDefaults:
            restoreDefault();
            break;
        case QDialogButtonBox::Save:
            save();
            break;
        default:
            break;
    }
}
