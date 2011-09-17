#include "CommandEditor.h"
#include "ui_CommandEditor.h"
#include "coremanager/MonkeyCore.h"
#include "consolemanager/pConsoleManager.h"

#include <QEvent>
#include <QFileDialog>
#include <QDebug>

CommandEditor::CommandEditor( QWidget* parent )
    : QFrame( parent ), ui( new Ui_CommandEditor )
{
    ui->setupUi( this );
    
    // populate parsers combobox
    ui->ccParsers->addItem( tr( "Try all" ) );
    ui->ccParsers->addSeparator();
    ui->ccParsers->addItems( MonkeyCore::consoleManager()->parsersName() );
}

CommandEditor::~CommandEditor()
{
    delete ui;
}

pCommand CommandEditor::command() const
{
    pCommand command;
    
    command.setText( ui->leText->text() );
    command.setCommand( ui->leCommand->text() );
    command.setWorkingDirectory( ui->leWorkDir->text() );
    command.setSkipOnError( ui->cbSkipOnError->isChecked() );
    //command.setParsers( ui->ccParsers->checkedList() );
    //command.setTryAllParsers( ui->cbTryAll->isChecked() );
    
    return command;
}

void CommandEditor::setCommand( const pCommand& command )
{
    ui->leText->setText( command.text() );
    ui->leCommand->setText( command.command() );
    ui->leWorkDir->setText( command.workingDirectory() );
    ui->cbSkipOnError->setChecked( command.skipOnError() );
    //ui->ccParsers->setCheckedList( command.parsers() );
    //ui->cbTryAll->setChecked( command.tryAllParsers() );
}

void CommandEditor::retranslateUi()
{
    ui->retranslateUi( this );
    // do your custom retranslate here
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
