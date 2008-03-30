#ifndef QMAKEPROJECTEDITOR_H
#define QMAKEPROJECTEDITOR_H

#include <pluginsmanager.h>

class XUPItem;

class QMakeProjectEditor : public XUPPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin XUPPlugin )

public:
	QMakeProjectEditor();
	virtual ~QMakeProjectEditor();
	virtual bool setEnabled( bool );
	virtual bool editProject( XUPItem* project );

protected:
	XUPItem* mItem;
};

#endif // QMAKEPROJECTEDITOR_H
