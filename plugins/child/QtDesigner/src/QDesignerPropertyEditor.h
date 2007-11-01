#ifndef QDESIGNERPROPERTYEDITOR_H
#define QDESIGNERPROPERTYEDITOR_H

#include <QDockWidget>

class QtDesignerChild;
class QDesignerFormWindowInterface;
class QDesignerPropertyEditorInterface;

class QDesignerPropertyEditor : public QDockWidget
{
	Q_OBJECT
	
public:
	QDesignerPropertyEditor( QtDesignerChild* );
	inline QDesignerPropertyEditorInterface* interface() const { return mInterface; }
	
private:
	QDesignerPropertyEditorInterface* mInterface;
};

#endif // QDESIGNERPROPERTYEDITOR_H
