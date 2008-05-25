#ifndef GNUDEBUGGER_H
#define GNUDEBUGGER_H

#include <pluginsmanager.h>
#include "DockGNUDebugger.h"

class GNUDebugger : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	GNUDebugger();
	virtual ~GNUDebugger();
	virtual bool setEnabled( bool );

protected slots:
	void saveSettings();
	void restoreSettings();
	QWidget* settingsWidget();

private:

	QHash<QString , QAction *> actionList;

	DockGNUDebugger * mDockGNUDebugger;
};

#endif
