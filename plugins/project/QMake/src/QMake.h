#ifndef QMAKE_H
#define QMAKE_H

#include "ProjectPlugin.h"

class QMake : public ProjectPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ProjectPlugin )

public:
	QMake();
	virtual bool setEnabled( bool );
	virtual QWidget* settingsWidget();
	virtual ProjectItem* getProjectItem( const QString& );

};

#endif // QMAKE_H
