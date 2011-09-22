#ifndef COMMANDSEDITOR_H
#define COMMANDSEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"

#include "xupmanager/core/XUPProjectItemHelper.h"

#include <QModelIndex>
#include <QItemSelection>

class Ui_CommandsEditor;
class CommandsEditorModel;

class Q_MONKEY_EXPORT CommandsEditor : public XUPPageEditor
{
    Q_OBJECT

public:
    CommandsEditor( QWidget* parent = 0 );
    virtual ~CommandsEditor();
    
    virtual void setup( XUPProjectItem* project );
    virtual void finalize();

protected:
    Ui_CommandsEditor* ui;
    CommandsEditorModel* mModel;
    XUPProjectItem* mProject;
    
    void setCommand( const QModelIndex& commandIndex );
    void getCommand( const QModelIndex& commandIndex );
    void updateState();

protected slots:
    void tvCommands_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
    void on_tbCommandAdd_clicked();
    void on_tbCommandUp_clicked();
    void on_tbCommandDown_clicked();
};

#endif // COMMANDSEDITOR_H
