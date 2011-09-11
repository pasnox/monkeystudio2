#include "QMakeVariablesEditor.h"
#include "UIQMakeEditor.h"

#include <xupmanager/core/XUPProjectItem.h>

QMakeVariablesEditor::QMakeVariablesEditor( QWidget* parent )
	: VariablesEditor( parent )
{
	/*setQuoteSpacedValuesChecked( false );
	setQuoteSpacedValuesVisible( false );*/
}

QMakeVariablesEditor::~QMakeVariablesEditor()
{
}


XUPItemVariableEditorModel::FilterMode QMakeVariablesEditor::filterMode() const
{
	return XUPItemVariableEditorModel::Out;
}

bool QMakeVariablesEditor::isFriendlyDisplayText() const
{
	return false;
}

QStringList QMakeVariablesEditor::filteredVariables() const
{
	QStringList variables = UIQMakeEditor::handledVariables();
	
	if ( mProject ) {
		variables << mProject->documentFilters().fileVariables();
	}
	
	return variables;
}
