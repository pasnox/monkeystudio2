#ifndef QMAKEPROJECTEDITOR_H
#define QMAKEPROJECTEDITOR_H

#include <pluginsmanager.h>

#include <QPointer>

class XUPItem;

class QMakeProjectEditor : public XUPPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin XUPPlugin )

public:
	QMakeProjectEditor();
	virtual ~QMakeProjectEditor();
	virtual bool setEnabled( bool );
	virtual QWidget* settingsWidget();
	virtual bool editProject( XUPItem* project );

protected:
	QPointer<XUPItem> mItem;
};

#endif // QMAKEPROJECTEDITOR_H
