#include "CommandsEditor.h"
#include "ui_CommandsEditor.h"
#include "CommandsEditorModel.h"
#include "coremanager/MonkeyCore.h"

#include <pMenuBar.h>

#include <QDebug>

CommandsEditor::CommandsEditor( QWidget* parent )
    : XUPPageEditor( parent )
{
    ui = new Ui_CommandsEditor;
    mModel = new CommandsEditorModel( this );
    mProject = 0;
    
    ui->setupUi( this );
    ui->tvCommands->setModel( mModel );
    
    connect( ui->tvCommands->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( tvCommands_selectionModel_selectionChanged( const QItemSelection&, const QItemSelection& ) ) );
}

CommandsEditor::~CommandsEditor()
{
    delete ui;
}

void CommandsEditor::setup( XUPProjectItem* project )
{
    mProject = project;
    mModel->setCommands( XUPProjectItemHelper::projectCommands( mProject ), MonkeyCore::menuBar() );
    tvCommands_selectionModel_selectionChanged( QItemSelection(), QItemSelection() );
}

void CommandsEditor::finalize()
{
    setCommand( ui->tvCommands->selectionModel()->selectedIndexes().value( 0 ) );
    mModel->submit();
    XUPProjectItemHelper::setProjectCommands( mProject, mModel->commands() );
}

void CommandsEditor::setCommand( const QModelIndex& commandIndex )
{
    const bool isAction = commandIndex.isValid() && commandIndex.parent() != QModelIndex();
    
    if ( !isAction ) {
        return;
    }
    
    pCommand command = ui->ceCommand->command();
    command.setUserData( commandIndex.data( Qt::CheckStateRole ) );
    mModel->setData( commandIndex, QVariant::fromValue( command ), Qt::EditRole );
}

void CommandsEditor::getCommand( const QModelIndex& commandIndex )
{
    const pCommand command = mModel->command( commandIndex );
    ui->ceCommand->setCommand( command );
}

void CommandsEditor::updateState()
{
    const QModelIndex index = ui->tvCommands->selectionModel()->selectedIndexes().value( 0 );
    const bool isAction = index.isValid() && index.parent() != QModelIndex();
    const int count = mModel->rowCount( index.parent() );
    
    ui->tbCommandAdd->setEnabled( index.isValid() );
    ui->tbCommandUp->setEnabled( isAction && index.row() > 0 && count > 1 );
    ui->tbCommandDown->setEnabled( isAction && index.row() < count -1 && count > 1 );
    ui->ceCommand->setEnabled( isAction );
}

void CommandsEditor::tvCommands_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
    const QModelIndex oldIndex = deselected.indexes().value( 0 );
    const QModelIndex newIndex = selected.indexes().value( 0 );
    
    setCommand( oldIndex );
    getCommand( newIndex );
    updateState();
}

void CommandsEditor::on_tbCommandAdd_clicked()
{
    const QModelIndex index = ui->tvCommands->selectionModel()->selectedIndexes().value( 0 );
    const bool isAction = index.isValid() && index.parent() != QModelIndex();
    const QModelIndex menuIndex = isAction ? index.parent() : index;
    const QModelIndex commandIndex = mModel->addCommand( menuIndex, pCommand( tr( "New command" ), "$cpp$" ) );
    
    if ( commandIndex.isValid() ) {
        ui->tvCommands->setCurrentIndex( commandIndex );
        ui->tvCommands->scrollTo( commandIndex, QAbstractItemView::EnsureVisible );
    }
}

void CommandsEditor::on_tbCommandUp_clicked()
{
    const QModelIndex index = ui->tvCommands->selectionModel()->selectedIndexes().value( 0 );
    mModel->swapCommand( index.parent(), index.row(), index.row() -1 );
    updateState();
}

void CommandsEditor::on_tbCommandDown_clicked()
{
    const QModelIndex index = ui->tvCommands->selectionModel()->selectedIndexes().value( 0 );
    mModel->swapCommand( index.parent(), index.row(), index.row() +1 );
    updateState();
}
