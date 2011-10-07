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
#include "CommandEditor.h"
#include "ui_CommandEditor.h"
#include "coremanager/MonkeyCore.h"
#include "consolemanager/pConsoleManager.h"
#include "pConsoleManager.h"

#include <QEvent>
#include <QFileDialog>
#include <QWhatsThis>
#include <QDebug>

#define TryAll QT_TRANSLATE_NOOP( "CommandEditor", "Try All" )

CommandEditor::CommandEditor( QWidget* parent )
    : QFrame( parent ), ui( new Ui_CommandEditor )
{
    ui->setupUi( this );
    
    // populate parsers combobox
    ui->ccParsers->addItem( tr( TryAll ) );
    ui->ccParsers->addSeparator();
    ui->ccParsers->addItems( MonkeyCore::consoleManager()->parsersName() );
}

CommandEditor::~CommandEditor()
{
    delete ui;
}

pCommand CommandEditor::command() const
{
    const QStringList parsers = ui->ccParsers->checkedStringList();
    pCommand command;
    
    command.setName( ui->leName->text() );
    command.setText( ui->leText->text() );
    command.setCommand( ui->leCommand->text() );
    command.setWorkingDirectory( ui->leWorkDir->text() );
    command.setSkipOnError( ui->cbSkipOnError->isChecked() );
    command.setTryAllParsers( false );
    
    foreach ( const QString& parser, parsers ) {
        if ( parser == tr( TryAll ) ) {
            command.setTryAllParsers( true );
        }
        else {
            command.addParser( parser );
        }
    }
    
    return command;
}

void CommandEditor::setCommand( const pCommand& command )
{
    ui->leName->setText( command.name() );
    ui->leText->setText( command.text() );
    ui->leCommand->setText( command.command() );
    ui->leWorkDir->setText( command.workingDirectory() );
    ui->cbSkipOnError->setChecked( command.skipOnError() );
    ui->ccParsers->clearCheckStates();
    
    foreach ( const QString& parser, command.parsers() ) {
        const int row = ui->ccParsers->findText( parser );
        ui->ccParsers->setItemData( row, Qt::Checked, Qt::CheckStateRole );
    }
    
    const int row = ui->ccParsers->findText( tr( TryAll ) );
    ui->ccParsers->setItemData( row, command.tryAllParsers() ? Qt::Checked : QVariant(), Qt::CheckStateRole );
}

void CommandEditor::retranslateUi()
{
    ui->retranslateUi( this );
}

void CommandEditor::changeEvent( QEvent* event )
{
    QWidget::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void CommandEditor::on_tbCommand_clicked()
{
    const QString filePath = QFileDialog::getOpenFileName( window(), tr( "Select an executable file" ), ui->leCommand->text() );
    
    if ( !filePath.isNull() ) {
        ui->leCommand->setText( filePath );
    }
}

void CommandEditor::on_tbWorkDir_clicked()
{
    const QString path = QFileDialog::getExistingDirectory( window(), tr( "Select a folder" ), ui->leWorkDir->text() );
    
    if ( !path.isEmpty() ) {
        ui->leWorkDir->setText( path );
    }
}

void CommandEditor::on_tbHelp_clicked()
{
    QWhatsThis::showText( mapToGlobal( QPoint( rect().center().x(), 0 ) ), pConsoleManager::variablesHelp() );
}
