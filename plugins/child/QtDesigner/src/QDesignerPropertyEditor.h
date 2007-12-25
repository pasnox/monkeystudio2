#ifndef QDESIGNERPROPERTYEDITOR_H
#define QDESIGNERPROPERTYEDITOR_H

#include "pDockWidget.h"

class QtDesignerChild;
class QDesignerFormWindowInterface;
class QDesignerPropertyEditorInterface;

class QDesignerPropertyEditor : public pDockWidget
{
	Q_OBJECT
	
public:
	QDesignerPropertyEditor( QtDesignerChild* );
	inline QDesignerPropertyEditorInterface* interface() const { return mInterface; }
	
private:
	QDesignerPropertyEditorInterface* mInterface;
};

#endif // QDESIGNERPROPERTYEDITOR_H
