#ifndef QMAKEPROJECTEDITOR_H
#define QMAKEPROJECTEDITOR_H

#include <pluginsmanager.h>

#include <QPointer>

class XUPProjectItem;

class QMakeProjectEditor : public XUPPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin XUPPlugin )

public:
	QMakeProjectEditor();
	virtual ~QMakeProjectEditor();
	virtual bool setEnabled( bool );
	virtual QWidget* settingsWidget();
	virtual bool editProject( XUPProjectItem* project );

protected:
	QPointer<XUPProjectItem> mItem;
};

#endif // QMAKEPROJECTEDITOR_H
