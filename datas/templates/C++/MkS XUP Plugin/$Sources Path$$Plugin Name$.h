#ifndef $Plugin Name.upper$_H
#define $Plugin Name.upper$_H

#include <XUPPlugin.h>

class $Plugin Name$ : public XUPPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin XUPPlugin )

public:
    virtual bool editProject( XUPProjectItem* project );

protected:
    virtual void fillPluginInfos();

    virtual bool install();
    virtual bool uninstall();
};

#endif // $Plugin Name.upper$_H
