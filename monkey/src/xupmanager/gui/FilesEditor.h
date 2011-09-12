#ifndef FILESEDITOR_H
#define FILESEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"

#include <QModelIndex>

class Ui_FilesEditor;

class XUPProjectItem;
class XUPItem;
class XUPItemVariableEditorModel;

class Q_MONKEY_EXPORT FilesEditor : public XUPPageEditor
{
    Q_OBJECT

public:
    FilesEditor( QWidget* parent = 0 );
    virtual ~FilesEditor();
    
    bool isQuoteSpacedValuesVisible() const;
    bool isQuoteSpacedValuesChecked() const;
    bool isDeleteRemovedFilesVisible() const;
    bool isDeleteRemovedFilesChecked() const;
    
    virtual void setup( XUPProjectItem* project );
    virtual void finalize();

public slots:
    void setQuoteSpacedValuesVisible( bool visible );
    void setQuoteSpacedValuesChecked( bool checked );
    void setDeleteRemovedFilesVisible( bool visible );
    void setDeleteRemovedFilesChecked( bool checked );

protected:
    Ui_FilesEditor* ui;
    XUPProjectItem* mProject;
    XUPItemVariableEditorModel* mModel;
    
    QModelIndex currentVariable() const;
    QModelIndex currentValue() const;

protected slots:
    void tvVariables_selectionModel_selectionChanged();
    void on_tbAdd_clicked();
    void on_tbEdit_clicked();
};

#endif // FILESEDITOR_H
