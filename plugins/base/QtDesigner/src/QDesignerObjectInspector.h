#ifndef QDESIGNEROBJECTINSPECTOR_H
#define QDESIGNEROBJECTINSPECTOR_H

#include <QDockWidget>

class QtDesignerChild;
class QDesignerObjectInspectorInterface;

class QDesignerObjectInspector : public QDockWidget
{
public:
	QDesignerObjectInspector( QtDesignerChild* );
	
private:
	QDesignerObjectInspectorInterface* mInterface;
};

#endif // QDESIGNEROBJECTINSPECTOR_H
