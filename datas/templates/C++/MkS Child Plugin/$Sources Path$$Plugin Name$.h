#ifndef $Plugin Name.upper$_H
#define $Plugin Name.upper$_H

#include <pluginsmanager/ChildPlugin.h>

class $Plugin Name$ : public QObject, public ChildPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin ChildPlugin )

public:
    virtual pAbstractChild* createDocument( const QString& fileName );

protected:
    virtual void fillPluginInfos();

    virtual bool install();
    virtual bool uninstall();
};

#endif // $Plugin Name.upper$_H
