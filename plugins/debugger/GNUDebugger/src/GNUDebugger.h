#ifndef GNUDEBUGGER_H
#define GNUDEBUGGER_H

#include <pluginsmanager.h>

#include <QPointer>

class GNUDebugger : public DebuggerPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin DebuggerPlugin )

public:
	GNUDebugger();
	virtual ~GNUDebugger();
	virtual bool setEnabled( bool );
	virtual QPixmap pixmap() const;
	virtual QWidget* settingsWidget();

protected slots:
	void saveSettings();
	void restoreSettings();

private:
	QPointer<QAction> aSeparator;
	QHash<QString , QAction*> actionList;
};

#endif
