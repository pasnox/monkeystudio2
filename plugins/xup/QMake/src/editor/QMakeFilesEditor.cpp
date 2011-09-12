#include "QMakeFilesEditor.h"

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
