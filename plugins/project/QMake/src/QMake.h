#ifndef QMAKE_H
#define QMAKE_H

#include "ProjectPlugin.h"

class QMake : public ProjectPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ProjectPlugin )

public:
	QMake();
	virtual ~QMake();
	virtual bool setEnabled( bool );
	virtual QWidget* settingsWidget();
	virtual ProjectItem* openProject( const QString&, ProjectItem* = 0 );
	virtual void editSettings( ProjectItem* );

};

#endif // QMAKE_H
