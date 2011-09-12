#ifndef VARIABLESEDITOR_H
#define VARIABLESEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"
#include "xupmanager/core/XUPItemVariableEditorModel.h"

#include <QModelIndex>

class Ui_VariablesEditor;

class XUPProjectItem;
class XUPItem;
class XUPItemVariableEditorModel;

class Q_MONKEY_EXPORT VariablesEditor : public XUPPageEditor
{
    Q_OBJECT

public:
    VariablesEditor( QWidget* parent = 0 );
    virtual ~VariablesEditor();
    
    bool isQuoteSpacedValuesVisible() const;
    bool isQuoteSpacedValuesChecked() const;
    
    virtual XUPItemVariableEditorModel::FilterMode filterMode() const;
    virtual bool isFriendlyDisplayText() const;
    virtual QStringList filteredVariables() const;
    
    virtual void setup( XUPProjectItem* project );
    virtual void finalize();

public slots:
    void setQuoteSpacedValuesVisible( bool visible );
    void setQuoteSpacedValuesChecked( bool checked );

protected:
    Ui_VariablesEditor* ui;
    QAction* aValuesAddValue;
    QAction* aValuesAddFile;
    QAction* aValuesAddPath;
    QAction* aValuesEditValue;
    QAction* aValuesEditFile;
    QAction* aValuesEditPath;
    XUPProjectItem* mProject;
    XUPItemVariableEditorModel* mModel;
    
    QModelIndex currentVariable() const;
    QModelIndex currentValue() const;

protected slots:
    void lvVariables_selectionModel_selectionChanged();
    void on_tbVariablesAdd_clicked();
    void on_tbVariablesEdit_clicked();
    
    void lvValues_selectionModel_selectionChanged();
    void on_tbValuesAdd_clicked();
    void on_tbValuesAdd_triggered( QAction* action );
    void on_tbValuesEdit_clicked();
    void on_tbValuesEdit_triggered( QAction* action );
    void on_tbValuesClear_clicked();
};

#endif // VARIABLESEDITOR_H
