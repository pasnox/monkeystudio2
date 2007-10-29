#ifndef QDESIGNERWIDGETBOX_H
#define QDESIGNERWIDGETBOX_H

#include <QDockWidget>

class QtDesignerChild;
class QDesignerWidgetBoxInterface;

class QDesignerWidgetBox : public QDockWidget
{
public:
	QDesignerWidgetBox( QtDesignerChild* ) ;
	
private:
	QDesignerWidgetBoxInterface* mInterface;
};

#endif // QDESIGNERWIDGETBOX_H
