#ifndef QMAKEVARIABLESEDITOR_H
#define QMAKEVARIABLESEDITOR_H

#include <xupmanager/gui/VariablesEditor.h>

class QMakeVariablesEditor : public VariablesEditor
{
	Q_OBJECT
	
public:
	QMakeVariablesEditor( QWidget* parent = 0 );
	virtual ~QMakeVariablesEditor();
	
	virtual XUPItemVariableEditorModel::FilterMode filterMode() const;
	virtual bool isFriendlyDisplayText() const;
	virtual QStringList filteredVariables() const;
};

#endif // QMAKEVARIABLESEDITOR_H
