#ifndef QDESIGNEROBJECTINSPECTOR_H
#define QDESIGNEROBJECTINSPECTOR_H

#include <QDockWidget>

class QtDesignerChild;
class QDesignerObjectInspectorInterface;

class QDesignerObjectInspector : public QDockWidget
{
public:
	QDesignerObjectInspector( QtDesignerChild* );
	inline QDesignerObjectInspectorInterface* interface() const { return mInterface; }
	
private:
	QDesignerObjectInspectorInterface* mInterface;
};

#endif // QDESIGNEROBJECTINSPECTOR_H
