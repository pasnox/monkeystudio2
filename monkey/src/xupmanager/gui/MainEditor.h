#ifndef MAINEDITOR_H
#define MAINEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"

#include <QModelIndex>

class Ui_MainEditor;

class XUPProjectItem;
class XUPItem;
class XUPItemVariableEditorModel;

class Q_MONKEY_EXPORT MainEditor : public XUPPageEditor
{
	Q_OBJECT

public:
	MainEditor( QWidget* parent = 0 );
	virtual ~MainEditor();
	
	virtual void setup( XUPProjectItem* project );
	virtual void finalize();

protected:
	Ui_MainEditor* ui;
	XUPProjectItem* mProject;
	
	void updateMainFileComboBox( const QString& selectFile );

protected slots:
	void on_tbDynamicFolder_clicked();
};

#endif // MAINEDITOR_H
