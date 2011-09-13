#ifndef $Plugin Name.upper$_H
#define $Plugin Name.upper$_H

#include <pluginsmanager/BasePlugin.h>

class $Plugin Name$ : public BasePlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin )

protected:
    virtual void fillPluginInfos();

    virtual bool install();
    virtual bool uninstall();
};

#endif // $Plugin Name.upper$_H
