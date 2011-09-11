#ifndef QMAKECONFIGURATIONEDITOR_H
#define QMAKECONFIGURATIONEDITOR_H

#include <xupmanager/gui/XUPPageEditor.h>
#include "UIQMakeEditor.h"

class Ui_QMakeConfigurationEditor;
class pGenericTableModel;

class QModelIndex;

class QMakeConfigurationEditor : public XUPPageEditor
{
	Q_OBJECT

public:
	QMakeConfigurationEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent = 0 );
	virtual ~QMakeConfigurationEditor();

	virtual void setup( XUPProjectItem* project );
	virtual void finalize();

protected:
	UIQMakeEditor::ProjectValues& mPositiveValues;
	UIQMakeEditor::ProjectValues& mNegativeValues;
	Ui_QMakeConfigurationEditor* ui;
	XUPProjectItem* mProject;
	pGenericTableModel* mPositiveQtModulesModel;
	pGenericTableModel* mPositiveConfigurationModel;
	pGenericTableModel* mNegativeQtModulesModel;
	pGenericTableModel* mNegativeConfigurationModel;

protected slots:
	void showIndexHelp( const QModelIndex& index );
};

#endif // QMAKECONFIGURATIONEDITOR_H
