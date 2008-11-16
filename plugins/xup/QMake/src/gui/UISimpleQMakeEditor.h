#ifndef UISIMPLEQMAKEEDITOR_H
#define UISIMPLEQMAKEEDITOR_H

#include "ui_UISimpleQMakeEditor.h"
#include "../QtVersionManager.h"

#include <QMap>

class XUPProjectItem;
class XUPItem;

class UISimpleQMakeEditor : public QDialog, public Ui::UISimpleQMakeEditor
{
	Q_OBJECT
	
public:
	UISimpleQMakeEditor( XUPProjectItem* project, QWidget* parent = 0 );
	virtual ~UISimpleQMakeEditor();

protected:
	XUPProjectItem* mProject;
	QStringList mConfigGui;
	QStringList mFileVariables;
	QStringList mPathVariables;
	QtVersion mQtVersion;
	QMap<QString, QString> mValues;
	QMap<QString, QTreeWidgetItem*> mProjectFilesItems;
	
	void updateProjectFiles();
	void init( XUPProjectItem* project );
	XUPItem* getUniqueVariableItem( const QString& variableName, bool create );

protected slots:
	void projectTypeChanged();
	void on_tbProjectTarget_clicked();
	void modules_itemSelectionChanged();
	void on_tbAddFile_clicked();
	void on_tbEditFile_clicked();
	void on_tbRemoveFile_clicked();
	void accept();
};

#endif // UISIMPLEQMAKEEDITOR_H
