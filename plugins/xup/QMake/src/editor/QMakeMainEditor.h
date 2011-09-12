#ifndef QMAKEMAINEDITOR_H
#define QMAKEMAINEDITOR_H

#include <xupmanager/gui/XUPPageEditor.h>
#include "UIQMakeEditor.h"

class Ui_QMakeMainEditor;

class QMakeMainEditor : public XUPPageEditor
{
    Q_OBJECT

public:
    enum Template {
        Solution = 0,
        Application,
        StaticLibrary,
        SharedLibrary,
        QtPlugin,
        QtDesignerPlugin
    };
    
    QMakeMainEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent = 0 );
    virtual ~QMakeMainEditor();

    virtual void setup( XUPProjectItem* project );
    virtual void finalize();

protected:
    UIQMakeEditor::ProjectValues& mPositiveValues;
    UIQMakeEditor::ProjectValues& mNegativeValues;
    Ui_QMakeMainEditor* ui;
    XUPProjectItem* mProject;
    
    bool isSolution() const;

protected slots:
    void projectTypeChanged();
    void on_tbProjectTarget_clicked();
};

#endif // QMAKEMAINEDITOR_H
