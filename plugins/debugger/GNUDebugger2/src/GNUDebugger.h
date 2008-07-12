#ifndef GNUDEBUGGER_H
#define GNUDEBUGGER_H

#include <pluginsmanager.h>
#include "DockGNUDebugger.h"

// for loading one time the parser file
#include "./kernel/gdbPatternFile.1.3.h"

#include <QPointer>

//! Main class
/**
\author xiantia
\version 1.3.2
*/

class GNUDebugger : public DebuggerPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin DebuggerPlugin  )

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
	QPointer<GdbPatternFile> patternFile;

	QHash<QString , QAction*> actionList;
	QPointer<DockGNUDebugger> mDockGNUDebugger;
	
};

#endif
