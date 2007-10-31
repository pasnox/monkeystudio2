#ifndef QDESIGNERSIGNALSLOTEDITOR_H
#define QDESIGNERSIGNALSLOTEDITOR_H

#include <QDockWidget>

class QtDesignerChild;

class QDesignerSignalSlotEditor : public QDockWidget
{
public:
	QDesignerSignalSlotEditor( QtDesignerChild* );
	inline QWidget* interface() const { return mInterface; }
	
private:
	QWidget* mInterface;
};

#endif // QDESIGNERSIGNALSLOTEDITOR_H
