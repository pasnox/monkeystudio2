#ifndef $Plugin Name.upper$_H
#define $Plugin Name.upper$_H

#include <pluginsmanager/DebuggerPlugin.h>

class $Plugin Name$ : public QObject, public DebuggerPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin DebuggerPlugin )

protected:
    virtual void fillPluginInfos();

    virtual bool install();
    virtual bool uninstall();
};

#endif // $Plugin Name.upper$_H
