#ifndef XUPPROJECTEDITOR_H
#define XUPPROJECTEDITOR_H

#include <pluginsmanager.h>

class XUPItem;

class XUPProjectEditor : public XUPPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin XUPPlugin )

public:
	XUPProjectEditor();
	virtual ~XUPProjectEditor();
	virtual bool setEnabled( bool );
	virtual bool editProject( XUPItem* project );
};

#endif // XUPPROJECTEDITOR_H
