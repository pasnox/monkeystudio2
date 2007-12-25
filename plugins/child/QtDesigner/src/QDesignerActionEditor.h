#ifndef QDESIGNERACTIONEDITOR_H
#define QDESIGNERACTIONEDITOR_H

#include "pDockWidget.h"

class QtDesignerChild;
class QDesignerActionEditorInterface;

class QDesignerActionEditor : public pDockWidget
{
	Q_OBJECT

public:
	QDesignerActionEditor( QtDesignerChild* );
	inline QDesignerActionEditorInterface* interface() const { return mInterface; }
	
private:
	QDesignerActionEditorInterface* mInterface;
};

#endif // QDESIGNERACTIONEDITOR_H
