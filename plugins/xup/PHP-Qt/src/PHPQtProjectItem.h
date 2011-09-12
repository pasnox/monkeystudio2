#ifndef PHPQTPROJECTITEM_H
#define PHPQTPROJECTITEM_H

#include "xupmanager/core/XUPProjectItem.h"

class CLIToolPlugin;

class PHPQtProjectItem : public XUPProjectItem
{
    Q_OBJECT

public:
    Q_INVOKABLE PHPQtProjectItem();
    
    virtual QString projectType() const;
    virtual void installCommands();
    
protected:
    CLIToolPlugin* interpreter() const;
};

#endif // PHPQTPROJECTITEM_H
