#ifndef QDESIGNEROBJECTINSPECTOR_H
#define QDESIGNEROBJECTINSPECTOR_H

#include "pDockWidget.h"

class QtDesignerChild;
class QDesignerObjectInspectorInterface;

class QDesignerObjectInspector : public pDockWidget
{
public:
	QDesignerObjectInspector( QtDesignerChild* );
	inline QDesignerObjectInspectorInterface* interface() const { return mInterface; }
	
private:
	QDesignerObjectInspectorInterface* mInterface;
};

#endif // QDESIGNEROBJECTINSPECTOR_H
