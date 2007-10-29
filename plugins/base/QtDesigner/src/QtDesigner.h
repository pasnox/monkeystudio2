#ifndef QTDESIGNER_H
#define QTDESIGNER_H

#include "ChildPlugin.h"

class QtDesigner : public ChildPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ChildPlugin )
	
public:
	QtDesigner();
	virtual ~QtDesigner();
	virtual QWidget* settingsWidget();
	virtual bool setEnabled( bool );
	virtual bool openFile( const QString&, const QPoint& = QPoint() );
	
};

#endif // QTDESIGNER_H
