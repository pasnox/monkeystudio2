#ifndef UIQMAKEEDITOR_H
#define UIQMAKEEDITOR_H

#include <xupmanager/gui/UIXUPEditor.h>

#include <QHash>

class XUPItem;

class UIQMakeEditor : public UIXUPEditor
{
    Q_OBJECT
    
public:
    typedef QHash<QString, QStringList> ProjectValues;
    UIQMakeEditor( QWidget* parent = 0 );
    virtual ~UIQMakeEditor();
    
    virtual void setupProject( XUPProjectItem* project );
    
    static QStringList handledVariables();

protected:
    UIQMakeEditor::ProjectValues mPositiveValues;
    UIQMakeEditor::ProjectValues mNegativeValues;
    
    virtual void finalize();
    
    void initializeVariables( XUPProjectItem* project );
    XUPItem* uniqueVariable( XUPItem* scope, const QString& variableName, bool positive, bool create );
    void updateVariable( XUPItem* scope, const QString& variableName, bool positive, const QStringList& values );
};

#endif // UIQMAKEEDITOR_H
