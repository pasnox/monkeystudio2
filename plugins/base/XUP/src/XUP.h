#ifndef XUP_H
#define XUP_H

#include "BasePlugin.h"

#include <QPointer>

class UIXUPManager;

class XUP : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	XUP();
	virtual ~XUP();
	virtual bool setEnabled( bool );

protected:
	QPointer<UIXUPManager> mDockXUPManager;
};

#endif // XUP_H
