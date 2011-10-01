#include "VariablesEditor.h"
#include "ui_VariablesEditor.h"
#include "xupmanager/core/XUPProjectItem.h"

#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

VariablesEditor::VariablesEditor( QWidget* parent )
    : XUPPageEditor( parent )
{
    ui = new Ui_VariablesEditor;
    mProject = 0;
    mModel = new XUPItemVariableEditorModel( this );
    
    ui->setupUi( this );
    ui->lvVariables->setModel( mModel );
    ui->lvValues->setModel( mModel );
    
    // tbValuesAdd actions
    QMenu* addMenu = new QMenu( ui->tbValuesAdd );
    aValuesAddValue = addMenu->addAction( tr( "As Value..." ) );
    aValuesAddFile = addMenu->addAction( tr( "As File..." ) );
    aValuesAddPath = addMenu->addAction( tr( "As Path..." ) );
    ui->tbValuesAdd->setMenu( addMenu );
    
    // tbValuesEdit actions
    QMenu* editMenu = new QMenu( ui->tbValuesEdit );
    aValuesEditValue = editMenu->addAction( tr( "As Value..." ) );
    aValuesEditFile = editMenu->addAction( tr( "As File..." ) );
    aValuesEditPath = editMenu->addAction( tr( "As Path..." ) );
    ui->tbValuesEdit->setMenu( editMenu );
    
    connect( ui->lvVariables->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvVariables_selectionModel_selectionChanged() ) );
    connect( ui->lvValues->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvValues_selectionModel_selectionChanged() ) );
    connect( ui->cbQuoteEnabled, SIGNAL( toggled( bool ) ), mModel, SLOT( setQuoteValues( bool ) ) );
    
    mModel->setQuoteValues( ui->cbQuoteEnabled->isChecked() );
    
    lvVariables_selectionModel_selectionChanged();
}

VariablesEditor::~VariablesEditor()
{
    delete ui;
}

void VariablesEditor::setQuoteSpacedValuesVisible( bool visible )
{
    ui->cbQuoteEnabled->setVisible( visible );
}

bool VariablesEditor::isQuoteSpacedValuesVisible() const
{
    return ui->cbQuoteEnabled->isVisible();
}

void VariablesEditor::setQuoteSpacedValuesChecked( bool checked )
{
    ui->cbQuoteEnabled->setChecked( checked );
}

bool VariablesEditor::isQuoteSpacedValuesChecked() const
{
    return ui->cbQuoteEnabled->isChecked();
}

XUPItemVariableEditorModel::FilterMode VariablesEditor::filterMode() const
{
    return XUPItemVariableEditorModel::Out;
}

bool VariablesEditor::isFriendlyDisplayText() const
{
    return false;
}

QStringList VariablesEditor::filteredVariables() const
{
    return mProject ? mProject->documentFilters().fileVariables() : QStringList();
}

void VariablesEditor::setup( XUPProjectItem* project )
{
    mProject = project;
    mModel->setFilterMode( filterMode() );
    mModel->setFriendlyDisplayText( isFriendlyDisplayText() );
    mModel->setFilteredVariables( filteredVariables() );
    mModel->setRootItem( project );
    const QModelIndex index = mModel->index( 0, 0 );
    ui->lvVariables->setCurrentIndex( index );
    ui->lvVariables->scrollTo( index );
}

void VariablesEditor::finalize()
{
    mModel->submit();
}

QModelIndex VariablesEditor::currentVariable() const
{
    const QModelIndex index = ui->lvVariables->selectionModel()->selectedIndexes().value( 0 );
    return ui->lvVariables->rootIndex() == index.parent() ? index : QModelIndex();
}

QModelIndex VariablesEditor::currentValue() const
{
    const QModelIndex index = ui->lvValues->selectionModel()->selectedIndexes().value( 0 );
    return ui->lvValues->rootIndex() == index.parent() ? index : QModelIndex();
}

void VariablesEditor::lvVariables_selectionModel_selectionChanged()
{
    const QModelIndex index = currentVariable();
    ui->tbVariablesEdit->setEnabled( index.isValid() );
    ui->gbValues->setEnabled( index.isValid() );
    ui->lvValues->setRootIndex( index );
    lvValues_selectionModel_selectionChanged();
}

void VariablesEditor::on_tbVariablesAdd_clicked()
{
    const QStringList variables = mModel->knownVariables();
    bool ok;
    
    const QString variable = QInputDialog::getItem( QApplication::activeWindow(), tr( "Add variable..." ), tr( "Select a variable name or enter a new one" ), variables, 0, true, &ok );
    
    if ( !variable.isEmpty() && ok ) {
        const QModelIndex index = mModel->addVariable( variable );
        
        if ( index.isValid() ) {
            ui->lvVariables->setCurrentIndex( index );
            ui->lvVariables->scrollTo( index );
        }
        else {
            QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This variable is filtered." ) );
        }
    }
}

