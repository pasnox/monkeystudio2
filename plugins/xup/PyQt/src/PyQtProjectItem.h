#ifndef PYQTPROJECTITEM_H
#define PYQTPROJECTITEM_H

#include "xupmanager/core/XUPProjectItem.h"

class CLIToolPlugin;

class PyQtProjectItem : public XUPProjectItem
{
    Q_OBJECT

public:
    Q_INVOKABLE PyQtProjectItem();
    
    virtual QString projectType() const;
    virtual void installCommands();
    
protected:
    CLIToolPlugin* interpreter() const;
};

#endif // PYQTPROJECTITEM_H
