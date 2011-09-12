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
#include "pluginsmanager/CLIToolPlugin.h"
#include "consolemanager/pConsoleManager.h"
#include "pMonkeyStudio.h"
#include "coremanager/MonkeyCore.h"
#include "maininterface/UIMain.h"

#include "ui_UICLIToolSettings.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QPushButton>

using namespace pMonkeyStudio;

UICLIToolSettings::UICLIToolSettings( CLIToolPlugin* p, QWidget* w )
    : QWidget( w ), ui( new Ui_UICLIToolSettings ), mPlugin( p )
{
    Q_ASSERT( mPlugin );
    ui->setupUi( this );
    
    // delete widget when close
    setAttribute( Qt::WA_DeleteOnClose );
    
    // memorize defaults and user commands
    mDefault = mPlugin->defaultCommand();
    mCommand = mPlugin->command();
    mReset = mCommand;
    
    // add parsers
    ui->lwBuildCommandParsers->addItems( MonkeyCore::consoleManager()->parsersName() );
    
    // set uncheck state for parser items
    for ( int i = 0; i < ui->lwBuildCommandParsers->count(); i++ ) {
        ui->lwBuildCommandParsers->item( i )->setCheckState( Qt::Unchecked );
    }
    
    // load commands
    updateCommand();
}

UICLIToolSettings::~UICLIToolSettings()
{
    delete ui;
}

void UICLIToolSettings::updateCommand()
{
    ui->leBuildCommandText->setText( mCommand.text() );
    ui->leBuildCommandCommand->setText( mCommand.command() );
    ui->leBuildCommandArguments->setText( mCommand.arguments() );
    ui->leBuildCommandWorkingDirectory->setText( mCommand.workingDirectory() );
    ui->cbBuildCommandSkipOnError->setChecked( mCommand.skipOnError() );
    
    for ( int i = 0; i < ui->lwBuildCommandParsers->count(); i++ ) {
        QListWidgetItem* it = ui->lwBuildCommandParsers->item( i );
        it->setCheckState( mCommand.parsers().contains( it->text() ) ? Qt::Checked : Qt::Unchecked );
    }
    
    ui->cbBuildCommandTryAll->setChecked( mCommand.tryAllParsers() );
}

void UICLIToolSettings::restoreDefault()
{
    mCommand = mDefault;
    updateCommand();
}

void UICLIToolSettings::reset()
{
    mCommand = mReset;
    updateCommand();
}

void UICLIToolSettings::save()
{
    mCommand.setText( ui->leBuildCommandText->text() );
    mCommand.setCommand( ui->leBuildCommandCommand->text() );
    mCommand.setArguments( ui->leBuildCommandArguments->text() );
    mCommand.setWorkingDirectory( ui->leBuildCommandWorkingDirectory->text() );
    mCommand.setSkipOnError( ui->cbBuildCommandSkipOnError->isChecked() );
    QStringList parsers;
    
    for ( int i = 0; i < ui->lwBuildCommandParsers->count(); i++ ) {
        QListWidgetItem* it = ui->lwBuildCommandParsers->item( i );
        
        if ( it->checkState() == Qt::Checked ) {
            parsers << it->text();
        }
    }
    
    mCommand.setParsers( parsers );
    mCommand.setTryAllParsers( ui->cbBuildCommandTryAll->isChecked() );
    mPlugin->setCommand( mCommand );
}

void UICLIToolSettings::on_tbBuildCommandCommand_clicked()
{
    const QString filePath = QFileDialog::getOpenFileName( MonkeyCore::mainWindow(), tr( "Select an executable" ), ui->leBuildCommandCommand->text() );
    
    if ( !filePath.isNull() ) {
        ui->leBuildCommandCommand->setText( filePath );
    }
}

void UICLIToolSettings::on_tbBuildCommandWorkingDirectory_clicked()
{
    const QString path = QFileDialog::getExistingDirectory( this, tr( "Select a folder" ), ui->leBuildCommandWorkingDirectory->text() );
    
    if ( !path.isEmpty() ) {
        ui->leBuildCommandWorkingDirectory->setText( path );
    }
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
