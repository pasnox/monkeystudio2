#ifndef MakefileProjectItem_H
#define MakefileProjectItem_H

#include "xupmanager/core/XUPProjectItem.h"

class CLIToolPlugin;

class MakefileProjectItem : public XUPProjectItem
{
    Q_OBJECT

public:
    Q_INVOKABLE MakefileProjectItem();
    
    virtual QString projectType() const;
    virtual void installCommands();
    virtual void addFiles( const QStringList& files, XUPItem* scope = 0 );
    virtual void removeValue( XUPItem* item, bool deleteFiles );
    virtual bool open( const QString& fileName, const QString& codec );

protected:
    virtual UIXUPEditor* newEditDialog() const;
};

#endif // MakefileProjectItem_H
