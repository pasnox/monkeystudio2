#ifndef COMMANDEDITOR_H
#define COMMANDEDITOR_H

#include <QFrame>

#include "consolemanager/pCommand.h"

class Ui_CommandEditor;
class QEvent;

class CommandEditor : public QFrame
{
    Q_OBJECT

public:
    CommandEditor( QWidget* parent = 0 );
    virtual ~CommandEditor();
    
    pCommand command() const;

public slots:
    virtual void retranslateUi();
    void setCommand( const pCommand& command );
    
protected:
    Ui_CommandEditor* ui;
    
    virtual void changeEvent( QEvent* event );

protected slots:
    void on_tbCommand_clicked();
    void on_tbWorkDir_clicked();
    void on_tbHelp_clicked();
};

#endif // COMMANDEDITOR_H
