#ifndef QDESIGNERACTIONEDITOR_H
#define QDESIGNERACTIONEDITOR_H

#include <QDockWidget>

class QtDesignerChild;
class QDesignerActionEditorInterface;

class QDesignerActionEditor : public QDockWidget
{
	Q_OBJECT

public:
	QDesignerActionEditor( QtDesignerChild* );
	
private:
	QDesignerActionEditorInterface* mInterface;
};

#endif // QDESIGNERACTIONEDITOR_H
