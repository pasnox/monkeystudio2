#include "QMakeFilesEditor.h"

#include <xupmanager/core/XUPProjectItem.h>

QMakeFilesEditor::QMakeFilesEditor( QWidget* parent )
    : FilesEditor( parent )
{
    setQuoteSpacedValuesChecked( true );
    setQuoteSpacedValuesVisible( false );
    //setDeleteRemovedFilesChecked( true );
    //setDeleteRemovedFilesVisible( false );
}

QMakeFilesEditor::~QMakeFilesEditor()
{
}

QStringList QMakeFilesEditor::filteredFileVariables() const
{
    QStringList variables = FilesEditor::filteredFileVariables();
    variables.removeOne( "TRANSLATIONS" );
    return variables;
}
