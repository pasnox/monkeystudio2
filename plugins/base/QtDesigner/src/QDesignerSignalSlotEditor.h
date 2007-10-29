#ifndef QDESIGNERSIGNALSLOTEDITOR_H
#define QDESIGNERSIGNALSLOTEDITOR_H

#include <QDockWidget>

class QtDesignerChild;

class QDesignerSignalSlotEditor : public QDockWidget
{
public:
	QDesignerSignalSlotEditor( QtDesignerChild* );
	
private:
	QWidget* mInterface;
};

#endif // QDESIGNERSIGNALSLOTEDITOR_H
