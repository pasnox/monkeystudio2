#ifndef QTASSISTANT_H
#define QTASSISTANT_H

#include "ChildPlugin.h"

class QtAssistant : public ChildPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ChildPlugin )
	
public:
	QtAssistant();
	virtual ~QtAssistant();
	virtual QWidget* settingsWidget();
	virtual bool setEnabled( bool );
	virtual pAbstractChild* openFile( const QString&, const QPoint& = QPoint() ) { return 0; }
	
};

#endif // QTASSISTANT_H
