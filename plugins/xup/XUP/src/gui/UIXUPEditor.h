#ifndef UISIMPLEQMAKEEDITOR_H
#define UISIMPLEQMAKEEDITOR_H

#include "ui_UIXUPEditor.h"

#include <QMap>

class XUPProjectItem;
class XUPItem;

class UIXUPEditor : public QDialog, public Ui::UIXUPEditor
{
	Q_OBJECT
	
public:
	UIXUPEditor( XUPProjectItem* project, QWidget* parent = 0 );
	virtual ~UIXUPEditor();

protected:
	XUPProjectItem* mProject;
	QStringList mFileVariables;
	QStringList mPathVariables;
	QMap<QString, QString> mValues;
	QMap<QString, QTreeWidgetItem*> mProjectFilesItems;
	QStringList mManagedVariables;
	
	QStringList mVariablesToRemove;
	
	QAction* aOthersValuesAddValue;
	QAction* aOthersValuesAddFile;
	QAction* aOthersValuesAddPath;
	QAction* aOthersValuesEditValue;
	QAction* aOthersValuesEditFile;
	QAction* aOthersValuesEditPath;
	
	void updateProjectFiles();
	void updateValuesEditorVariables();
	void updateValuesEditorValues( const QString& variable = QString::null );
	void init( XUPProjectItem* project );
	XUPItem* getUniqueVariableItem( const QString& variableName, bool create );

protected slots:
	// files
	void on_tbAddFile_clicked();
	void on_tbEditFile_clicked();
	void on_tbRemoveFile_clicked();
	
	// variables
	void on_lwOthersVariables_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
	void on_tbOthersVariablesAdd_clicked();
	void on_tbOthersVariablesEdit_clicked();
	void on_tbOthersVariablesRemove_clicked();
	
	// values
	void on_lwOthersValues_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
	void on_tbOthersValuesAdd_clicked();
	void on_tbOthersValuesAdd_triggered( QAction* action );
	void on_tbOthersValuesEdit_clicked();
	void on_tbOthersValuesEdit_triggered( QAction* action );
	void on_tbOthersValuesRemove_clicked();
	void on_tbOthersValuesClear_clicked();
	
	void accept();
};

#endif // UISIMPLEQMAKEEDITOR_H
