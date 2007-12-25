#ifndef QDESIGNERSIGNALSLOTEDITOR_H
#define QDESIGNERSIGNALSLOTEDITOR_H

#include "pDockWidget.h"

class QtDesignerChild;

class QDesignerSignalSlotEditor : public pDockWidget
{
public:
	QDesignerSignalSlotEditor( QtDesignerChild* );
	inline QWidget* interface() const { return mInterface; }
	
private:
	QWidget* mInterface;
};

#endif // QDESIGNERSIGNALSLOTEDITOR_H
