#ifndef QDESIGNERWIDGETBOX_H
#define QDESIGNERWIDGETBOX_H

#include "pDockWidget.h"

class QtDesignerChild;
class QDesignerWidgetBoxInterface;

class QDesignerWidgetBox : public pDockWidget
{
public:
	QDesignerWidgetBox( QtDesignerChild* ) ;
	inline QDesignerWidgetBoxInterface* interface() const { return mInterface; }
	
private:
	QDesignerWidgetBoxInterface* mInterface;
};

#endif // QDESIGNERWIDGETBOX_H