void VariablesEditor::on_tbVariablesEdit_clicked()
{
    const QModelIndex index = currentVariable();
    
    if ( !index.isValid() ) {
        return;
    }
    
    bool ok;
    QString variable = index.data( Qt::DisplayRole ).toString();
    variable = QInputDialog::getText( QApplication::activeWindow(), tr( "Edit variable..." ), tr( "Enter a new name for this variable" ), QLineEdit::Normal, variable, &ok );
    
    if ( !variable.isEmpty() && ok ) {
        if ( !mModel->setData( index, variable, Qt::DisplayRole ) ) {
            QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This variable exists or is filtered." ) );
        }
    }
}

void VariablesEditor::lvValues_selectionModel_selectionChanged()
{
    const QModelIndex variableIndex = currentVariable();
    const QModelIndex valueIndex = currentValue();
    const int count = mModel->rowCount( variableIndex );
    ui->tbValuesEdit->setEnabled( valueIndex.isValid() );
    ui->tbValuesClear->setEnabled( variableIndex.isValid() && count > 0 );
}

void VariablesEditor::on_tbValuesAdd_clicked()
{
    on_tbValuesAdd_triggered( aValuesAddValue );
}

void VariablesEditor::on_tbValuesAdd_triggered( QAction* action )
{
    const QModelIndex variableIndex = currentVariable();
    
    if ( !variableIndex.isValid() ) {
        return;
    }
    
    const QString title = tr( "Add a value..." );
    bool ok = true;
    QString value;
    
    if ( action == aValuesAddValue ) {
        value = QInputDialog::getText( QApplication::activeWindow(), title, tr( "Enter the value :" ), QLineEdit::Normal, QString(), &ok );
        
        if ( !ok ) {
            value.clear();
        }
    }
    else if ( action == aValuesAddFile ) {
        value = QFileDialog::getOpenFileName( QApplication::activeWindow(), tr( "Choose a file" ), mProject->path() );
        
        if ( !value.isEmpty() ) {
            value = mProject->relativeFilePath( value );
        }
    }
    else if ( action == aValuesAddPath ) {
        value = QFileDialog::getExistingDirectory( QApplication::activeWindow(), tr( "Choose a path" ), mProject->path() );
        
        if ( !value.isEmpty() ) {
            value = mProject->relativeFilePath( value );
        }
    }
    
    if ( value.isEmpty() || !ok ) {
        return;
    }
    
    const QModelIndex index = mModel->addValue( variableIndex, value );
    ui->lvValues->setCurrentIndex( index );
    ui->lvValues->scrollTo( index );
}

void VariablesEditor::on_tbValuesEdit_clicked()
{
    on_tbValuesEdit_triggered( aValuesEditValue );
}

void VariablesEditor::on_tbValuesEdit_triggered( QAction* action )
{
    const QModelIndex variableIndex = currentVariable();
    const QModelIndex valueIndex = currentValue();
    
    if ( !variableIndex.isValid() || !valueIndex.isValid() ) {
        return;
    }
    
    const QString title = tr( "Edit a value..." );
    bool ok = true;
    QString value = valueIndex.data( Qt::DisplayRole ).toString();
    
    if ( action == aValuesEditValue ) {
        value = QInputDialog::getText( QApplication::activeWindow(), title, tr( "Edit the value :" ), QLineEdit::Normal, value, &ok );
        
        if ( !ok ) {
            value.clear();
        }
    }
    else {
        value = mProject->filePath( value );
        
        if ( action == aValuesEditFile ) {
            value = QFileDialog::getOpenFileName( QApplication::activeWindow(), tr( "Choose a file" ), value );
        }
        else if ( action == aValuesEditPath ) {
            value = QFileDialog::getExistingDirectory( QApplication::activeWindow(), tr( "Choose a path" ), value );
        }
        
        if ( !value.isEmpty() ) {
            value = mProject->relativeFilePath( value );
        }
    }
    
    if ( value.isEmpty() || !ok ) {
        return;
    }
    
    if ( !mModel->setData( valueIndex, value, Qt::DisplayRole ) ) {
        QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This value already exists." ) );
    }
}

void VariablesEditor::on_tbValuesClear_clicked()
{
    const QModelIndex variableIndex = currentVariable();
    
    if ( !variableIndex.isValid() ) {
        return;
    }
    
    if ( QMessageBox::question( QApplication::activeWindow(), tr( "Clear values..." ), tr( "Are you sure you want to clear these values ?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No ) {
        return;
    }
    
    for ( int i = 0; i < mModel->rowCount( variableIndex ); i++ ) {
        const QModelIndex index = variableIndex.child( i, 0 );
        mModel->setData( index, Qt::Checked, Qt::CheckStateRole );
    }
}
