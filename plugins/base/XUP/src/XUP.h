#ifndef XUP_H
#define XUP_H

#include "XUPPlugin.h"

#include <QPointer>

class UIXUPManager;

class XUP : public XUPPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin XUPPlugin )

public:
	XUP();
	virtual ~XUP();
	virtual bool setEnabled( bool );
	virtual bool editProject( XUPItem* project );

protected:
	QPointer<UIXUPManager> mDockXUPManager;
};

#endif // XUP_H
